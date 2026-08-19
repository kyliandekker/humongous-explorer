#pragma once

#include <string>

namespace humongousexplorer::core
{
	//---------------------------------------------------------------------
	// LoadStatus
	//---------------------------------------------------------------------
	/// <summary>
	/// Indicates the overall outcome of a load operation that may involve multiple items.
	/// </summary>
	enum class LoadStatus
	{
		Success,
		PartialSuccess,
		Failure,
	};

	//---------------------------------------------------------------------
	// LoadResult
	//---------------------------------------------------------------------
	/// <summary>
	/// Represents the outcome of a load operation, containing a status and an optional error message.
	/// </summary>
	struct LoadResult
	{
		LoadStatus status = LoadStatus::Failure;
		std::string errorMessage;
	};
}
