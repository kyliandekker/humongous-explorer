#include "UIHelpers.h"

#include "archive/ArchiveType.h"

#include "parsing/ChunkIDs.h"

#include "resources/ResourceType.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	std::string GetIconFromArchiveType(archive::ArchiveType a_ArchiveType)
	{
		switch (a_ArchiveType)
		{
			case archive::ArchiveType::Unknown:
			{
				return "icon_all_files.svg";
			}
			case archive::ArchiveType::HE0:
			{
				return "icon_he0.svg";
			}
			case archive::ArchiveType::HE1:
			{
				return "icon_he1.svg";
			}
			case archive::ArchiveType::HE2:
			{
				return "icon_he2.svg";
			}
			case archive::ArchiveType::HE3:
			{
				return "icon_he3.svg";
			}
			case archive::ArchiveType::HE4:
			{
				return "icon_he4.svg";
			}
			case archive::ArchiveType::HE7:
			{
				return "icon_he7.svg";
			}
			case archive::ArchiveType::HE8:
			{
				return "icon_he8.svg";
			}
			case archive::ArchiveType::HE9:
			{
				return "icon_he9.svg";
			}
			case archive::ArchiveType::A:
			{
				return "icon_a.svg";
			}
		}
		return "";
	}

	//---------------------------------------------------------------------
	static const std::unordered_map<std::string_view, DisplayableChunk> s_Empty =
	{
	};

	//---------------------------------------------------------------------
	static const std::unordered_map<std::string_view, DisplayableChunk> s_ADisplayableChunks =
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

	//---------------------------------------------------------------------
	static const std::unordered_map<std::string_view, DisplayableChunk> s_HE0DisplayableChunks =
	{
		{ parsing::RNAM_CHUNK_ID, { resources::ResourceType::Room, false } }
	};

	//---------------------------------------------------------------------
	static const std::unordered_map<std::string_view, DisplayableChunk> s_HE2DisplayableChunks =
	{
		{ parsing::TALK_CHUNK_ID, { resources::ResourceType::Talkie, false } }
	};

	//---------------------------------------------------------------------
	static const std::unordered_map<std::string_view, DisplayableChunk> s_HE4DisplayableChunks =
	{
		{ parsing::SGEN_CHUNK_ID, { resources::ResourceType::Song, false } }
	};

	//---------------------------------------------------------------------
	const std::unordered_map<std::string_view, DisplayableChunk>& GetDisplayableChunks(archive::ArchiveType a_ArchiveType)
	{
		switch (a_ArchiveType)
		{
			case archive::ArchiveType::Unknown:
			{
				return s_Empty;
			}
			case archive::ArchiveType::HE0:
			{
				return s_Empty;
			}
			case archive::ArchiveType::HE1:
			{
				return s_Empty;
			}
			case archive::ArchiveType::HE2:
			{
				return s_HE2DisplayableChunks;
			}
			case archive::ArchiveType::HE3:
			{
				return s_Empty;
			}
			case archive::ArchiveType::HE4:
			{
				return s_HE4DisplayableChunks;
			}
			case archive::ArchiveType::HE7:
			{
				return s_Empty;
			}
			case archive::ArchiveType::HE8:
			{
				return s_Empty;
			}
			case archive::ArchiveType::A:
			{
				return s_ADisplayableChunks;
			}
		}
		return s_Empty;
	}
}
