#pragma once

#include <string>

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	enum class ResourceType
	{
		Unknown,
		Talkie, // Voice files.
		SFX, // SFX.
		Song, // Soundtracks.
		RoomBackground, // Big images.
		RoomImage, // Room-owned image resource.
		RoomImageLayer, // Separate layer showing the room-owned image at the position on the background.
		LocalScript, // Local room script.
		GlobalScript, // Global script available from any room.
		VerbScript, // Player interaction script.
		Room
	};

	//---------------------------------------------------------------------
	inline std::string GetIconFromResourceType(ResourceType a_ResourceType)
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
				return base + "icon_folder_open.svg";
			}
		}
	}

	//---------------------------------------------------------------------
	inline std::string GetNameFromResourceType(ResourceType a_ResourceType)
	{
		switch (a_ResourceType)
		{
			case ResourceType::Unknown:
			{
				return "All Types";
			}
			case ResourceType::Talkie:
			{
				return "Talkies";
			}
			case ResourceType::SFX:
			{
				return "SFX";
			}
			case ResourceType::Song:
			{
				return "Songs";
			}
			case ResourceType::RoomBackground:
			{
				return "Room Backgrounds";
			}
			case ResourceType::RoomImage:
			{
				return "Sprites";
			}
			case ResourceType::RoomImageLayer:
			{
				return "Room Image Layers";
			}
			case ResourceType::LocalScript:
			{
				return "Local Scripts";
			}
			case ResourceType::GlobalScript:
			{
				return "Global Scripts";
			}
			case ResourceType::VerbScript:
			{
				return "Verb Scripts";
			}
			case ResourceType::Room:
			{
				return "Rooms";
			}
		}
		return "";
	}
}