#include "ArchiveType.h"

#include <cctype>

namespace humongousexplorer::archive
{
	//---------------------------------------------------------------------
	ArchiveType GetArchiveTypeFromExtension(const std::string& a_sExtension)
	{
		std::string ext = a_sExtension;
		for (auto& c : ext)
		{
			c = static_cast<char>(::toupper(static_cast<unsigned char>(c)));
		}

		if (ext == "HE0")
		{
			return ArchiveType::HE0;
		}
		if (ext == "HE1")
		{
			return ArchiveType::HE1;
		}
		if (ext == "HE2")
		{
			return ArchiveType::HE2;
		}
		if (ext == "HE3")
		{
			return ArchiveType::HE3;
		}
		if (ext == "HE4")
		{
			return ArchiveType::HE4;
		}
		if (ext == "HE7")
		{
			return ArchiveType::HE7;
		}
		if (ext == "HE8")
		{
			return ArchiveType::HE8;
		}
		if (ext == "HE9")
		{
			return ArchiveType::HE9;
		}
		if (ext == "(A)")
		{
			return ArchiveType::A;
		}
		return ArchiveType::Unknown;
	}
}