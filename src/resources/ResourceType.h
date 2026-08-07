#pragma once

namespace humongousexplorer::resources
{
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
}