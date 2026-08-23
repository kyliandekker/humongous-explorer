#pragma once

#include <string>
#include <unordered_map>

struct ID3D11ShaderResourceView;

namespace humongousexplorer::dx11
{
	//---------------------------------------------------------------------
	// SVGTextureCache
	//---------------------------------------------------------------------
	/// <summary>
	/// Loads SVG files and caches them as DX11 textures.
	/// </summary>
	class SVGTextureCache
	{
	public:
		/// <summary>
		/// Returns a cached texture for the given SVG path, loading it if needed.
		/// </summary>
		/// <param name="a_sName">Name of the SVG file.</param>
		/// <returns>The texture, or nullptr on failure.</returns>
		static ID3D11ShaderResourceView* Get(const std::string& a_sName);

		/// <summary>
		/// Returns the native width of the cached SVG.
		/// </summary>
		static int GetWidth(const std::string& a_sPath);

		/// <summary>
		/// Returns the native height of the cached SVG.
		/// </summary>
		static int GetHeight(const std::string& a_sPath);

		/// <summary>
		/// Releases all cached textures.
		/// </summary>
		static void Shutdown();

	private:
		struct CachedTexture
		{
			ID3D11ShaderResourceView* pTexture = nullptr;
			int iWidth = 0;
			int iHeight = 0;
		};

		static std::unordered_map<std::string, CachedTexture> s_Cache;
	};
}
