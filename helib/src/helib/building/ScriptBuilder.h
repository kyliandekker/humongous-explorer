#pragma once

#include <helib/building/BuilderBase.h>

// external
#include <vector>

#include <helib/script/OPCodesHENew.h>
#include <helib/building/script/Script.h>

namespace humongousexplorer::archive
{
	class Archive;
}
namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// ScriptBuilder
	//---------------------------------------------------------------------
	/// <summary>
	/// Rebuilds scripts by updating their TALK data.
	/// </summary>
	class ScriptBuilder : public BuilderBase
	{
	public:
		/// <summary>
		/// Associates the chunks before rebuilding other archive this archive is dependent on.
		/// </summary>
		bool Bind(archive::ArchiveSet& a_ArchiveSet) override;

		/// <summary>
		/// Builds the chunks in the associated archive.
		/// </summary>
		bool Build() override;
	protected:
		archive::Archive* m_pA = nullptr;
		archive::Archive* m_pHE2 = nullptr;

		script::OPCodeMap m_mOPCodeMap;
		std::vector<Script> m_aTalkScripts;
	};
}