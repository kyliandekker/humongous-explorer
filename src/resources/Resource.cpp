#include "./Resource.h"

#include "parsing/ChunkParser.h"

#include "resources/ResourceType.h"

#include "parsing/Chunk.h"

#include "core/Memory.h"

#include "parsing/ChunkIDs.h"
#include "parsing/chunks/image/BMAP_Chunk.h"
#include "parsing/chunks/image/RMHD_Chunk.h"
#include "parsing/chunks/image/APAL_Chunk.h"
#include "parsing/chunks/image/TRNS_Chunk.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	// SongResource
	//---------------------------------------------------------------------
	uint16_t SoundResource::GetSampleRate() const
	{
		return m_iSampleRate;
	}

	//---------------------------------------------------------------------
	void SoundResource::SetSampleRate(uint16_t a_iSampleRate)
	{
		m_iSampleRate = a_iSampleRate;
	}

	//---------------------------------------------------------------------
	core::Data SoundResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}
	
	//---------------------------------------------------------------------
	void SoundResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	std::string SoundResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return "";
		}
		return core::SizeToString(m_pDataChunk->ChunkSize());
	}

	//---------------------------------------------------------------------
	std::string SoundResource::GetDurationStr()
	{
		if (!m_pDataChunk)
		{
			return "";
		}

		size_t bytesPerSecond =
			(size_t)m_iSampleRate * 1 * (8 / 8);

		size_t totalMs =
			(m_pDataChunk->GetData().size() * 1000) / bytesPerSecond;

		// size_t hours = totalMs / 3600000;
		size_t minutes = (totalMs % 3600000) / 60000;
		size_t seconds = (totalMs % 60000) / 1000;
		size_t ms = totalMs % 1000;

		static char buffer[32];
		snprintf(buffer, sizeof(buffer),
			"%zu:%02zu.%03zu",
			minutes, seconds, ms);

		return buffer;
	}

	//---------------------------------------------------------------------
	SongResource::SongResource() : SoundResource()
	{
		m_eResourceType = resources::ResourceType::Song;
	}

	//---------------------------------------------------------------------
	// TalkResource
	//---------------------------------------------------------------------
	TalkResource::TalkResource() : SoundResource()
	{
		m_eResourceType = resources::ResourceType::Talkie;
	}

	//---------------------------------------------------------------------
	core::Data TalkResource::GetLipSyncData() const
	{
		if (!m_pLipSyncChunk)
		{
			return core::Data();
		}
		return m_pLipSyncChunk->GetData();
	}

	//---------------------------------------------------------------------
	void TalkResource::SetLipSyncChunk(parsing::Chunk* a_pChunk)
	{
		m_pLipSyncChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	// SFXResource
	//---------------------------------------------------------------------
	SFXResource::SFXResource() : SoundResource()
	{
		m_eResourceType = resources::ResourceType::SFX;
	}

	//---------------------------------------------------------------------
	// RoomResource
	//---------------------------------------------------------------------
	RoomResource::RoomResource() : Resource()
	{
		m_eResourceType = resources::ResourceType::Room;
	}

	//---------------------------------------------------------------------
	std::string RoomResource::GetSize() const
	{
		return "";
	}

	//---------------------------------------------------------------------
	std::string RoomResource::GetDurationStr()
	{
		return "";
	}

	//---------------------------------------------------------------------
	// LocalScriptResource
	//---------------------------------------------------------------------
	LocalScriptResource::LocalScriptResource() : Resource()
	{
		m_eResourceType = resources::ResourceType::LocalScript;
	}

	//---------------------------------------------------------------------
	core::Data LocalScriptResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}

	//---------------------------------------------------------------------
	void LocalScriptResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	std::string LocalScriptResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return "";
		}
		return core::SizeToString(m_pDataChunk->ChunkSize());
	}

	//---------------------------------------------------------------------
	std::string LocalScriptResource::GetDurationStr()
	{
		return "";
	}

	//---------------------------------------------------------------------
	// GlobalScriptResource
	//---------------------------------------------------------------------
	GlobalScriptResource::GlobalScriptResource() : Resource()
	{
		m_eResourceType = resources::ResourceType::GlobalScript;
	}

	//---------------------------------------------------------------------
	core::Data GlobalScriptResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}

	//---------------------------------------------------------------------
	void GlobalScriptResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	std::string GlobalScriptResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return "";
		}
		return core::SizeToString(m_pDataChunk->ChunkSize());
	}

	//---------------------------------------------------------------------
	std::string GlobalScriptResource::GetDurationStr()
	{
		return "";
	}

	//---------------------------------------------------------------------
	// VerbScriptResource
	//---------------------------------------------------------------------
	VerbScriptResource::VerbScriptResource() : Resource()
	{
		m_eResourceType = resources::ResourceType::VerbScript;
	}

	//---------------------------------------------------------------------
	core::Data VerbScriptResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}

	//---------------------------------------------------------------------
	void VerbScriptResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	std::string VerbScriptResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return "";
		}
		return core::SizeToString(m_pDataChunk->ChunkSize());
	}

	//---------------------------------------------------------------------
	std::string VerbScriptResource::GetDurationStr()
	{
		return "";
	}

	//---------------------------------------------------------------------
	// RoomBackgroundResource
	//---------------------------------------------------------------------
	RoomBackgroundResource::RoomBackgroundResource() : Resource()
	{
		m_eResourceType = resources::ResourceType::RoomBackground;
	}

	//---------------------------------------------------------------------
	core::Data RoomBackgroundResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}

	//---------------------------------------------------------------------
	void RoomBackgroundResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	std::string RoomBackgroundResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return "";
		}
		return core::SizeToString(m_pDataChunk->ChunkSize());
	}

	//---------------------------------------------------------------------
	std::string RoomBackgroundResource::GetDurationStr()
	{
		return "";
	}

	std::vector<uint8_t> CreateBitstream(const unsigned char* a_Data, size_t a_Length)
	{
		std::vector<uint8_t> bits;
		bits.reserve(a_Length * 8);

		for (size_t i = 0; i < a_Length; ++i)
		{
			const char c = a_Data[i];
			for (int j = 0; j < 8; j++)
			{
				bits.push_back((c >> j) & 1);
			}
		}
		return bits;
	}

	uint8_t CollectBits(int& a_Pos, std::vector<uint8_t>& a_Bitstream, int a_Count)
	{
		int result = 0;
		for (int i = 0; i < a_Count; i++)
		{
			result |= a_Bitstream[a_Pos++] << i;
		}

		return result;
	}

	bool DecodeHE(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		unsigned char color = a_FillColor;

		const size_t num_pixels = a_iWidth * a_iHeight;
		out.reserve(num_pixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < num_pixels; i++)
			{
				out.push_back(color % 256);
			}
		}
		else
		{
			std::vector<int> delta_color = { -4, -3, -2, -1, 1, 2, 3, 4 };

			std::vector<uint8_t> bits = CreateBitstream(a_BMAPData.dataAs<unsigned char>(), a_BMAPData.size());

			out.push_back(color % 256);

			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						const uint8_t bitc = CollectBits(pos, bits, 3);
						color += delta_color[bitc];
					}
					else
					{
						color = CollectBits(pos, bits, a_iPalen);
					}
				}
				out.push_back(color % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//---------------------------------------------------------------------
	void RoomBackgroundResource::Open()
	{
		m_aColors.clear();

		// BMAP is the data chunk.
		parsing::Chunk* bmap = m_pDataChunk->TryFindChild(parsing::BMAP_CHUNK_ID);
		parsing::BMAP_Chunk* bmapData = bmap->GetData().dataAs<parsing::BMAP_Chunk>();

		const int palen = bmapData->encoding % 10;
		
		// Determine which version.
		const bool he = bmapData->encoding >= 0x86 && bmapData->encoding <= 0x8A;
		const bool he_transparent = bmapData->encoding >= 0x90 && bmapData->encoding <= 0x94;

		// Get the RMIH and RMHD chunks.
		parsing::Chunk* rmih = m_pDataChunk->GetParent()->GetParent()->TryFindChild(parsing::RMDA_CHUNK_ID);

		parsing::Chunk* rmhd = rmih->TryFindChild(parsing::RMHD_CHUNK_ID);
		if (!rmhd)
		{
			return;
		}

		parsing::RMHD_Chunk* rmhdData = rmhd->GetData().dataAs<parsing::RMHD_Chunk>();
		if (!rmhdData)
		{
			return;
		}

		m_iWidth = rmhdData->width;
		m_iHeight = rmhdData->height;

		parsing::Chunk* apal = rmih->TryFindChild(parsing::APAL_CHUNK_ID);
		if (!apal)
		{
			return;
		}

		parsing::APAL_Chunk* apalData = apal->GetData().dataAs<parsing::APAL_Chunk>();
		if (!apalData)
		{
			return;
		}

		parsing::Chunk* trns = rmih->TryFindChild(parsing::TRNS_CHUNK_ID);
		if (!trns)
		{
			return;
		}

		parsing::TRNS_Chunk* trnsData = apal->GetData().dataAs<parsing::TRNS_Chunk>();
		if (!trnsData)
		{
			return;
		}

		core::Data bmapImageData = core::Data(core::add(bmap, sizeof(parsing::BMAP_Chunk)), bmap->ChunkSize() - sizeof(parsing::BMAP_Chunk));
		if (!DecodeHE(bmapData->fillColor, bmapImageData, m_iWidth, m_iHeight, palen, m_ImageData, he_transparent))
		{
			return;
		}

		std::vector<uint8_t> newOut;
		for (int i = 0; i < m_ImageData.size(); i++)
		{
			newOut.push_back(apalData->data[m_ImageData.dataAs<unsigned char>()[i] * 3]);
			newOut.push_back(apalData->data[m_ImageData.dataAs<unsigned char>()[i] * 3 + 1]);
			newOut.push_back(apalData->data[m_ImageData.dataAs<unsigned char>()[i] * 3 + 2]);
			if (m_ImageData[i] == bmapData->fillColor && he_transparent)
			{
				newOut.push_back(0);
			}
			else
			{
				newOut.push_back(255);
			}
		}

		//Color* colors = reinterpret_cast<Color*>(apalData);
		//size_t numColors = sizeof(apalData->data) / sizeof(colors);
		//for (size_t i = 0; i < numColors; i++)
		//{
		//	m_aColors.push_back({ colors->r, colors->g, colors->b });
		//	colors++;
		//}
	}

	//---------------------------------------------------------------------
	uint16_t RoomBackgroundResource::GetWidth() const
	{
		return m_iWidth;
	}

	//---------------------------------------------------------------------
	uint16_t RoomBackgroundResource::GetHeight() const
	{
		return m_iHeight;
	}

	//---------------------------------------------------------------------
	const std::vector<Color>& RoomBackgroundResource::GetColors() const
	{
		return m_aColors;
	}

	//---------------------------------------------------------------------
	// RoomImageResource
	//---------------------------------------------------------------------
	RoomImageResource::RoomImageResource() : Resource()
	{
		m_eResourceType = resources::ResourceType::RoomImage;
	}

	//---------------------------------------------------------------------
	core::Data RoomImageResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}

	//---------------------------------------------------------------------
	void RoomImageResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	std::string RoomImageResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return "";
		}
		return core::SizeToString(m_pDataChunk->ChunkSize());
	}

	//---------------------------------------------------------------------
	std::string RoomImageResource::GetDurationStr()
	{
		return "";
	}

	//---------------------------------------------------------------------
	void RoomImageResource::Open()
	{
	}
}