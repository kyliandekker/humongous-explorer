#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "core/Data.h"

namespace humongousexplorer::core
{
	class DataStream;
}
namespace humongousexplorer::parsing
{
	//---------------------------------------------------------------------
	constexpr uint32_t CHUNK_ID_SIZE = 4;
	constexpr size_t CHUNK_HEADER_SIZE = CHUNK_ID_SIZE + sizeof(uint32_t);

	//---------------------------------------------------------------------
	// Chunk
	//---------------------------------------------------------------------
	/// <summary>
	/// Represents a single chunk in a Humongous Entertainment archive, forming a tree of container and leaf nodes.
	/// </summary>
	class Chunk
	{
	public:
		/// <summary>
		/// Retrieves the 4-character tag identifier of this chunk as a string.
		/// </summary>
		/// <returns>The chunk tag string.</returns>
		std::string GetTag() const;

		/// <summary>
		/// Sets the 4-character tag identifier of this chunk from raw memory.
		/// </summary>
		/// <param name="a_sTag">Pointer to at least 4 bytes of tag data.</param>
		void SetTag(const void* a_sTag);

		/// <summary>
		/// Calculates the size of this chunk's content (excluding the 8-byte header).
		/// </summary>
		/// <returns>The content size in bytes.</returns>
		size_t ChunkSize() const;

		/// <summary>
		/// Calculates the total size of this chunk including its 8-byte header.
		/// </summary>
		/// <returns>The total chunk size in bytes.</returns>
		size_t WholeChunkSize() const;

		/// <summary>
		/// Recursively searches for a child chunk with the specified tag.
		/// </summary>
		/// <param name="a_sChunkID">The 4-character chunk tag to search for.</param>
		/// <returns>A pointer to the found chunk, or nullptr if not found.</returns>
		Chunk* TryFindChild(const std::string_view& a_sChunkID);

		/// <summary>
		/// Recursively searches for child chunks with the specified tag.
		/// </summary>
		/// <param name="a_sChunkID">The 4-character chunk tag to search for.</param>
		/// <returns>A pointer to the found chunk, or nullptr if not found.</returns>
		bool TryFindChildren(const std::string_view& a_sChunkID, std::vector<Chunk*>& a_aChunks);

		/// <summary>
		/// Finds the chunk located at a specific byte offset within this chunk's data.
		/// </summary>
		/// <param name="a_iTarget">The absolute byte offset to find.</param>
		/// <param name="a_iBase">The base offset of this chunk (default 0).</param>
		/// <returns>A pointer to the chunk at the target offset, or nullptr if out of range.</returns>
		Chunk* FindChunkAt(size_t a_iTarget, size_t a_iBase = 0);

		/// <summary>
		/// Traverses the parent chain to find and return the root chunk of the hierarchy.
		/// </summary>
		/// <returns>A pointer to the root chunk.</returns>
		Chunk* GetRoot();

		/// <summary>
		/// Calculates the byte offset of this chunk relative to the root of the hierarchy.
		/// </summary>
		/// <returns>The offset in bytes from the root chunk.</returns>
		size_t GetOffsetFromRoot() const;

		/// <summary>
		/// Retrieves the leaf data of this chunk (read-only).
		/// </summary>
		/// <returns>A const reference to the chunk's data.</returns>
		const core::Data& GetData() const;

		/// <summary>
		/// Retrieves the leaf data of this chunk (modifiable).
		/// </summary>
		/// <returns>A reference to the chunk's data.</returns>
		core::Data& GetData();

		/// <summary>
		/// Sets the leaf data of this chunk.
		/// </summary>
		/// <param name="a_Data">The data to assign to this chunk.</param>
		void SetData(const core::Data& a_Data);

		/// <summary>
		/// Retrieves the list of child chunks (modifiable).
		/// </summary>
		/// <returns>A reference to the vector of child chunks.</returns>
		std::vector<std::unique_ptr<Chunk>>& GetChildren();

		/// <summary>
		/// Retrieves the list of child chunks (read-only).
		/// </summary>
		/// <returns>A const reference to the vector of child chunks.</returns>
		const std::vector<std::unique_ptr<Chunk>>& GetChildren() const;

		/// <summary>
		/// Retrieves the parent chunk (read-only).
		/// </summary>
		/// <returns>A pointer to the parent chunk, or nullptr if this is the root.</returns>
		const Chunk* GetParent() const;

		/// <summary>
		/// Retrieves the parent chunk (modifiable).
		/// </summary>
		/// <returns>A pointer to the parent chunk, or nullptr if this is the root.</returns>
		Chunk* GetParent();

		/// <summary>
		/// Sets the parent chunk of this chunk.
		/// </summary>
		/// <param name="a_Parent">The chunk to set as this chunk's parent.</param>
		void SetParent(Chunk& a_Parent);

		/// <summary>
		/// Checks whether this chunk's data is XOR-encrypted.
		/// </summary>
		/// <returns>True if the chunk is encrypted, otherwise false.</returns>
		bool IsEncrypted() const;

		/// <summary>
		/// Sets the encryption state of this chunk.
		/// </summary>
		/// <param name="a_bIsEncrypted">True to mark the chunk as encrypted, false otherwise.</param>
		void SetEncrypted(bool a_bIsEncrypted);

		/// <summary>
		/// Retrieves the XOR encryption key used for this chunk.
		/// </summary>
		/// <returns>The encryption key byte.</returns>
		unsigned char GetEncryptionKey() const;

		/// <summary>
		/// Sets the XOR encryption key for this chunk.
		/// </summary>
		/// <param name="a_cEncryptionKey">The encryption key byte.</param>
		void SetEncryptionKey(unsigned char a_cEncryptionKey);

		/// <summary>
		/// Serializes this chunk and all its children into a DataStream.
		/// </summary>
		/// <param name="a_Data">The output DataStream to write the serialized chunk into.</param>
		void ToData(core::DataStream& a_Data) const;

		void SetAsRoot();

		char m_sTag[CHUNK_ID_SIZE] = {};
	private:
		bool m_bIsRoot = false;

		core::Data m_Data;		   // leaf: Owns its data.
		std::vector<std::unique_ptr<Chunk>> m_aChildren; // container: Owns its children.
		Chunk* m_pParent = nullptr;

		bool m_bIsEncrypted = false;
		unsigned char m_cEncryptionKey = 0x69;
	};
}