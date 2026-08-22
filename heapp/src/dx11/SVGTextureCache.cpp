#include "SVGTextureCache.h"

#include "core/Data.h"
#include "dx11/DX11System.h"
#include "ui/SVGParser.h"

namespace humongousexplorer::dx11
{
	std::unordered_map<std::string, SVGTextureCache::CachedTexture> SVGTextureCache::s_Cache;

	//---------------------------------------------------------------------
	ID3D11ShaderResourceView* SVGTextureCache::Get(const std::string& a_sPath)
	{
		if (a_sPath.empty())
		{
			return nullptr;
		}

		auto it = s_Cache.find(a_sPath);
		if (it != s_Cache.end())
		{
			return it->second.pTexture;
		}

		core::Data pixels;
		int texW = 0, texH = 0;
		if (!file::SVGParser::Load(a_sPath, pixels, texW, texH))
		{
			return nullptr;
		}

		ID3D11ShaderResourceView* srv = dx11::GetDX11System().CreateTexture(
			pixels.dataAs<unsigned char>(), texW, texH);

		if (!srv)
		{
			return nullptr;
		}

		CachedTexture cached;
		cached.pTexture = srv;
		cached.iWidth = texW;
		cached.iHeight = texH;

		s_Cache[a_sPath] = cached;

		return srv;
	}

	//---------------------------------------------------------------------
	int SVGTextureCache::GetWidth(const std::string& a_sPath)
	{
		auto it = s_Cache.find(a_sPath);
		return (it != s_Cache.end()) ? it->second.iWidth : 0;
	}

	//---------------------------------------------------------------------
	int SVGTextureCache::GetHeight(const std::string& a_sPath)
	{
		auto it = s_Cache.find(a_sPath);
		return (it != s_Cache.end()) ? it->second.iHeight : 0;
	}

	//---------------------------------------------------------------------
	void SVGTextureCache::Shutdown()
	{
		for (auto& [path, tex] : s_Cache)
		{
			if (tex.pTexture)
			{
				tex.pTexture->Release();
				tex.pTexture = nullptr;
			}
		}
		s_Cache.clear();
	}
}
