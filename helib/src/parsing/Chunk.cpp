#include "Chunk.h"

#include <cassert>
#include <limits>

#include "core/DataStream.h"
#include "core/Memory.h"

namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	// Chunk
	//---------------------------------------------------------------------
	std::string Chunk::GetTag() const
	{
		return std::string(m_sTag, CHUNK_ID_SIZE);
	}

	//---------------------------------------------------------------------
	void Chunk::SetTag(const void* a_sTag)
	{
		assert(a_sTag != nullptr);
		memcpy(m_sTag, a_sTag, CHUNK_ID_SIZE);
	}

	//---------------------------------------------------------------------
	size_t Chunk::ChunkSize() const
	{
		if (m_aChildren.empty())
		{
			return m_Data.size();
		}

		size_t childrenSize = 0;
		for (const std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			childrenSize += chunk->WholeChunkSize();
		}
		return childrenSize;
	}

	//---------------------------------------------------------------------
	size_t Chunk::WholeChunkSize() const
	{
		return ChunkSize() + CHUNK_HEADER_SIZE;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::TryFindChild(const std::string_view& a_sChunkID)
	{
		Chunk* found = nullptr;
		for (std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			if (core::chunkcmp(chunk->m_sTag, a_sChunkID) == 0)
			{
				return chunk.get();
			}

			if (found = chunk->TryFindChild(a_sChunkID))
			{
				return found;
			}
		}
		return found;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::FindChunkAt(size_t a_iTarget, size_t a_iBase)
	{
		const size_t totalSize = m_bIsRoot
			? ChunkSize()
			: WholeChunkSize();

		if (a_iTarget < a_iBase ||
			a_iTarget - a_iBase >= totalSize)
		{
			return nullptr;
		}

		// The synthetic root has no serialized representation.
		if (m_bIsRoot)
		{
			size_t childBase = a_iBase;

			for (const std::unique_ptr<Chunk>& child : m_aChildren)
			{
				const size_t childSize = child->WholeChunkSize();

				if (a_iTarget - childBase < childSize)
				{
					return child->FindChunkAt(a_iTarget, childBase);
				}

				childBase += childSize;
			}

			return this;
		}

		const size_t relative = a_iTarget - a_iBase;

		// This chunk's header belongs to this chunk.
		if (relative < CHUNK_HEADER_SIZE)
		{
			return this;
		}

		// Leaf: everything after the header is its own data.
		if (m_aChildren.empty())
		{
			return this;
		}

		// Container: payload consists of child chunks.
		size_t childBase = a_iBase + CHUNK_HEADER_SIZE;

		for (const std::unique_ptr<Chunk>& child : m_aChildren)
		{
			const size_t childSize = child->WholeChunkSize();

			if (a_iTarget - childBase < childSize)
			{
				return child->FindChunkAt(a_iTarget, childBase);
			}

			childBase += childSize;
		}

		return this;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::GetRoot()
	{
		Chunk* parent = this;
		while (parent && parent->m_pParent)
		{
			Chunk* cparent = parent->m_pParent;

			// If it is the root and it has only one child, return the actual root.
			bool isRoot = cparent->m_bIsRoot;
			bool oneChild = cparent->m_aChildren.size() == 1;

			if (isRoot && oneChild)
			{
				break;
			}
			parent = parent->m_pParent;
		}
		return parent;
	}

	//---------------------------------------------------------------------
	size_t Chunk::GetOffsetFromRoot() const
	{
		size_t offset = 0;
		const Chunk* current = this;

		while (current->m_pParent)
		{
			const Chunk* parent = current->m_pParent;

			// A real parent has a header before its children.
			if (!parent->m_bIsRoot)
			{
				offset += CHUNK_HEADER_SIZE;
			}

			// Add the serialized size of all preceding siblings.
			for (const std::unique_ptr<Chunk>& sibling : parent->m_aChildren)
			{
				if (sibling.get() == current)
				{
					break;
				}

				offset += sibling->WholeChunkSize();
			}

			current = parent;
		}

		return offset;
	}

	//---------------------------------------------------------------------
	const core::Data& Chunk::GetData() const
	{
		return m_Data;
	}

	//---------------------------------------------------------------------
	core::Data& Chunk::GetData()
	{
		return m_Data;
	}

	//---------------------------------------------------------------------
	void Chunk::SetData(const core::Data& a_Data)
	{
		m_Data = a_Data;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<Chunk>>& Chunk::GetChildren()
	{
		return m_aChildren;
	}

	//---------------------------------------------------------------------
	const std::vector<std::unique_ptr<Chunk>>& Chunk::GetChildren() const
	{
		return m_aChildren;
	}

	//---------------------------------------------------------------------
	const Chunk* Chunk::GetParent() const
	{
		return m_pParent;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::GetParent()
	{
		return m_pParent;
	}

	//---------------------------------------------------------------------
	void Chunk::SetParent(Chunk& a_Parent)
	{
		m_pParent = &a_Parent;
	}

	//---------------------------------------------------------------------
	bool Chunk::IsEncrypted() const
	{
		return m_bIsEncrypted;
	}

	//---------------------------------------------------------------------
	void Chunk::SetEncrypted(bool a_bIsEncrypted)
	{
		m_bIsEncrypted = a_bIsEncrypted;
	}

	//---------------------------------------------------------------------
	unsigned char Chunk::GetEncryptionKey() const
	{
		return m_cEncryptionKey;
	}

	//---------------------------------------------------------------------
	void Chunk::SetEncryptionKey(unsigned char a_cEncryptionKey)
	{
		m_cEncryptionKey = a_cEncryptionKey;
	}

	//---------------------------------------------------------------------
	void Chunk::ToData(core::DataStream& a_Data) const
	{
		a_Data.Write(m_sTag, sizeof(m_sTag));

		const size_t chunkSizest = WholeChunkSize();
		assert(chunkSizest <= std::numeric_limits<uint32_t>::max());
		uint32_t size32 = static_cast<uint32_t>(chunkSizest);
		unsigned char chunkSize[sizeof(uint32_t)] = {};

		core::WriteBE32(chunkSize, size32);
		a_Data.Write(chunkSize, sizeof(chunkSize));

		if (!m_Data.empty())
		{
			a_Data.Write(m_Data.data(), m_Data.size());
		}
		else
		{
			for (const std::unique_ptr<Chunk>& child : m_aChildren)
			{
				child->ToData(a_Data);
			}
		}
	}

	//---------------------------------------------------------------------
	void Chunk::SetAsRoot()
	{
		m_bIsRoot = true;
	}
}