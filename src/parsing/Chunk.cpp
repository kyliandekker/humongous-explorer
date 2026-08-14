#include "./Chunk.h"

#include "core/DataStream.h"
#include "core/Memory.h"

namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	// Chunk
	//---------------------------------------------------------------------
	void Chunk::Build(core::DataStream& a_Data) const
	{
		a_Data = core::DataStream(WholeChunkSize());
		BuildInner(a_Data);
		if (m_bIsEncrypted)
		{
			unsigned char* data = a_Data.dataAs<unsigned char>();
			core::xorShift(data, a_Data.size(), m_cEncryptionKey);
		}
	}

	//---------------------------------------------------------------------
	const char* Chunk::GetTag() const
	{
		return m_sTag;
	}

	//---------------------------------------------------------------------
	void Chunk::SetTag(const void* a_sTag)
	{
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
			childrenSize += chunk->ChunkSize();
		}
		return childrenSize;
	}

	//---------------------------------------------------------------------
	size_t Chunk::WholeChunkSize() const
	{
		if (m_aChildren.empty())
		{
			return m_Data.size() + HEADER_SIZE;
		}

		size_t childrenSize = 0;
		for (const std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			childrenSize += chunk->WholeChunkSize();
		}
		return childrenSize + HEADER_SIZE;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::TryFindChild(const std::string& a_sChunkID)
	{
		Chunk* found = nullptr;
		for (std::unique_ptr<Chunk>& chunk : m_aChildren)
		{
			if (core::chunkcmp(chunk->m_sTag, a_sChunkID.c_str()) == 0)
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
		size_t totalSize = WholeChunkSize();
		if (a_iTarget < a_iBase || a_iTarget >= a_iBase + totalSize)
		{
			return nullptr;
		}

		if (a_iTarget < a_iBase + 8)
		{
			return this;
		}

		size_t childPos = a_iBase + 8;
		for (auto& child : m_aChildren)
		{
			size_t childTotal = child->WholeChunkSize();
			if (a_iTarget < childPos + childTotal)
			{
				return child->FindChunkAt(a_iTarget, childPos);
			}
			childPos += childTotal;
		}
		return this;
	}

	//---------------------------------------------------------------------
	Chunk* Chunk::GetRoot()
	{
		Chunk* parent = this;
		while (parent && parent->m_pParent)
		{
			parent = parent->m_pParent;
		}
		return parent;
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
		return nullptr;
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
	void Chunk::BuildInner(core::DataStream& a_Data) const
	{
		a_Data.Write(m_sTag, sizeof(m_sTag));

		unsigned char chunkSize[sizeof(uint32_t)] = {};
		uint32_t size32 = static_cast<uint32_t>(WholeChunkSize());
		memcpy(chunkSize, reinterpret_cast<unsigned char*>(&size32), sizeof(uint32_t));

		core::reverseBytes(chunkSize, sizeof(uint32_t));
		a_Data.Write(chunkSize, sizeof(chunkSize));
		if (!m_Data.empty())
		{
			a_Data.Write(m_Data.data(), m_Data.size());
		}
		else
		{
			for (auto& child : m_aChildren)
			{
				child->BuildInner(a_Data);
			}
		}
	}
}