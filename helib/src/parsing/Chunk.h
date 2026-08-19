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
	constexpr uint32_t CHUNK_ID_SIZE = 4;
	constexpr auto HEADER_SIZE = CHUNK_ID_SIZE + sizeof(uint32_t);

	//---------------------------------------------------------------------
	class Chunk
	{
	public:
		std::string GetTag() const;
		void SetTag(const void* a_sTag);

		size_t ChunkSize() const;
		size_t WholeChunkSize() const;

		Chunk* TryFindChild(const std::string_view& a_sChunkID);
		Chunk* FindChunkAt(size_t a_iTarget, size_t a_iBase = 0);
		Chunk* GetRoot();

		size_t GetOffsetFromRoot() const;

		const core::Data& GetData() const;
		core::Data& GetData();
		void SetData(const core::Data& a_Data);

		std::vector<std::unique_ptr<Chunk>>& GetChildren();
		const std::vector<std::unique_ptr<Chunk>>& GetChildren() const;

		const Chunk* GetParent() const;
		Chunk* GetParent();
		void SetParent(Chunk& a_Parent);

		bool IsEncrypted() const;
		void SetEncrypted(bool a_bIsEncrypted);

		unsigned char GetEncryptionKey() const;
		void SetEncryptionKey(unsigned char a_cEncryptionKey);

		void ToData(core::DataStream& a_Data) const;

		char m_sTag[CHUNK_ID_SIZE] = {};
	private:
		core::Data m_Data;		   // leaf: Owns its data.
		std::vector<std::unique_ptr<Chunk>> m_aChildren; // container: Owns its children.
		Chunk* m_pParent = nullptr;

		bool m_bIsEncrypted = false;
		unsigned char m_cEncryptionKey = 0x69;
	};
}