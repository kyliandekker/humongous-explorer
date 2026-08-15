#include "./AudioPlayer.h"

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

        m_Open = true;
        m_Playing = false;
        m_Paused = false;
        m_Position = 0;

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

        m_Buffer.clear();

        ZeroMemory(&m_Format, sizeof(m_Format));
        ZeroMemory(&m_WaveHeader, sizeof(m_WaveHeader));

        m_Open = false;
        m_Playing = false;
        m_Paused = false;
        m_HeaderPrepared = false;
        m_Position = 0;
    }

    bool AudioPlayer::IsOpen() const
    {
        return m_Open && m_hWaveOut != nullptr;
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

        m_Buffer.resize(size);

        std::memcpy(
            m_Buffer.data(),
            data,
            size);

        m_Position = 0;

        return StartFromPosition(0);
    }

    bool AudioPlayer::StartFromPosition(size_t position)
    {
        if (!IsOpen())
        {
            return false;
        }

        if (m_Buffer.empty())
        {
            return false;
        }

        if (position >= m_Buffer.size())
        {
            m_Position = m_Buffer.size();
            m_Playing = false;
            m_Paused = false;
            return false;
        }

        const size_t remaining =
            m_Buffer.size() - position;

        if (remaining > std::numeric_limits<DWORD>::max())
        {
            return false;
        }

        ZeroMemory(
            &m_WaveHeader,
            sizeof(m_WaveHeader));

        m_WaveHeader.lpData =
            reinterpret_cast<LPSTR>(
                m_Buffer.data() + position);

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

        m_HeaderPrepared = true;

        result = waveOutWrite(
            m_hWaveOut,
            &m_WaveHeader,
            sizeof(m_WaveHeader));

        if (result != MMSYSERR_NOERROR)
        {
            Unprepare();

            return false;
        }

        m_Position = position;
        m_Playing = true;
        m_Paused = false;

        return true;
    }

    void AudioPlayer::Pause()
    {
        if (!IsOpen())
        {
            return;
        }

        if (!m_Playing || m_Paused)
        {
            return;
        }

        m_Position = GetBytePosition();

        MMRESULT result =
            waveOutPause(m_hWaveOut);

        if (result == MMSYSERR_NOERROR)
        {
            m_Paused = true;
        }
    }

    void AudioPlayer::Resume()
    {
        if (!IsOpen())
        {
            return;
        }

        if (!m_Playing || !m_Paused)
        {
            return;
        }

        MMRESULT result =
            waveOutRestart(m_hWaveOut);

        if (result == MMSYSERR_NOERROR)
        {
            m_Paused = false;
        }
    }

    void AudioPlayer::Stop()
    {
        if (!m_hWaveOut)
        {
            m_Playing = false;
            m_Paused = false;
            m_HeaderPrepared = false;
            return;
        }

        if (m_Playing || m_HeaderPrepared)
        {
            waveOutReset(m_hWaveOut);

            Unprepare();
        }

        ZeroMemory(
            &m_WaveHeader,
            sizeof(m_WaveHeader));

        m_Playing = false;
        m_Paused = false;
        m_HeaderPrepared = false;
        m_Position = 0;
    }

    void AudioPlayer::Unprepare()
    {
        if (!m_hWaveOut || !m_HeaderPrepared)
        {
            return;
        }

        waveOutUnprepareHeader(
            m_hWaveOut,
            &m_WaveHeader,
            sizeof(m_WaveHeader));

        m_HeaderPrepared = false;
    }

    bool AudioPlayer::IsPlaying() const
    {
        if (!m_Playing)
        {
            return false;
        }

        if (m_Paused)
        {
            return false;
        }

        if (!m_HeaderPrepared)
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
        return m_Playing && m_Paused;
    }

    size_t AudioPlayer::GetDevicePosition() const
    {
        if (!IsOpen())
        {
            return 0;
        }

        if (!m_Playing)
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
            return m_Position;
        }

        if (time.wType != TIME_BYTES)
        {
            return m_Position;
        }

        return static_cast<size_t>(time.u.cb);
    }

    size_t AudioPlayer::GetBytePosition() const
    {
        if (m_Buffer.empty())
        {
            return 0;
        }

        if (!m_Playing)
        {
            return m_Position;
        }

        size_t devicePosition =
            GetDevicePosition();

        if (m_WaveHeader.lpData == nullptr)
        {
            return m_Position;
        }

        const auto* bufferStart =
            m_Buffer.data();

        const auto* currentStart =
            reinterpret_cast<const uint8_t*>(
                m_WaveHeader.lpData);

        if (currentStart < bufferStart ||
            currentStart >= bufferStart + m_Buffer.size())
        {
            return m_Position;
        }

        const size_t bufferOffset =
            static_cast<size_t>(
                currentStart - bufferStart);

        size_t position =
            bufferOffset + devicePosition;

        if (position > m_Buffer.size())
        {
            position = m_Buffer.size();
        }

        return position;
    }

    void AudioPlayer::Seek(size_t position)
    {
        if (!IsOpen())
        {
            return;
        }

        if (m_Buffer.empty())
        {
            m_Position = 0;
            return;
        }

        position =
            std::min(position, m_Buffer.size());

        const bool wasPlaying = IsPlaying();
        const bool wasPaused = IsPaused();

        if (m_Playing || m_HeaderPrepared)
        {
            waveOutReset(m_hWaveOut);
            Unprepare();

            ZeroMemory(
                &m_WaveHeader,
                sizeof(m_WaveHeader));
        }

        m_Playing = false;
        m_Paused = false;

        m_Position = position;

        if (position >= m_Buffer.size())
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
                m_Paused = true;
            }
        }
    }

    size_t AudioPlayer::GetTotalBytes() const
    {
        return m_Buffer.size();
    }
}