#include "Resource.h"

// helib
#include <helib/core/DataStream.h>
#include <helib/core/Memory.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/parsing/ChunkParser.h>
#include <helib/parsing/chunks/image/APAL_Chunk.h>
#include <helib/parsing/chunks/image/BMAP_Chunk.h>
#include <helib/parsing/chunks/image/RMHD_Chunk.h>
#include <helib/parsing/chunks/image/TRNS_Chunk.h>
#include <helib/parsing/chunks/image/IMHD_Chunk.h>

#include "resources/ResourceType.h"

#include "dx11/DX11System.h"

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
	std::string SoundResource::GetDurationStr() const
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
	void TalkResource::Replace(const core::Data& a_Data)
	{
		size_t pos = m_pChunk->GetOffsetFromRoot();
		printf("Test");
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
	static core::Data CreateBitstream(const core::Data a_Data)
	{
		core::DataStream outBits = core::DataStream(a_Data.size() * 8);

		for (size_t i = 0; i < a_Data.size(); ++i)
		{
			const char c = a_Data[i];
			for (int j = 0; j < 8; j++)
			{
				uint8_t bit = (c >> j) & 1;
				outBits.Write(&bit, sizeof(bit));
			}
		}
		return outBits;
	}

	//---------------------------------------------------------------------
	static uint8_t CollectBits(int& a_Pos, const core::Data& a_Bitstream, int a_Count)
	{
		int result = 0;
		for (int i = 0; i < a_Count; i++)
		{
			result |= a_Bitstream[a_Pos++] << i;
		}

		return result;
	}

	//---------------------------------------------------------------------
	static bool DecodeHE(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data& a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		const size_t num_pixels = a_iWidth * a_iHeight;
		out.reserve(num_pixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < num_pixels; i++)
			{
				out.push_back(a_FillColor % 256);
			}
		}
		else
		{
			std::vector<int> delta_color = { -4, -3, -2, -1, 1, 2, 3, 4 };

			core::Data bits = CreateBitstream(a_BMAPData);

			out.push_back(a_FillColor % 256);

			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						const uint8_t bitc = CollectBits(pos, bits, 3);
						a_FillColor += delta_color[bitc];
					}
					else
					{
						a_FillColor = CollectBits(pos, bits, a_iPalen);
					}
				}
				out.push_back(a_FillColor % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//---------------------------------------------------------------------
	static bool DecodeMajmin(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data& a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		const size_t numPixels = a_iWidth * a_iHeight;
		out.reserve(numPixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < numPixels; i++)
			{
				out.push_back(a_FillColor % 256);
			}
		}
		else
		{
			core::Data bits = CreateBitstream(a_BMAPData);

			out.push_back(a_FillColor % 256);

			const size_t num_pixels = a_iWidth * a_iHeight;
			out.reserve(num_pixels);

			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						const uint8_t shift = CollectBits(pos, bits, 3) - 4;
						if (shift != 0)
						{
							a_FillColor += shift;
						}
						else
						{
							uint8_t ln = CollectBits(pos, bits, 8) - 1;
							for (size_t i = 0; i < ln; i++)
							{
								out.push_back((a_FillColor % 256));
							}
						}
					}
					else
					{
						a_FillColor = CollectBits(pos, bits, a_iPalen);
					}
				}
				out.push_back(a_FillColor % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//---------------------------------------------------------------------
	static bool DecodeBasic(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data& a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		const size_t numPixels = a_iWidth * a_iHeight;
		out.reserve(numPixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < numPixels; i++)
			{
				out.push_back(a_FillColor % 256);
			}
		}
		else
		{
			core::Data bits = CreateBitstream(a_BMAPData);

			out.push_back(a_FillColor % 256);

			const size_t num_pixels = a_iWidth * a_iHeight;
			out.reserve(num_pixels);

			int sub = 1;
			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						if (bits[pos++] == 1)
						{
							sub = -sub;
						}
						a_FillColor -= sub;
					}
					else
					{
						a_FillColor = CollectBits(pos, bits, a_iPalen);
						sub = 1;
					}
				}
				out.push_back(a_FillColor % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//---------------------------------------------------------------------
	static bool DecodeRaw(const core::Data& a_BMAPData, core::Data& a_OutData)
	{
		a_OutData = a_BMAPData;
		return true;
	}

	//---------------------------------------------------------------------
	// ImageResource
	//---------------------------------------------------------------------
	ImageResource::ImageResource() : Resource()
	{}

	//---------------------------------------------------------------------
	ImageResource::~ImageResource()
	{
		if (m_pSRV)
		{
			m_pSRV->Release();
			m_pSRV = nullptr;
		}
	}

	//---------------------------------------------------------------------
	core::Data ImageResource::GetData() const
	{
		return m_pDataChunk->GetData();
	}

	//---------------------------------------------------------------------
	void ImageResource::SetDataChunk(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	const core::Data& ImageResource::GetImageData() const
	{
		return m_ImageData;
	}

	//---------------------------------------------------------------------
	ID3D11ShaderResourceView* ImageResource::GetSRV()
	{
		if (m_pSRV || m_ImageData.empty())
		{
			return m_pSRV;
		}

		// Validate that ImageData is RGBA (4 bytes per pixel). If it's still
		// 1 byte per pixel (palette indices), CreateTexture2D would read
		// past the allocation (width*4 pitch vs width*1 buffer) and AV in
		// atidxx64.dll - the crash reported at DX11System.cpp:126.
		const size_t expected = static_cast<size_t>(m_iWidth) * static_cast<size_t>(m_iHeight) * 4;
		if (m_ImageData.size() < expected)
		{
			return nullptr;
		}

		m_pSRV = dx11::GetDX11System().CreateTexture(
			m_ImageData.data(),
			m_iWidth,
			m_iHeight
		);

		return m_pSRV;
	}

	//---------------------------------------------------------------------
	std::string ImageResource::GetDimensions() const
	{
		if (GetWidth() == 0 || GetHeight() == 0)
		{
			return "Unknown";
		}
		return std::to_string(GetWidth()) + "x" + std::to_string(GetHeight());
	}

	//---------------------------------------------------------------------
	std::string ImageResource::GetSize() const
	{
		if (!m_pDataChunk)
		{
			return core::SizeToString(m_ImageData.size());
		}
		return core::SizeToString(m_ImageData.empty() ? m_pDataChunk->ChunkSize() : m_ImageData.size());
	}

	//---------------------------------------------------------------------
	uint16_t ImageResource::GetWidth() const
	{
		return m_iWidth;
	}

	//---------------------------------------------------------------------
	uint16_t ImageResource::GetHeight() const
	{
		return m_iHeight;
	}

	//---------------------------------------------------------------------
	const std::vector<Color>& ImageResource::GetColors() const
	{
		return m_aColors;
	}

	//---------------------------------------------------------------------
	// RoomBackgroundResource
	//---------------------------------------------------------------------
	RoomBackgroundResource::RoomBackgroundResource() : ImageResource()
	{
		m_eResourceType = resources::ResourceType::RoomBackground;
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

		parsing::TRNS_Chunk* trnsData = trns->GetData().dataAs<parsing::TRNS_Chunk>();
		if (!trnsData)
		{
			return;
		}

		size_t bmapSize = 0;
		if (bmap->ChunkSize() >= sizeof(parsing::BMAP_Chunk))
		{
			bmapSize = bmap->ChunkSize() - sizeof(parsing::BMAP_Chunk);
		}
		if (bmapSize > 0)
		{
			core::Data bmapImageData = core::Data(core::add(bmap->GetData().data(), sizeof(parsing::BMAP_Chunk)), bmapSize);
			if (!DecodeHE(bmapData->fillColor, bmapImageData, m_iWidth, m_iHeight, palen, m_ImageData, he_transparent))
			{
				return;
			}
		}
		else
		{
			// No BMAP data -> no room background (valid case). Leave m_ImageData empty
			// so GetSRV() returns nullptr instead of trying to create a 0-byte texture.
			m_ImageData = core::Data();
		}

		if (!m_ImageData.empty())
		{
			std::vector<uint8_t> newOut;
			newOut.reserve(m_ImageData.size() * 4);
			for (size_t i = 0; i < m_ImageData.size(); i++)
			{
				uint8_t idx = m_ImageData.dataAs<unsigned char>()[i];
				newOut.push_back(apalData->data[idx * 3]);
				newOut.push_back(apalData->data[idx * 3 + 1]);
				newOut.push_back(apalData->data[idx * 3 + 2]);
				if (m_ImageData[i] == bmapData->fillColor && he_transparent)
				{
					newOut.push_back(0);
				}
				else
				{
					newOut.push_back(255);
				}
			}

			m_ImageData = core::Data(newOut.data(), newOut.size());
		}

		for (size_t i = 0; i < 256; i++)
		{
			m_aColors.push_back({
				apalData->data[i * 3],
				apalData->data[i * 3 + 1],
				apalData->data[i * 3 + 2]
			});
		}
	}

	//---------------------------------------------------------------------
	// RoomImageResource
	//---------------------------------------------------------------------
	RoomImageResource::RoomImageResource() : ImageResource()
	{
		m_eResourceType = resources::ResourceType::RoomImage;
	}

	constexpr uint32_t StripWidth = 8;
	//---------------------------------------------------------------------
	void RoomImageResource::Open()
	{
		m_aColors.clear();

		// SMAP is the data chunk.
		parsing::Chunk* smap = m_pDataChunk->TryFindChild(parsing::SMAP_CHUNK_ID);
		if (!smap)
		{
			return;
		}

		parsing::Chunk* obim = m_pDataChunk->TryFindParent(parsing::OBIM_CHUNK_ID);
		if (!obim)
		{
			return;
		}

		parsing::Chunk* imhd = obim->TryFindChild(parsing::IMHD_CHUNK_ID);
		if (!imhd)
		{
			return;
		}

		parsing::IMHD_Chunk* imhdData = imhd->GetData().dataAs<parsing::IMHD_Chunk>();
		if (!imhdData)
		{
			return;
		}

		parsing::Chunk* rmda = obim->GetParent();
		if (!rmda)
		{
			return;
		}

		if (rmda->GetTag() != parsing::RMDA_CHUNK_ID)
		{
			return;
		}

		parsing::Chunk* apal = rmda->TryFindChild(parsing::APAL_CHUNK_ID);
		if (!apal)
		{
			return;
		}

		parsing::APAL_Chunk* apalData = apal->GetData().dataAs<parsing::APAL_Chunk>();
		if (!apalData)
		{
			return;
		}

		parsing::Chunk* trns = rmda->TryFindChild(parsing::TRNS_CHUNK_ID);
		if (!trns)
		{
			return;
		}

		parsing::TRNS_Chunk* trnsData = trns->GetData().dataAs<parsing::TRNS_Chunk>();
		if (!trnsData)
		{
			return;
		}

		const size_t num_strips = static_cast<size_t>(static_cast<size_t>(floor(static_cast<double>(imhdData->width / StripWidth))));

		std::vector<uint32_t> offsets;
		int j = 0;
		for (size_t i = 0; i < num_strips; i++, j += sizeof(uint32_t))
		{
			uint32_t number = *reinterpret_cast<uint32_t*>(core::add(smap->GetData().data(), j));
			offsets.push_back(number);
		}

		struct offset_pair
		{
			size_t start, end;
		};

		std::vector<offset_pair> index;
		for (size_t i = 0; i < offsets.size(); i++)
		{
			index.push_back({ offsets[i], (i + 1) == offsets.size() ? smap->GetData().size() : offsets[i + 1]});
		}

		struct strip
		{
			core::Data data;
		};

		std::vector<strip> strips;
		for (size_t i = 0; i < num_strips; i++)
		{
			strip str;
			str.data = core::Data(core::add(smap->GetData().data(), index[i].start), index[i].end - index[i].start);
			strips.push_back(str);
		}

		struct IndexColor
		{
			uint8_t index = 0;
			uint8_t trans_color = 0;

			IndexColor(uint8_t index, uint8_t trans_color)
			{
				this->index = index;
				this->trans_color = trans_color;
			}

			IndexColor()
			{
			}
		};

		size_t total_size = 0;
		std::vector< std::vector<std::vector<IndexColor>>> data_blocks;

		m_ImageData = core::Data(imhdData->width * imhdData->height);
		for (auto& strip : strips)
		{
			std::vector<std::vector<IndexColor>> data_new_block;

			const uint8_t code = strip.data[0];

			bool horizontal = true;
			if (code >= 0x03 && code <= 0x12 || code >= 0x22 && code <= 0x26)
			{
				horizontal = false;
			}

			const bool he_transparent = code >= 0x22 && code <= 0x30 || code >= 0x54 && code <= 0x80 || code >= 0x8F;

			const int palen = code % 10;

			const uint8_t color = strip.data[1];

			if (code >= 0x40 && code <= 0x80)
			{
				if (!DecodeMajmin(color, strip.data, StripWidth, imhdData->height, palen, m_ImageData, he_transparent))
				{
					return;
				}
			}
			else if (code >= 0x0E && code <= 0x30)
			{
				if (!DecodeBasic(color, strip.data, StripWidth, imhdData->height, palen, m_ImageData, he_transparent))
				{
					return;
				}
			}
			else if (code >= 0x86 && code <= 0x94)
			{
				if (!DecodeHE(color, strip.data, StripWidth, imhdData->height, palen, m_ImageData, he_transparent))
				{
					return;
				}
			}
			else if (code >= 0x01 && code <= 0x95)
			{
				if (!DecodeRaw(strip.data, m_ImageData))
				{
					return;
				}
			}
		}
	}

	//---------------------------------------------------------------------
	void Resource::SetChunk(parsing::Chunk* a_pChunk)
	{
		m_pChunk = a_pChunk;
	}
}
