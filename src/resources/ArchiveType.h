#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "parsing/ChunkIDs.h"
#include "resources/ResourceType.h"

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
	inline ArchiveType GetArchiveTypeFromExtension(const std::string& a_sExtension)
	{
		std::string ext = a_sExtension;
		for (auto& c : ext) c = static_cast<char>(::toupper(c));
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
		if (ext == "(A)")
		{
			return ArchiveType::A;
		}
		return ArchiveType::Unknown;
	}

	struct DisplayableChunk
	{
		resources::ResourceType m_eResourceType;
		bool m_bVisible = true;
	};

	static const std::unordered_map<std::string, DisplayableChunk> s_Empty =
	{
	};

	static const std::unordered_map<std::string, DisplayableChunk> s_ADisplayableChunks =
	{
		{ parsing::LFLF_CHUNK_ID, { resources::ResourceType::Room, true } },

		{ parsing::SCRP_CHUNK_ID, { resources::ResourceType::GlobalScript, false } },
		{ parsing::EXCD_CHUNK_ID, { resources::ResourceType::GlobalScript, false } },
		{ parsing::ENCD_CHUNK_ID, { resources::ResourceType::GlobalScript, false } },

		{ parsing::LSCR_CHUNK_ID, { resources::ResourceType::LocalScript, false } },
		{ parsing::LSC2_CHUNK_ID, { resources::ResourceType::LocalScript, false } },

		{ parsing::VERB_CHUNK_ID, { resources::ResourceType::VerbScript, false } },

		{ parsing::IM00_CHUNK_ID, { resources::ResourceType::RoomBackground, false } },

		{ parsing::IM01_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM02_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM03_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM04_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM05_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM06_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM07_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM08_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM09_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM10_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM11_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM12_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM13_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM14_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM15_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM16_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM17_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM0A_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM0B_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM0C_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM0D_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM0E_CHUNK_ID, { resources::ResourceType::RoomImage, false } },
		{ parsing::IM0F_CHUNK_ID, { resources::ResourceType::RoomImage, false } },

		{ parsing::DIGI_CHUNK_ID, { resources::ResourceType::SFX, false } },

		{ parsing::TALK_CHUNK_ID, { resources::ResourceType::Talkie, false } }
	};

	static const std::unordered_map<std::string, DisplayableChunk> s_HE0DisplayableChunks =
	{
		{ parsing::RNAM_CHUNK_ID, { resources::ResourceType::Room, false } }
	};

	static const std::unordered_map<std::string, DisplayableChunk> s_HE2DisplayableChunks =
	{
		{ parsing::TALK_CHUNK_ID, { resources::ResourceType::Talkie, false } }
	};

	static const std::unordered_map<std::string, DisplayableChunk> s_HE4DisplayableChunks =
	{
		{ parsing::SGEN_CHUNK_ID, { resources::ResourceType::Song, false } }
	};

	//---------------------------------------------------------------------
	inline const std::unordered_map<std::string, DisplayableChunk>& GetDisplayableChunks(ArchiveType a_ArchiveType)
	{
		switch (a_ArchiveType)
		{
			case ArchiveType::Unknown:
			case ArchiveType::Folder:
			{
				return s_Empty;
			}
			case ArchiveType::HE0:
			{
				return s_Empty;
			}
			case ArchiveType::HE1:
			{
				return s_Empty;
			}
			case ArchiveType::HE2:
			{
				return s_HE2DisplayableChunks;
			}
			case ArchiveType::HE3:
			{
				return s_Empty;
			}
			case ArchiveType::HE4:
			{
				return s_HE4DisplayableChunks;
			}
			case ArchiveType::HE7:
			{
				return s_Empty;
			}
			case ArchiveType::HE8:
			{
				return s_Empty;
			}
			case ArchiveType::A:
			{
				return s_ADisplayableChunks;
			}
		}
		return s_Empty;
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