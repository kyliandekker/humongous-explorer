#pragma once

#include "helib/core/Data.h"

#include <cstdint>
#include <string>

namespace humongousexplorer::file
{
	//---------------------------------------------------------------------
	// SVGParser
	//---------------------------------------------------------------------
	/// <summary>
	/// Parses SVG files and rasterizes them to RGBA pixel buffers.
	/// </summary>
	class SVGParser
	{
	public:
		/// <summary>
		/// Loads and rasterizes an SVG file from disk.
		/// </summary>
		/// <param name="a_sPath">Path to the SVG file.</param>
		/// <param name="a_pPixels">Output pixel buffer (RGBA, 4 bytes per pixel).</param>
		/// <param name="a_iWidth">Output image width.</param>
		/// <param name="a_iHeight">Output image height.</param>
		/// <param name="a_fScale">Rasterization scale (1.0 = native size).</param>
		/// <returns>True if the SVG was loaded and rasterized successfully.</returns>
		static bool Load(const std::string& a_sPath, core::Data& a_pPixels, int& a_iWidth, int& a_iHeight, float a_fScale = 1.0f);
	};
}
