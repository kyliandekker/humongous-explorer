#include "audio/WaveLoader.h"

#include <cstring>
#include <limits>

#include "core/Data.h"
#include "file/file.h"

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
		return static_cast<uint16_t>(a_pData[0])
			| (static_cast<uint16_t>(a_pData[1]) << 8);
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
	static void WriteU16(uint8_t* a_pData, uint16_t a_Value)
	{
		a_pData[0] = static_cast<uint8_t>(a_Value);
		a_pData[1] = static_cast<uint8_t>(a_Value >> 8);
	}

	//---------------------------------------------------------------------
	static void WriteU32(uint8_t* a_pData, uint32_t a_Value)
	{
		a_pData[0] = static_cast<uint8_t>(a_Value);
		a_pData[1] = static_cast<uint8_t>(a_Value >> 8);
		a_pData[2] = static_cast<uint8_t>(a_Value >> 16);
		a_pData[3] = static_cast<uint8_t>(a_Value >> 24);
	}

	//---------------------------------------------------------------------
	bool WaveLoader::Load(
		const std::string& a_sFilePath,
		core::Data& a_Data,
		uint16_t& a_iSampleRate)
	{
		core::Data file;

		if (!file::LoadFile(a_sFilePath, file))
		{
			return false;
		}

		const uint8_t* p = static_cast<const uint8_t*>(file.data());
		const uint8_t* pEnd = p + file.size();

		// RIFF header:
		// "RIFF" + uint32 size + "WAVE"
		if (file.size() < 12)
		{
			return false;
		}

		if (std::memcmp(p, "RIFF", 4) != 0 ||
			std::memcmp(p + 8, "WAVE", 4) != 0)
		{
			return false;
		}

		p += 12;

		// Search for "fmt " and "data" chunks.
		FMTChunk fmtChunk = {};
		bool fmtFound = false;

		const uint8_t* pData = nullptr;
		uint32_t dataSize = 0;

		while (pEnd - p >= 8)
		{
			const char* chunkId =
				reinterpret_cast<const char*>(p);

			const uint32_t chunkSize = ReadU32(p + 4);

			p += 8;

			if (chunkSize > static_cast<uint32_t>(pEnd - p))
			{
				return false;
			}

			if (std::memcmp(chunkId, "fmt ", 4) == 0 &&
				chunkSize >= sizeof(FMTChunk))
			{
				fmtChunk.audioFormat = ReadU16(p);
				fmtChunk.numChannels = ReadU16(p + 2);
				fmtChunk.sampleRate = ReadU32(p + 4);
				fmtChunk.byteRate = ReadU32(p + 8);
				fmtChunk.blockAlign = ReadU16(p + 12);
				fmtChunk.bitsPerSample = ReadU16(p + 14);

				fmtFound = true;
			}
			else if (std::memcmp(chunkId, "data", 4) == 0)
			{
				pData = p;
				dataSize = chunkSize;
			}

			p += chunkSize;

			// WAV chunks are word-aligned.
			if (chunkSize & 1)
			{
				if (p >= pEnd)
				{
					return false;
				}

				++p;
			}
		}

		if (!fmtFound || !pData)
		{
			return false;
		}

		// Humongous Entertainment uses uint16_t sample rates.
		if (fmtChunk.sampleRate > std::numeric_limits<uint16_t>::max())
		{
			return false;
		}

		if (fmtChunk.numChannels == 0)
		{
			return false;
		}

		a_iSampleRate =
			static_cast<uint16_t>(fmtChunk.sampleRate);

		// Already mono 8-bit.
		if (fmtChunk.numChannels == 1 &&
			fmtChunk.bitsPerSample == 8)
		{
			a_Data = core::Data(pData, dataSize);
			return true;
		}

		// Convert 16-bit PCM to mono 8-bit PCM.
		if (fmtChunk.bitsPerSample == 16)
		{
			const size_t bytesPerSample =
				static_cast<size_t>(fmtChunk.numChannels) * 2;

			const size_t sampleCount =
				dataSize / bytesPerSample;

			a_Data = core::Data(sampleCount);

			uint8_t* out =
				static_cast<uint8_t*>(a_Data.data());

			for (size_t i = 0; i < sampleCount; ++i)
			{
				int32_t sum = 0;

				for (uint16_t channel = 0;
					channel < fmtChunk.numChannels;
					++channel)
				{
					const size_t offset =
						(i * fmtChunk.numChannels + channel) * 2;

					const uint16_t rawSample =
						ReadU16(pData + offset);

					const int16_t sample =
						static_cast<int16_t>(rawSample);

					sum += sample;
				}

				const int32_t mono =
					sum / fmtChunk.numChannels;

				// Convert signed 16-bit [-32768, 32767]
				// to unsigned 8-bit [0, 255].
				out[i] = static_cast<uint8_t>(
					(mono >> 8) + 128);
			}

			return true;
		}

		// Convert multi-channel 8-bit PCM to mono 8-bit PCM.
		if (fmtChunk.bitsPerSample == 8)
		{
			const size_t sampleCount =
				dataSize / fmtChunk.numChannels;

			a_Data = core::Data(sampleCount);

			uint8_t* out =
				static_cast<uint8_t*>(a_Data.data());

			for (size_t i = 0; i < sampleCount; ++i)
			{
				int32_t sum = 0;

				for (uint16_t channel = 0;
					channel < fmtChunk.numChannels;
					++channel)
				{
					sum +=
						static_cast<int>(
							pData[
								i * fmtChunk.numChannels +
									channel
							]
							) - 128;
				}

				const int32_t mono =
					sum / fmtChunk.numChannels;

				out[i] =
					static_cast<uint8_t>(mono + 128);
			}

			return true;
		}

		return false;
	}

	//---------------------------------------------------------------------
	bool WaveLoader::Save(
		const std::string& a_sFilePath,
		const core::Data& a_Data,
		uint16_t a_iSampleRate)
	{
		const uint32_t dataSize =
			static_cast<uint32_t>(a_Data.size());

		// RIFF:
		// 12 byte header
		// 24 byte fmt chunk
		// 8 byte data chunk header
		// data
		const uint32_t fileSize =
			44 + dataSize;

		core::Data file(fileSize);

		uint8_t* p =
			static_cast<uint8_t*>(file.data());

		// RIFF header
		std::memcpy(p, "RIFF", 4);
		p += 4;

		WriteU32(p, fileSize - 8);
		p += 4;

		std::memcpy(p, "WAVE", 4);
		p += 4;

		// fmt chunk
		std::memcpy(p, "fmt ", 4);
		p += 4;

		WriteU32(p, 16);
		p += 4;

		// PCM
		WriteU16(p, 1);
		p += 2;

		// Mono
		WriteU16(p, 1);
		p += 2;

		// WAV stores sample rate as uint32_t.
		// HE provides it as uint16_t.
		const uint32_t sampleRate =
			static_cast<uint32_t>(a_iSampleRate);

		WriteU32(p, sampleRate);
		p += 4;

		// 8-bit mono PCM.
		const uint32_t byteRate =
			sampleRate;

		WriteU32(p, byteRate);
		p += 4;

		// One byte per sample.
		WriteU16(p, 1);
		p += 2;

		// 8-bit samples.
		WriteU16(p, 8);
		p += 2;

		// data chunk
		std::memcpy(p, "data", 4);
		p += 4;

		WriteU32(p, dataSize);
		p += 4;

		if (dataSize > 0)
		{
			std::memcpy(
				p,
				a_Data.data(),
				dataSize
			);
		}

		return file::SaveFile(a_sFilePath, file);
	}
}