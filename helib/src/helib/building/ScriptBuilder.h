#pragma once

#include <helib/building/BuilderBase.h>

// external
#include <vector>

#include <helib/script/OPCodesHE.h>

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
	// TalkScript
	//---------------------------------------------------------------------
	class TalkScript
	{
	public:
		parsing::Chunk* GetChunk();
		const std::vector<parsing::Chunk*> GetTALKChunks() const;

		void SetChunk(parsing::Chunk* a_pChunk);
		void AddTALKChunk(parsing::Chunk* a_pChunk);
	private:
		parsing::Chunk* m_pChunk = nullptr; // SCRP/LSCR/LSC2/VERB/EXCD/ENCD in (A).
		std::vector<parsing::Chunk*> m_aTALKChunks;
	};

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
		std::vector<TalkScript> m_aTALKScripts;

		script::OPCodeMap m_mOPCodeMap;
	};
}