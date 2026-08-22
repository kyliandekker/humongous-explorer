#include "winfile.h"

// external
#include <ShlObj_core.h>

// graphics
#include "win32/WINPCH.h"

namespace humongousexplorer::file
{
	//---------------------------------------------------------------------
	bool genericFileOpen(fs::path& a_sPath, const IID a_Rclsid, FILEOPENDIALOGOPTIONS a_Options, const std::vector<COMDLG_FILTERSPEC>& a_aFilters = {}, const fs::path& a_InitialPath = {})
	{
		CoInitialize(nullptr);

		IFileDialog* pfd;
		HRESULT hr = CoCreateInstance(a_Rclsid, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
		if (FAILED(hr))
		{
			return false;
		}

		// Set file type filters
		if (!a_aFilters.empty())
		{
			pfd->SetFileTypes(static_cast<UINT>(a_aFilters.size()), a_aFilters.data());
		}

		// Set options
		DWORD dwOptions;
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | a_Options);
		}

		// Set initial folder if provided
		if (!a_InitialPath.empty())
		{
			IShellItem* psiFolder = nullptr;
			hr = SHCreateItemFromParsingName(a_InitialPath.c_str(), NULL, IID_PPV_ARGS(&psiFolder));
			if (SUCCEEDED(hr))
			{
				pfd->SetFolder(psiFolder);
				psiFolder->Release();
			}
		}

		// Show dialog
		if (SUCCEEDED(pfd->Show(NULL)))
		{
			IShellItem* psi;
			if (SUCCEEDED(pfd->GetResult(&psi)))
			{
				LPWSTR pszPath;
				psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszPath);

				std::wstring wide(pszPath);
				fs::path resultPath(wide);

				// If user didnÃ¯Â¿Â½t specify extension, apply default one from filter
				if (resultPath.extension().empty() && !a_aFilters.empty())
				{
					UINT fileTypeIndex = 0;
					if (SUCCEEDED(pfd->GetFileTypeIndex(&fileTypeIndex)) && fileTypeIndex > 0 && fileTypeIndex <= a_aFilters.size())
					{
						std::wstring defExt = a_aFilters[fileTypeIndex - 1].pszSpec;
						// pszSpec can be "*.ext1;*.ext2", take the first one and strip "*."
						auto semi = defExt.find(L';');
						if (semi != std::wstring::npos)
						{
							defExt = defExt.substr(0, semi);
						}
						if (defExt.size() > 1 && defExt[0] == L'*')
						{
							defExt = defExt.substr(1); // remove '*', keeps '.'
						}

						resultPath += defExt;
					}
				}

				a_sPath = resultPath;

				psi->Release();
				CoTaskMemFree(pszPath);
				pfd->Release();
				return true;
			}
		}

		pfd->Release();
		return false;
	}

	//---------------------------------------------------------------------
	bool PickContainer(fs::path& a_sPath)
	{
		return genericFileOpen(a_sPath, CLSID_FileOpenDialog, FOS_PICKFOLDERS);
	}

	//---------------------------------------------------------------------
	bool PickFile(fs::path& a_sPath, const std::vector<COMDLG_FILTERSPEC>& a_aFilters, const fs::path& a_InitialPath)
	{
		return genericFileOpen(a_sPath, CLSID_FileOpenDialog, 0, a_aFilters, a_InitialPath);
	}

	//---------------------------------------------------------------------
	bool SaveFile(fs::path& a_sPath, const std::vector<COMDLG_FILTERSPEC>& a_aFilters, const fs::path& a_InitialPath)
	{
		return genericFileOpen(a_sPath, CLSID_FileSaveDialog, 0, a_aFilters, a_InitialPath);
	}

	//---------------------------------------------------------------------
	const fs::path GetAppDataPath()
	{
		PWSTR path_tmp;
		fs::path path;
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
		path = path_tmp;

		fs::create_directories(path);

		return path;
	}

	//---------------------------------------------------------------------
	bool CreateDirectory(const fs::path& a_Path)
	{
		return fs::create_directories(a_Path);
	}

	//---------------------------------------------------------------------
	bool OpenInExplorer(const fs::path& a_Path)
	{
		ShellExecuteA(NULL, "open", fs::absolute(a_Path).string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
		return true;
	}
}