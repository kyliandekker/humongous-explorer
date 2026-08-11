#include "ResourceType.h"

#include "humongous/ChunkIDs.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	ResourceType GetResourceTypeFromChunkID(const std::string& a_sChunkID)
	{
		if (a_sChunkID == parsing::LFLF_CHUNK_ID)
		{
			return ResourceType::RoomBackground;
		}
		else if (a_sChunkID == parsing::OBIM_CHUNK_ID)
		{
			return ResourceType::RoomImage;
		}
		else if (a_sChunkID == parsing::TALK_CHUNK_ID)
		{
			return ResourceType::Talkie;
		}
		else if (a_sChunkID == parsing::DIGI_CHUNK_ID)
		{
			return ResourceType::SFX;
		}
		else if (a_sChunkID == parsing::DIGI_CHUNK_ID)
		{
			return ResourceType::Song;
		}

		return ResourceType::Unknown;
	}

	//---------------------------------------------------------------------
	std::string GetIconFromResourceType(ResourceType a_ResourceType)
	{
		std::string base = "../icons/";
		switch (a_ResourceType)
		{
			case ResourceType::Unknown:
			{
				return base + "icon_all_files.svg";
			}
			case ResourceType::Talkie:
			{
				return base + "icon_talkie.svg";
			}
			case ResourceType::SFX:
			{
				return base + "icon_sfx.svg";
			}
			case ResourceType::Song:
			{
				return base + "icon_song.svg";
			}
			case ResourceType::RoomBackground:
			{
				return base + "icon_background.svg";
			}
			case ResourceType::RoomImage:
			case ResourceType::RoomImageLayer:
			{
				return base + "icon_image.svg";
			}
			case ResourceType::LocalScript:
			{
				return base + "icon_local_script.svg";
			}
			case ResourceType::GlobalScript:
			{
				return base + "icon_global_script.svg";
			}
			case ResourceType::VerbScript:
			{
				return base + "icon_verb_script.svg";
			}
			case ResourceType::Room:
			{
				return base + "icon_room.svg";
			}
		}
		return "";
	}

	//---------------------------------------------------------------------
	std::string GetNameFromResourceType(ResourceType a_ResourceType)
	{
		switch (a_ResourceType)
		{
			case ResourceType::Unknown:
			{
				return "All Types";
			}
			case ResourceType::Talkie:
			{
				return "Talkie";
			}
			case ResourceType::SFX:
			{
				return "SFX";
			}
			case ResourceType::Song:
			{
				return "Song";
			}
			case ResourceType::RoomBackground:
			{
				return "Room Background";
			}
			case ResourceType::RoomImage:
			{
				return "Room Sprite";
			}
			case ResourceType::RoomImageLayer:
			{
				return "Room Sprite (Layered)";
			}
			case ResourceType::LocalScript:
			{
				return "Local Script";
			}
			case ResourceType::GlobalScript:
			{
				return "Global Script";
			}
			case ResourceType::VerbScript:
			{
				return "Verb Script";
			}
			case ResourceType::Room:
			{
				return "Room";
			}
		}
		return "";
	}
}