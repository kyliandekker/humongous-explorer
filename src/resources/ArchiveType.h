#pragma once

#include <string>

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	enum class ArchiveType
	{
		Unknown,
		Folder,

		HE0,
		HE1,
		HE2,
		HE3,
		HE4,
		HE7,
		HE8,

		A,
	};

	//---------------------------------------------------------------------
	inline std::string GetIconFromArchiveType(ArchiveType a_ArchiveType)
	{
		std::string base = "../icons/";
		switch (a_ArchiveType)
		{
			case ArchiveType::Unknown:
			{
				return base + "icon_all_files.svg";
			}
			case ArchiveType::Folder:
			{
				return base + "icon_folder_open.svg";
			}
			case ArchiveType::HE0:
			{
				return base + "icon_he0.svg";
			}
			case ArchiveType::HE1:
			{
				return base + "icon_he1.svg";
			}
			case ArchiveType::HE2:
			{
				return base + "icon_he2.svg";
			}
			case ArchiveType::HE3:
			{
				return base + "icon_he3.svg";
			}
			case ArchiveType::HE4:
			{
				return base + "icon_he4.svg";
			}
			case ArchiveType::HE7:
			{
				return base + "icon_he7.svg";
			}
			case ArchiveType::HE8:
			{
				return base + "icon_he8.svg";
			}
			case ArchiveType::A:
			{
				return base + "icon_a.svg";
			}
		}
		return "";
	}

	//---------------------------------------------------------------------
	inline std::string GetNameFromArchiveType(ArchiveType a_ArchiveType)
	{
		switch (a_ArchiveType)
		{
			case ArchiveType::Unknown:
			{
				return "Unknown";
			}
			case ArchiveType::Folder:
			{
				return "Folder";
			}
			case ArchiveType::HE0:
			{
				return "HE0";
			}
			case ArchiveType::HE1:
			{
				return "HE1";
			}
			case ArchiveType::HE2:
			{
				return "HE2";
			}
			case ArchiveType::HE3:
			{
				return "HE3";
			}
			case ArchiveType::HE4:
			{
				return "HE4";
			}
			case ArchiveType::HE7:
			{
				return "HE7";
			}
			case ArchiveType::HE8:
			{
				return "HE8";
			}
			case ArchiveType::A:
			{
				return "(a)";
			}
		}
		return "";
	}
}