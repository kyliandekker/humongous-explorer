#include "file.h"

// core
#include "core/DataStream.h"

namespace humongousexplorer::file
{
	// Platform independent open-file.
	namespace
	{
		FILE* OpenFile(const fs::path& a_Path, const char* a_Mode)
		{
#ifdef _WIN32
			FILE* file = nullptr;
			fopen_s(&file, a_Path.string().c_str(), a_Mode);
			return file;
#else
			return fopen(a_Path.string().c_str(), a_Mode);
#endif
		}
	}

	//---------------------------------------------------------------------
	bool LoadFile(const fs::path& a_Path, core::Data& a_Data)
	{
		if (!fs::exists(a_Path))
		{
			return false;
		}

		FILE* file = OpenFile(a_Path, "rb");
		if (!file)
		{
			return false;
		}

		if (fseek(file, 0, SEEK_END) != 0)
		{
			fclose(file);
			return false;
		}

		long fileSizeResult = ftell(file);

		if (fileSizeResult <= 0)
		{
			fclose(file);
			return false;
		}

		size_t fileSize = static_cast<size_t>(fileSizeResult);

		rewind(file);

		a_Data = core::DataStream(fileSize);

		const size_t bytesRead = fread(a_Data.data(), 1, fileSize, file);

		fclose(file);

		return bytesRead == static_cast<size_t>(fileSize);
	}

	//---------------------------------------------------------------------
	bool SaveFile(const fs::path& a_Path, const core::Data& a_Data)
	{
		FILE* file = OpenFile(a_Path, "wb");
		if (!file)
		{
			return false;
		}

		const size_t bytesWritten = fwrite(a_Data.data(), 1, a_Data.size(), file);

		fclose(file);

		return bytesWritten == a_Data.size();
	}
}