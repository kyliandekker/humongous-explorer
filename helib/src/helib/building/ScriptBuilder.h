#pragma once

#include <helib/building/BuilderBase.h>

// external
#include <vector>

#include <helib/script/OPCodesHENew.h>

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
	struct ScriptInfo
	{
		size_t m_iInstrTell;
		size_t m_iInstrEnd;
	};

	struct TalkScriptInfo : public ScriptInfo
	{
		size_t m_iTalkOffset;
		size_t m_iTalkSize;
		parsing::Chunk* m_pTalkChunk = nullptr;
	};

	struct JumpScriptInfo : public ScriptInfo
	{
		size_t m_iJumpTo;
		size_t m_iJumpSize;
	};

	//---------------------------------------------------------------------
	// TalkScript
	//---------------------------------------------------------------------
	class TalkScript
	{
	public:
		parsing::Chunk* GetChunk();

		void SetChunk(parsing::Chunk* a_pChunk);
		std::vector<TalkScriptInfo>& GetTalks();
		std::vector<JumpScriptInfo>& GetJumps();
	private:
		parsing::Chunk* m_pChunk = nullptr; // SCRP/LSCR/LSC2/VERB/EXCD/ENCD in (A).
		std::vector<TalkScriptInfo> m_aTalks;
		std::vector<JumpScriptInfo> m_aJumps;
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
		std::vector<TalkScript> m_aTalkScripts;

		script::OPCodeMap m_mOPCodeMap;
	};
}