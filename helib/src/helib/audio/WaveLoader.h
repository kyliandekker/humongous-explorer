#pragma once

#include <cstdint>
#include <string>

#include <helib/file/FILEPCH.h>

namespace humongousexplorer::core
{
	class Data;
}
namespace humongousexplorer::audio
{
	//---------------------------------------------------------------------
	// WaveLoader
	//---------------------------------------------------------------------
	/// <summary>
	/// Loads and saves wave files. Always works with mono 8-bit audio.
	/// </summary>
	class WaveLoader
	{
	public:
		/// <summary>
		/// Loads a wave file, converts to mono 8-bit, and stores the result.
		/// </summary>
		/// <param name="a_FilePath">Path to the .wav file.</param>
		/// <param name="a_Data">Output buffer containing the converted mono 8-bit PCM data.</param>
		/// <param name="a_iSampleRate">Output: the sample rate of the loaded audio.</param>
		/// <returns>True if the wave file was loaded and converted successfully.</returns>
		static bool Load(const fs::path& a_FilePath, core::Data& a_Data, uint16_t& a_iSampleRate);

		/// <summary>
		/// Saves mono 8-bit PCM data to a wave file.
		/// </summary>
		/// <param name="a_FilePath">Path to the output .wav file.</param>
		/// <param name="a_Data">The mono 8-bit PCM data to save.</param>
		/// <param name="a_iSampleRate">The sample rate of the audio.</param>
		/// <returns>True if the wave file was saved successfully.</returns>
		static bool Save(const fs::path& a_FilePath, const core::Data& a_Data, uint16_t a_iSampleRate);
	};
}
