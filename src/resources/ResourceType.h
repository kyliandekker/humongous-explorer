#pragma once

#include <string>

namespace humongousexplorer::resources
{
	//---------------------------------------------------------------------
	enum class ResourceType
	{
		Unknown,

		RoomBackground, // Big images.
		RoomImage, // Room-owned image resource.
		RoomImageLayer, // Separate layer showing the room-owned image at the position on the background.

		Talkie, // Voice files.
		SFX, // SFX.
		Song, // Soundtracks.

		LocalScript, // Local room script.
		GlobalScript, // Global script available from any room.
		VerbScript, // Player interaction script.

		Room // Used internally.
	};

	//---------------------------------------------------------------------
	std::string GetIconFromResourceType(ResourceType a_ResourceType);

	//---------------------------------------------------------------------
	std::string GetNameFromResourceType(ResourceType a_ResourceType);
}