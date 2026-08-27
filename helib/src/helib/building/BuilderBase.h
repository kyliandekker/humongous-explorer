#pragma once

namespace humongousexplorer::archive
{
	class Archive;
}
namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// BuilderBase
	//---------------------------------------------------------------------
	/// <summary>
	/// Provides the base functionality for an archive builder.
	/// </summary>
	class BuilderBase
	{
	public:
		/// <summary>
		/// Associates the chunks before rebuilding other archive this archive is dependent on.
		/// </summary>
		virtual bool Precache(archive::Archive& a_Archive) = 0;
		
		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		virtual bool Build(archive::Archive& a_Archive) = 0;
	};
}