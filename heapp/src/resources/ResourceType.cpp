#include "ResourceType.h"

#include "parsing/ChunkIDs.h"

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	std::string GetIconFromResourceType(ResourceType a_ResourceType)
	{
		switch (a_ResourceType)
		{
			case ResourceType::Unknown:
			{
				return "icon_all_files.svg";
			}
			case ResourceType::Talkie:
			{
				return "icon_talkie.svg";
			}
			case ResourceType::SFX:
			{
				return "icon_sfx.svg";
			}
			case ResourceType::Song:
			{
				return "icon_song.svg";
			}
			case ResourceType::RoomBackground:
			{
				return "icon_background.svg";
			}
			case ResourceType::RoomImage:
			case ResourceType::RoomImageLayer:
			{
				return "icon_image.svg";
			}
			case ResourceType::LocalScript:
			{
				return "icon_local_script.svg";
			}
			case ResourceType::GlobalScript:
			{
				return "icon_global_script.svg";
			}
			case ResourceType::VerbScript:
			{
				return "icon_verb_script.svg";
			}
			case ResourceType::Room:
			{
				return "icon_room.svg";
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