#include "./ScriptBuilder.h"

#include <cassert>

#include <helib/archive/ArchiveSet.h>
#include <helib/archive/Archive.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/script/ScrInstruction.h>
#include <helib/script/ScrArgumentType.h>
#include <helib/script/OPCodesHE.h>

namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// TalkScript
	//---------------------------------------------------------------------
	parsing::Chunk* TalkScript::GetChunk()
	{
		return m_pChunk;
	}

	//---------------------------------------------------------------------
	const std::vector<parsing::Chunk*> TalkScript::GetTALKChunks() const
	{
		return m_aTALKChunks;
	}

	//---------------------------------------------------------------------
	void TalkScript::SetChunk(parsing::Chunk* a_pChunk)
	{
		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Could not set TALK script: Script was null.");
			return;
		}

		bool isScript =
			a_pChunk->GetTag() == parsing::SCRP_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::ENCD_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::EXCD_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::LSCR_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::LSC2_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::VERB_CHUNK_ID;

		assert(isScript);
		if (!isScript)
		{
			core::Log(core::LogLevel::Error, "Could not TALK script: Chunk was not a script.");
			return;
		}

		m_pChunk = a_pChunk;
	}

	//---------------------------------------------------------------------
	void TalkScript::AddTALKChunk(parsing::Chunk* a_pChunk)
	{
		assert(a_pChunk);
		if (!a_pChunk)
		{
			core::Log(core::LogLevel::Error, "Could not set TALK script: Script was null.");
			return;
		}

		bool isTALK =
			a_pChunk->GetTag() == parsing::TALK_CHUNK_ID;

		assert(isTALK);
		if (!isTALK)
		{
			core::Log(core::LogLevel::Error, "Could not TALK script: Chunk was not a TALK chunk.");
			return;
		}

		m_aTALKChunks.push_back(a_pChunk);
	}

	//---------------------------------------------------------------------
	size_t GetStartOfByteCode(parsing::Chunk* a_pChunk)
	{
		const core::Data& data = a_pChunk->GetData();
		if (
			a_pChunk->GetTag() == parsing::SCRP_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::ENCD_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::EXCD_CHUNK_ID
		)
		{
			return 0;
		}
		else if (a_pChunk->GetTag() == parsing::LSCR_CHUNK_ID) // Skip ID (8bit unsigned int)
		{
			return sizeof(uint8_t);
		}
		else if (a_pChunk->GetTag() == parsing::LSC2_CHUNK_ID) // Skip ID (32bit unsigned int)
		{
			return sizeof(uint32_t);
		}
		else if (a_pChunk->GetTag() == parsing::VERB_CHUNK_ID)
		{
			size_t tell = 0;
			size_t dataSize = data.size();
			while (tell < dataSize)
			{
				uint8_t key = data[tell];
				tell += 1; // key byte
				if (key == 0x00)
				{
					break;
				}
				tell += 2; // 2-byte offset
			}

			return tell;
		}

		assert(false);
		core::Log(core::LogLevel::Error, "Could not bind scripts: Could not recognize script type.");

		return 0;
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<script::ScrInstruction>> GetInstructions(size_t a_iTell, const core::Data& a_Data, const script::OPCodeMap& a_mOPCodeMap)
	{
		std::vector<std::unique_ptr<script::ScrInstruction>> instructions;

		while (a_iTell < a_Data.size())
		{
			const unsigned char* pureDat = a_Data.dataAs<unsigned char>() + a_iTell;
			uint8_t code = *pureDat;

			auto it = a_mOPCodeMap.find(code);

			assert(it != a_mOPCodeMap.end());
			if (it == a_mOPCodeMap.end())
			{
				return {};
			}

			std::vector<std::unique_ptr<humongousexplorer::script::ScrArgument>> args = it->second.GetSizeFn()(code, pureDat + 1);
			std::unique_ptr<humongousexplorer::script::ScrInstruction> instr = std::make_unique<humongousexplorer::script::ScrInstruction>(code);
			for (std::unique_ptr<humongousexplorer::script::ScrArgument>& arg : args)
			{
				instr->AddArgument(arg);
			}
			std::size_t size = instr->GetSize();
			instructions.push_back(std::move(instr));

			// Go to next arg.
			a_iTell += size;
		}

		return instructions;
	}

	//---------------------------------------------------------------------
	struct TalkRef
	{
		std::size_t pos;
		std::size_t size;
	};

	//---------------------------------------------------------------------
	std::vector<TalkRef> GetTalkRefs(const humongousexplorer::core::Data& a_Data)
	{
		std::vector<TalkRef> out;
		for (std::size_t i = 0; i + 1 < a_Data.size(); )
		{
			if (a_Data[i] == 0x7F && a_Data[i + 1] == 0x54)
			{
				std::size_t a = i + 2;
				std::size_t b = a;
				while (b < a_Data.size() && a_Data[b] != 0x7F)
				{
					b += 1;
				}
				std::size_t comma = a;
				bool found = false;
				for (std::size_t k = a; k < b; k += 1)
				{
					if (a_Data[k] == ',')
					{
						comma = k;
						found = true;
					}
				}
				if (found == true)
				{
					std::string posStr = std::string(a_Data.dataAs<const char>() + a, comma - a);
					std::string sizeStr = std::string(a_Data.dataAs<const char>() + comma + 1, b - comma - 1);
					TalkRef ref;
					ref.pos = std::stoul(posStr);
					ref.size = std::stoul(sizeStr);
					out.push_back(ref);
				}
				i = b + 1;
			}
			else
			{
				i += 1;
			}
		}
		return out;
	}

	//---------------------------------------------------------------------
	// ScriptBuilder
	//---------------------------------------------------------------------
	bool ScriptBuilder::Bind(archive::ArchiveSet& a_ArchiveSet)
	{
		for (std::unique_ptr<archive::Archive>& archive : a_ArchiveSet.GetArchives())
		{
			if (archive->GetType() == archive::ArchiveType::HE2)
			{
				m_pHE2 = archive.get();
			}
			else if (archive->GetType() == archive::ArchiveType::A)
			{
				m_pA = archive.get();
			}
		}

		assert(m_pHE2);
		if (!m_pHE2)
		{
			core::Log(core::LogLevel::Error, "Could not bind scripts: Could not find HE2 archive.");
			return false;
		}

		assert(m_pA);
		if (!m_pA)
		{
			core::Log(core::LogLevel::Error, "Could not bind scripts: Could not find (A) archive.");
			return false;
		}

		script::GetOPCodeTable(m_mOPCodeMap, a_ArchiveSet.GetScriptVersion(), a_ArchiveSet.GetHEVersion());
		assert(!m_mOPCodeMap.empty());
		if (m_mOPCodeMap.empty())
		{
			core::Log(core::LogLevel::Error, "Could not bind scripts: Could not find OP codes map.");
			return false;
		}

		// Find all scripts.
		std::vector<parsing::Chunk*> scripts;
		m_pA->GetRoot().TryFindChildren({
				parsing::SCRP_CHUNK_ID,
				parsing::LSCR_CHUNK_ID,
				parsing::LSC2_CHUNK_ID,
				parsing::ENCD_CHUNK_ID,
				parsing::EXCD_CHUNK_ID,
				parsing::VERB_CHUNK_ID,
			}, scripts
		);
		assert(!scripts.empty());
		if (scripts.empty())
		{
			core::Log(core::LogLevel::Error, "Could not bind scripts: Could not find any scripts in (A).");
			return false;
		}
		
		// TODO: Increase speed. This is super slow. Constantly allocating arguments just to check if they are certain ones and throw away arguments anyways is wasteful.
		for (parsing::Chunk* chunk : scripts)
		{
			assert(chunk);
			if (!chunk)
			{
				core::Log(core::LogLevel::Error, "Could not bind scripts: Script was null.");
				return false;
			}

			size_t tell = GetStartOfByteCode(chunk);
			std::vector<std::unique_ptr<script::ScrInstruction>> instructions = GetInstructions(tell, chunk->GetData(), m_mOPCodeMap);

			TalkScript talkScript;
			talkScript.SetChunk(chunk);

			bool isTalkScript = false;
			for (std::unique_ptr<script::ScrInstruction>& instruction : instructions)
			{
				// So contrary to popular belief, talkActor is not the only code for calling TALKies.
				// These are known to call TALKs as of right now:
				//	* o72_getScriptString: By far the most calls.
				//	* o72_talkActor: Second-most calls.
				//	* o72_talkEgo: Third-most calls.
				//	* o6_printLine: Really only occasionally, but still a significant amount.
				//	* o6_printActor: Weirdly few calls.
				for (std::unique_ptr<script::ScrArgument>& arg : instruction->GetArguments())
				{
					isTalkScript = true;

					std::vector<TalkRef> talks = GetTalkRefs(arg->GetData());
					for (const TalkRef& talkRef : talks)
					{
						parsing::Chunk* referencedTALK = m_pHE2->GetRoot().FindChunkAt(talkRef.pos);
						assert(referencedTALK);
						if (!referencedTALK)
						{
							core::Log(core::LogLevel::Error, "Could not bind scripts: Script referenced invalid TALK chunk.");
							return false;
						}

						assert(referencedTALK->WholeChunkSize() == talkRef.size);
						if (referencedTALK->WholeChunkSize() != talkRef.size)
						{
							core::Log(core::LogLevel::Error, "Could not bind scripts: Referenced TALK chunk in script was not the same size.");
							return false;
						}

						talkScript.AddTALKChunk(m_pHE2->GetRoot().FindChunkAt(talkRef.pos));
					}
				}
			}

			if (isTalkScript)
			{
				m_aTALKScripts.push_back(talkScript);
			}
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool ScriptBuilder::Build()
	{
		assert(m_pHE2);
		if (!m_pHE2)
		{
			core::Log(core::LogLevel::Error, "Could not build scripts: HE2 archive was null.");
			return false;
		}

		assert(m_pA);
		if (!m_pA)
		{
			core::Log(core::LogLevel::Error, "Could not build scripts: (A) archive was null.");
			return false;
		}

		assert(!m_mOPCodeMap.empty());
		if (m_mOPCodeMap.empty())
		{
			core::Log(core::LogLevel::Error, "Could not build scripts: Could not find OP codes map.");
			return false;
		}

		for (auto& talk : m_aTALKScripts)
		{
			//talk.GetChunk();
		}

		return true;
	}
}