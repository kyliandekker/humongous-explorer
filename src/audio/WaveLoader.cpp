#include "audio/WaveLoader.h"

#include <cstring>

#include "core/Data.h"
#include "file/file_abstractions.h"

namespace humongousexplorer::audio
{
#pragma pack(push, 1)
	struct FMTChunk
	{
		uint16_t audioFormat;
		uint16_t numChannels;
		uint32_t sampleRate;
		uint32_t byteRate;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
	};
#pragma pack(pop)

	//---------------------------------------------------------------------
	static uint16_t ReadU16(const uint8_t* a_pData)
	{
		return static_cast<uint16_t>(a_pData[0]) | (static_cast<uint16_t>(a_pData[1]) << 8);
	}

	//---------------------------------------------------------------------
	static uint32_t ReadU32(const uint8_t* a_pData)
	{
		return static_cast<uint32_t>(a_pData[0])
			| (static_cast<uint32_t>(a_pData[1]) << 8)
			| (static_cast<uint32_t>(a_pData[2]) << 16)
			| (static_cast<uint32_t>(a_pData[3]) << 24);
	}

	//---------------------------------------------------------------------
	bool WaveLoader::Load(const std::string& a_sFilePath, core::Data& a_Data, uint16_t& a_iSampleRate)
	{
		core::Data file;
		if (!file::LoadFile(a_sFilePath, file))
		{
			return false;
		}

		const uint8_t* p = static_cast<const uint8_t*>(file.data());
		const uint8_t* pEnd = p + file.size();

		// RIFF header: "RIFF" + uint32 size + "WAVE"
		if (p + 12 > pEnd)
		{
			return false;
		}

		if (std::memcmp(p, "RIFF", 4) != 0 || std::memcmp(p + 8, "WAVE", 4) != 0)
		{
			return false;
		}

		p += 12;

		// Search for "fmt " and "data" chunks
		FMTChunk fmtChunk = {};
		bool fmtFound = false;
		const uint8_t* pData = nullptr;
		uint32_t dataSize = 0;

		while (p + 8 <= pEnd)
		{
			const char* chunkId = reinterpret_cast<const char*>(p);
			uint32_t chunkSize = ReadU32(p + 4);
			p += 8;

			if (p + chunkSize > pEnd)
			{
				break;
			}

			if (std::memcmp(chunkId, "fmt ", 4) == 0 && chunkSize >= sizeof(FMTChunk))
			{
				std::memcpy(&fmtChunk, p, sizeof(FMTChunk));
				fmtFound = true;
			}
			else if (std::memcmp(chunkId, "data", 4) == 0)
			{
				pData = p;
				dataSize = chunkSize;
			}

			p += chunkSize;

			// WAV chunks are word-aligned
			if (chunkSize & 1)
			{
				p++;
			}
		}

		if (!fmtFound || !pData)
		{
			return false;
		}

		a_iSampleRate = fmtChunk.sampleRate;

		// Already mono 8-bit, just copy the raw data.
		if (fmtChunk.numChannels == 1 && fmtChunk.bitsPerSample == 8)
		{
			a_Data = core::Data(pData, dataSize);
			return true;
		}

		if (fmtChunk.bitsPerSample == 16)
		{
			size_t sampleCount = dataSize / (fmtChunk.numChannels * 2);
			a_Data = core::Data(sampleCount);
			uint8_t* out = static_cast<uint8_t*>(a_Data.data());

			for (size_t i = 0; i < sampleCount; ++i)
			{
				int32_t sum = 0;
				for (uint16_t ch = 0; ch < fmtChunk.numChannels; ++ch)
				{
					size_t offset = (i * fmtChunk.numChannels + ch) * 2;
					int16_t sample = static_cast<int16_t>(ReadU16(pData + offset));
					sum += sample;
				}
				int16_t mono = static_cast<int16_t>(sum / fmtChunk.numChannels);
				out[i] = static_cast<uint8_t>((mono >> 8) + 128);
			}
			return true;
		}
		else if (fmtChunk.bitsPerSample == 8)
		{
			size_t sampleCount = dataSize / fmtChunk.numChannels;
			a_Data = core::Data(sampleCount);
			uint8_t* out = static_cast<uint8_t*>(a_Data.data());

			for (size_t i = 0; i < sampleCount; ++i)
			{
				int32_t sum = 0;
				for (uint16_t ch = 0; ch < fmtChunk.numChannels; ++ch)
				{
					sum += static_cast<int>(pData[i * fmtChunk.numChannels + ch]) - 128;
				}
				int mono = sum / fmtChunk.numChannels;
				out[i] = static_cast<uint8_t>(mono + 128);
			}
			return true;
		}

		return false;
	}

	//---------------------------------------------------------------------
	bool WaveLoader::Save(const std::string& a_sFilePath, const core::Data& a_Data, uint16_t a_iSampleRate)
	{
		uint32_t dataSize = static_cast<uint32_t>(a_Data.size());
		uint32_t fileSize = 44 + dataSize;

		core::Data file(fileSize);
		uint8_t* p = static_cast<uint8_t*>(file.data());

		// RIFF header
		std::memcpy(p, "RIFF", 4); p += 4;
		uint32_t riffSize = fileSize - 8;
		std::memcpy(p, &riffSize, 4); p += 4;
		std::memcpy(p, "WAVE", 4); p += 4;

		// fmt chunk
		std::memcpy(p, "fmt ", 4); p += 4;
		uint32_t fmtSize = 16;
		std::memcpy(p, &fmtSize, 4); p += 4;
		uint16_t audioFormat = 1;
		std::memcpy(p, &audioFormat, 2); p += 2;
		uint16_t numChannels = 1;
		std::memcpy(p, &numChannels, 2); p += 2;
		std::memcpy(p, &a_iSampleRate, 4); p += 4;
		uint32_t byteRate = a_iSampleRate * numChannels;
		std::memcpy(p, &byteRate, 4); p += 4;
		uint16_t blockAlign = numChannels;
		std::memcpy(p, &blockAlign, 2); p += 2;
		uint16_t bitsPerSample = 8;
		std::memcpy(p, &bitsPerSample, 2); p += 2;

		// data chunk
		std::memcpy(p, "data", 4); p += 4;
		std::memcpy(p, &dataSize, 4); p += 4;
		std::memcpy(p, a_Data.data(), dataSize);

		return file::SaveFile(a_sFilePath, file);
	}
}
