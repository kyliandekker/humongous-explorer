#pragma once

namespace humongousexplorer::archive
{
	class ArchiveSet;
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
		virtual bool Bind(archive::ArchiveSet& a_ArchiveSet) = 0;
		
		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		virtual bool Build() = 0;
	};
}