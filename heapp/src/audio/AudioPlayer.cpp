#include "AudioPlayer.h"

#include <algorithm>
#include <cstring>
#include <limits>

#undef min
#undef max

namespace humongousexplorer::audio
{
	AudioPlayer::AudioPlayer()
	{
	}

	AudioPlayer::~AudioPlayer()
	{
		Close();
	}

	bool AudioPlayer::Open(
		uint32_t sampleRate,
		uint8_t bitsPerSample,
		uint8_t channels)
	{
		Close();

		if (sampleRate == 0 ||
			bitsPerSample == 0 ||
			channels == 0)
		{
			return false;
		}

		if ((bitsPerSample % 8) != 0)
		{
			return false;
		}

		const uint32_t blockAlign =
			static_cast<uint32_t>(channels) *
			static_cast<uint32_t>(bitsPerSample) / 8;

		if (blockAlign == 0)
		{
			return false;
		}

		const uint64_t avgBytesPerSecond =
			static_cast<uint64_t>(sampleRate) *
			static_cast<uint64_t>(blockAlign);

		if (avgBytesPerSecond > std::numeric_limits<DWORD>::max())
		{
			return false;
		}

		ZeroMemory(&m_Format, sizeof(m_Format));

		m_Format.wFormatTag = WAVE_FORMAT_PCM;
		m_Format.nChannels = channels;
		m_Format.nSamplesPerSec = sampleRate;
		m_Format.wBitsPerSample = bitsPerSample;
		m_Format.nBlockAlign = static_cast<WORD>(blockAlign);
		m_Format.nAvgBytesPerSec =
			static_cast<DWORD>(avgBytesPerSecond);

		MMRESULT result = waveOutOpen(
			&m_hWaveOut,
			WAVE_MAPPER,
			&m_Format,
			0,
			0,
			CALLBACK_NULL);

		if (result != MMSYSERR_NOERROR)
		{
			m_hWaveOut = nullptr;
			return false;
		}

		m_bOpen = true;
		m_bPlaying = false;
		m_bPaused = false;
		m_iPosition = 0;

		return true;
	}

	void AudioPlayer::Close()
	{
		Stop();

		if (m_hWaveOut)
		{
			waveOutClose(m_hWaveOut);
			m_hWaveOut = nullptr;
		}

		m_aBuffer.clear();

		ZeroMemory(&m_Format, sizeof(m_Format));
		ZeroMemory(&m_WaveHeader, sizeof(m_WaveHeader));

		m_bOpen = false;
		m_bPlaying = false;
		m_bPaused = false;
		m_bHeaderPrepared = false;
		m_iPosition = 0;
	}

	bool AudioPlayer::IsOpen() const
	{
		return m_bOpen && m_hWaveOut != nullptr;
	}

	bool AudioPlayer::Play(const void* data, size_t size)
	{
		if (!IsOpen())
		{
			return false;
		}

		if (data == nullptr || size == 0)
		{
			return false;
		}

		if (size > std::numeric_limits<DWORD>::max())
		{
			return false;
		}

		Stop();

		m_aBuffer.resize(size);

		std::memcpy(
			m_aBuffer.data(),
			data,
			size);

		m_iPosition = 0;

		return StartFromPosition(0);
	}

	bool AudioPlayer::StartFromPosition(size_t position)
	{
		if (!IsOpen())
		{
			return false;
		}

		if (m_aBuffer.empty())
		{
			return false;
		}

		if (position >= m_aBuffer.size())
		{
			m_iPosition = m_aBuffer.size();
			m_bPlaying = false;
			m_bPaused = false;
			return false;
		}

		const size_t remaining =
			m_aBuffer.size() - position;

		if (remaining > std::numeric_limits<DWORD>::max())
		{
			return false;
		}

		ZeroMemory(
			&m_WaveHeader,
			sizeof(m_WaveHeader));

		m_WaveHeader.lpData =
			reinterpret_cast<LPSTR>(
				m_aBuffer.data() + position);

		m_WaveHeader.dwBufferLength =
			static_cast<DWORD>(remaining);

		MMRESULT result = waveOutPrepareHeader(
			m_hWaveOut,
			&m_WaveHeader,
			sizeof(m_WaveHeader));

		if (result != MMSYSERR_NOERROR)
		{
			ZeroMemory(
				&m_WaveHeader,
				sizeof(m_WaveHeader));

			return false;
		}

		m_bHeaderPrepared = true;

		result = waveOutWrite(
			m_hWaveOut,
			&m_WaveHeader,
			sizeof(m_WaveHeader));

		if (result != MMSYSERR_NOERROR)
		{
			Unprepare();

			return false;
		}

		m_iPosition = position;
		m_bPlaying = true;
		m_bPaused = false;

		return true;
	}

	void AudioPlayer::Pause()
	{
		if (!IsOpen())
		{
			return;
		}

		if (!m_bPlaying || m_bPaused)
		{
			return;
		}

		m_iPosition = GetBytePosition();

		MMRESULT result =
			waveOutPause(m_hWaveOut);

		if (result == MMSYSERR_NOERROR)
		{
			m_bPaused = true;
		}
	}

	void AudioPlayer::Resume()
	{
		if (!IsOpen())
		{
			return;
		}

		if (!m_bPlaying || !m_bPaused)
		{
			return;
		}

		MMRESULT result =
			waveOutRestart(m_hWaveOut);

		if (result == MMSYSERR_NOERROR)
		{
			m_bPaused = false;
		}
	}

	void AudioPlayer::Stop()
	{
		if (!m_hWaveOut)
		{
			m_bPlaying = false;
			m_bPaused = false;
			m_bHeaderPrepared = false;
			return;
		}

		if (m_bPlaying || m_bHeaderPrepared)
		{
			waveOutReset(m_hWaveOut);

			Unprepare();
		}

		ZeroMemory(
			&m_WaveHeader,
			sizeof(m_WaveHeader));

		m_bPlaying = false;
		m_bPaused = false;
		m_bHeaderPrepared = false;
		m_iPosition = 0;
	}

	void AudioPlayer::Unprepare()
	{
		if (!m_hWaveOut || !m_bHeaderPrepared)
		{
			return;
		}

		waveOutUnprepareHeader(
			m_hWaveOut,
			&m_WaveHeader,
			sizeof(m_WaveHeader));

		m_bHeaderPrepared = false;
	}

	bool AudioPlayer::IsPlaying() const
	{
		if (!m_bPlaying)
		{
			return false;
		}

		if (m_bPaused)
		{
			return false;
		}

		if (!m_bHeaderPrepared)
		{
			return false;
		}

		if (m_WaveHeader.dwFlags & WHDR_DONE)
		{
			return false;
		}

		return true;
	}

	bool AudioPlayer::IsPaused() const
	{
		return m_bPlaying && m_bPaused;
	}

	size_t AudioPlayer::GetDevicePosition() const
	{
		if (!IsOpen())
		{
			return 0;
		}

		if (!m_bPlaying)
		{
			return 0;
		}

		MMTIME time{};

		time.wType = TIME_BYTES;

		MMRESULT result = waveOutGetPosition(
			m_hWaveOut,
			&time,
			sizeof(time));

		if (result != MMSYSERR_NOERROR)
		{
			return m_iPosition;
		}

		if (time.wType != TIME_BYTES)
		{
			return m_iPosition;
		}

		return static_cast<size_t>(time.u.cb);
	}

	size_t AudioPlayer::GetBytePosition() const
	{
		if (m_aBuffer.empty())
		{
			return 0;
		}

		if (!m_bPlaying)
		{
			return m_iPosition;
		}

		size_t devicePosition =
			GetDevicePosition();

		if (m_WaveHeader.lpData == nullptr)
		{
			return m_iPosition;
		}

		const auto* bufferStart =
			m_aBuffer.data();

		const auto* currentStart =
			reinterpret_cast<const uint8_t*>(
				m_WaveHeader.lpData);

		if (currentStart < bufferStart ||
			currentStart >= bufferStart + m_aBuffer.size())
		{
			return m_iPosition;
		}

		const size_t bufferOffset =
			static_cast<size_t>(
				currentStart - bufferStart);

		size_t position =
			bufferOffset + devicePosition;

		if (position > m_aBuffer.size())
		{
			position = m_aBuffer.size();
		}

		return position;
	}

	void AudioPlayer::Seek(size_t position)
	{
		if (!IsOpen())
		{
			return;
		}

		if (m_aBuffer.empty())
		{
			m_iPosition = 0;
			return;
		}

		position =
			std::min(position, m_aBuffer.size());

		const bool wasPlaying = IsPlaying();
		const bool wasPaused = IsPaused();

		if (m_bPlaying || m_bHeaderPrepared)
		{
			waveOutReset(m_hWaveOut);
			Unprepare();

			ZeroMemory(
				&m_WaveHeader,
				sizeof(m_WaveHeader));
		}

		m_bPlaying = false;
		m_bPaused = false;

		m_iPosition = position;

		if (position >= m_aBuffer.size())
		{
			return;
		}

		if (wasPlaying)
		{
			StartFromPosition(position);
			return;
		}

		if (wasPaused)
		{
			if (StartFromPosition(position))
			{
				waveOutPause(m_hWaveOut);
				m_bPaused = true;
			}
		}
	}

	size_t AudioPlayer::GetTotalBytes() const
	{
		return m_aBuffer.size();
	}
}