#include "SVGParser.h"

#define NANOSVG_IMPLEMENTATION
#include <nanosvg/nanosvg.h>
#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvg/nanosvgrast.h>

namespace humongousexplorer::file
{
	//---------------------------------------------------------------------
	bool SVGParser::Load(const std::string& a_sPath, core::Data& a_pPixels, int& a_iWidth, int& a_iHeight, float a_fScale)
	{
		NSVGimage* svgImage = nsvgParseFromFile(a_sPath.c_str(), "px", 96.0f);
		if (!svgImage)
		{
			return false;
		}

		a_iWidth = static_cast<int>(svgImage->width);
		a_iHeight = static_cast<int>(svgImage->height);

		NSVGrasterizer* rasterizer = nsvgCreateRasterizer();
		if (!rasterizer)
		{
			nsvgDelete(svgImage);
			return false;
		}

		size_t pixelSize = static_cast<size_t>(a_iWidth) * a_iHeight * 4;
		a_pPixels = core::Data(pixelSize);

		nsvgRasterize(rasterizer, svgImage, 0, 0, a_fScale,
			a_pPixels.dataAs<unsigned char>(), a_iWidth, a_iHeight, a_iWidth * 4);

		nsvgDeleteRasterizer(rasterizer);
		nsvgDelete(svgImage);

		return true;
	}
}
