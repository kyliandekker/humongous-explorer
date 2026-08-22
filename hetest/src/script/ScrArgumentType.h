#pragma once

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	enum class ScrArgumentType
	{
		Byte, // Unsigned.
		Int16, // Signed.
		Int32, // Signed.
		Ref, // Signed, Int16 or Int32 depending on version. Signed because it can jump to a previous position.
		String // Keep in mind it is the size of string + null termination.
	};
}
