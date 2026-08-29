#include "./file.h"

// core
#include <helib/core/Data.h>

#include <cstdio>
#include <cstring>
#include <system_error>

namespace humongousexplorer::file
{
	// Platform independent open-file.
	namespace
	{
		FILE* OpenFile(const fs::path& a_Path, const char* a_Mode)
		{
#ifdef _WIN32
			FILE* file = nullptr;
			// Use wide path on Windows for unicode support (� etc.)
			std::wstring wMode(a_Mode, a_Mode + std::strlen(a_Mode));
			_wfopen_s(&file, a_Path.wstring().c_str(), wMode.c_str());
			return file;
#else
			return fopen(a_Path.c_str(), a_Mode);
#endif
		}
	}

	//---------------------------------------------------------------------
	bool LoadFile(const fs::path& a_Path, core::Data& a_Data)
	{
		std::error_code ec;
		uintmax_t fsize = fs::file_size(a_Path, ec);
		if (ec)
		{
			return false;
		}

		if (fsize == 0)
		{
			a_Data = core::Data(0);
			return true;
		}

		if (fsize > static_cast<uintmax_t>(SIZE_MAX))
		{
			return false;
		}

		size_t fileSize = static_cast<size_t>(fsize);

		FILE* file = OpenFile(a_Path, "rb");
		if (!file)
		{
			return false;
		}

		a_Data = core::Data(fileSize);

		const size_t bytesRead = fread(a_Data.data(), 1, fileSize, file);

		fclose(file);

		return bytesRead == fileSize;
	}

	//---------------------------------------------------------------------
	bool SaveFile(const fs::path& a_Path, const core::Data& a_Data)
	{
		fs::path sanitizedPath = a_Path.lexically_normal();
		FILE* file = OpenFile(sanitizedPath, "wb");
		if (!file)
		{
			return false;
		}

		const size_t bytesWritten = fwrite(a_Data.data(), 1, a_Data.size(), file);

		fclose(file);

		return bytesWritten == a_Data.size();
	}

	//---------------------------------------------------------------------
	bool CreateDirectory(const fs::path& a_Path)
	{
		fs::path sanitizedPath = a_Path.lexically_normal();
		return fs::create_directories(sanitizedPath);
	}

}