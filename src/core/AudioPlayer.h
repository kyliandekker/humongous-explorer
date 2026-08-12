#pragma once

#include <Windows.h>
#include <mmsystem.h>

#include <cstdint>
#include <vector>

namespace humongousexplorer::core
{
    class AudioPlayer
    {
    public:
        AudioPlayer();
        ~AudioPlayer();

        AudioPlayer(const AudioPlayer&) = delete;
        AudioPlayer& operator=(const AudioPlayer&) = delete;

        bool Open(
            uint32_t sampleRate,
            uint8_t bitsPerSample,
            uint8_t channels);

        void Close();

        bool IsOpen() const;

        bool Play(const void* data, size_t size);
        void Pause();
        void Resume();
        void Stop();

        bool IsPlaying() const;
        bool IsPaused() const;

        void Seek(size_t position);

        size_t GetBytePosition() const;
        size_t GetTotalBytes() const;

    private:
        bool StartFromPosition(size_t position);

        size_t GetDevicePosition() const;

        void Unprepare();

    private:
        HWAVEOUT m_hWaveOut = nullptr;

        WAVEFORMATEX m_Format{};
        WAVEHDR m_WaveHeader{};

        std::vector<uint8_t> m_Buffer;

        size_t m_Position = 0;

        bool m_Open = false;
        bool m_Playing = false;
        bool m_Paused = false;
        bool m_HeaderPrepared = false;
    };
}