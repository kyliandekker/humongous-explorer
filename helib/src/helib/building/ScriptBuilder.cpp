#include "./ScriptBuilder.h"

#include <cassert>
#include <map>

#include <helib/archive/Archive.h>
#include <helib/archive/ArchiveSet.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/script/ScrArgumentType.h>

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
	std::vector<TalkScriptInfo>& TalkScript::GetTalks()
	{
		return m_aTalks;
	}

	//---------------------------------------------------------------------
	std::vector<JumpScriptInfo>& TalkScript::GetJumps()
	{
		return m_aJumps;
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
	struct TalkRef
	{
		size_t pos;
		size_t size;
		size_t strSize;
	};

	//---------------------------------------------------------------------
	std::vector<TalkRef> GetTalkRefs(const humongousexplorer::core::Data& a_Data)
	{
		std::vector<TalkRef> out;
		out.reserve(3);
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
					ref.strSize = posStr.size() + 1 + sizeStr.size();
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

		// Cache chunks in a map beforehand.
		std::unordered_map<std::size_t, parsing::Chunk*> talkChunkTable;
		std::vector<parsing::Chunk*> talkChunks;
		m_pHE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, talkChunks);
		for (parsing::Chunk* talkChunk : talkChunks)
		{
			talkChunkTable[talkChunk->GetOffsetFromRoot()] = talkChunk;
		}

		for (parsing::Chunk* chunk : scripts)
		{
			assert(chunk);
			if (!chunk)
			{
				core::Log(core::LogLevel::Error, "Could not bind scripts: Script was null.");
				return false;
			}

			size_t tell = GetStartOfByteCode(chunk);
			core::Data data = chunk->GetData();

			TalkScript talkScript;
			talkScript.SetChunk(chunk);

			bool isTalkScript = false;

			std::unordered_map<size_t, uint8_t> byteCodePoints;

			while (tell < data.size())
			{
				size_t instrTell = tell;

				const unsigned char* pureDat = data.dataAs<unsigned char>() + tell;
				uint8_t code = *pureDat;

				byteCodePoints[tell] = code;

				auto it = m_mOPCodeMap.find(code);

				assert(it != m_mOPCodeMap.end());
				if (it == m_mOPCodeMap.end())
				{
					core::Log(core::LogLevel::Error, "Could not bind scripts: Script encountered invalid byte code.");
					return false;
				}

				std::vector<script::ArgInfo> args = it->second.GetSizeFn()(code, pureDat + 1);

				size_t size = sizeof(code);

				size_t instrEnd = tell + sizeof(code);
				for (const script::ArgInfo& arg : args)
				{
					instrEnd += arg.m_iSize;
				}

				for (const script::ArgInfo& arg : args)
				{
					const unsigned char* argData = data.dataAs<unsigned char>() + tell + size;

					if (arg.m_eArgumentType == script::ScrArgumentType::String)
					{
						// So contrary to popular belief, talkActor is not the only code for calling TALKies.
						// These are known to call TALKs as of right now:
						//	* o72_getScriptString: By far the most calls.
						//	* o72_talkActor: Second-most calls.
						//	* o72_talkEgo: Third-most calls.
						//	* o6_printLine: Really only occasionally, but still a significant amount.
						//	* o6_printActor: Weirdly few calls.
						std::vector<TalkRef> talks = GetTalkRefs(core::Data(argData, arg.m_iSize));
						assert(talks.size() == 1 || talks.size() == 0);
						for (const TalkRef& talkRef : talks)
						{
							parsing::Chunk* referencedTALK = talkChunkTable[talkRef.pos];
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

							bool isTALK =
								referencedTALK->GetTag() == parsing::TALK_CHUNK_ID;

							assert(isTALK);
							if (!isTALK)
							{
								core::Log(core::LogLevel::Error, "Could not TALK script: Chunk was not a TALK chunk.");
								return false;
							}

							isTalkScript = true;

							talkScript.GetTalks().push_back({ instrTell, instrEnd, talkRef.pos, talkRef.size, referencedTALK });
						}
					}
					else if (arg.m_eArgumentType == script::ScrArgumentType::Ref)
					{
						int32_t jump{};

						if (arg.m_iSize == sizeof(int16_t))
						{
							jump = static_cast<int16_t>(
								static_cast<uint16_t>(argData[0]) |
								(static_cast<uint16_t>(argData[1]) << 8)
								);
						}
						else
						{
							jump = static_cast<int32_t>(
								static_cast<uint32_t>(argData[0]) |
								(static_cast<uint32_t>(argData[1]) << 8) |
								(static_cast<uint32_t>(argData[2]) << 16) |
								(static_cast<uint32_t>(argData[3]) << 24)
								);
						}

						const int32_t endOfArgument =
							static_cast<int32_t>(tell) +
							static_cast<int32_t>(size) +
							static_cast<int32_t>(arg.m_iSize);

						size_t jumpTo = static_cast<size_t>(endOfArgument + jump);
						assert(jumpTo < data.size());
						if (jumpTo >= data.size())
						{
							core::Log(core::LogLevel::Error, "Could not bind scripts: Script encountered invalid jump-to position.");
							return false;
						}

						talkScript.GetJumps().push_back({ instrTell, instrEnd, jumpTo, static_cast<size_t>(jump) });
					}

					size += arg.m_iSize;
				}

				// Go to next arg.
				tell += size;
			}

			if (isTalkScript)
			{
				for (JumpScriptInfo& jump : talkScript.GetJumps())
				{
					auto itJump = byteCodePoints.find(jump.m_iJumpTo);

					assert(itJump != byteCodePoints.end());
					if (itJump == byteCodePoints.end())
					{
						core::Log(core::LogLevel::Error, "Could not bind scripts: Script encountered invalid jump-to position.");
						return false;
					}

					auto it = m_mOPCodeMap.find(itJump->second);

					assert(it != m_mOPCodeMap.end());
					if (it == m_mOPCodeMap.end())
					{
						core::Log(core::LogLevel::Error, "Could not bind scripts: Jump-to position had an invalid byte code.");
						return false;
					}

				}

				m_aTalkScripts.push_back(talkScript);
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

		for (TalkScript& talkScript : m_aTalkScripts)
		{
			struct DiffInfo
			{
				int32_t m_iDiff;
				size_t m_iEndOfInstr;
			};
			std::map<std::size_t, DiffInfo> diffByOffset;
			for (TalkScriptInfo& talkInfo : talkScript.GetTalks())
			{
				std::string oldTalkRef = std::to_string(talkInfo.m_iTalkOffset) + "," + std::to_string(talkInfo.m_iTalkSize);

				parsing::Chunk* talkChunk = talkInfo.m_pTalkChunk;
				std::string newTalkRef = std::to_string(talkChunk->GetOffsetFromRoot()) + "," + std::to_string(talkChunk->WholeChunkSize());

				int32_t diff = static_cast<int32_t>(newTalkRef.size()) - static_cast<int32_t>(oldTalkRef.size());
				diffByOffset[talkInfo.m_iInstrTell] = { diff, talkInfo.m_iInstrEnd };
			}

			parsing::Chunk* chunk = talkScript.GetChunk();
			assert(chunk);
			if (!chunk)
			{
				core::Log(core::LogLevel::Error, "Could not build scripts: Script was null.");
				return false;
			}

			size_t tell = GetStartOfByteCode(chunk);
			core::Data data = chunk->GetData();

			while (tell < data.size())
			{
				size_t instrTell = tell;

				const unsigned char* pureDat = data.dataAs<unsigned char>() + tell;
				uint8_t code = *pureDat;

				auto it = m_mOPCodeMap.find(code);

				assert(it != m_mOPCodeMap.end());
				if (it == m_mOPCodeMap.end())
				{
					core::Log(core::LogLevel::Error, "Could not bind scripts: Script encountered invalid byte code.");
					return false;
				}

				std::vector<script::ArgInfo> args = it->second.GetSizeFn()(code, pureDat + 1);

				size_t size = sizeof(code);

				size_t instrEnd = tell + sizeof(code);
				for (const script::ArgInfo& arg : args)
				{
					instrEnd += arg.m_iSize;
				}

				for (const script::ArgInfo& arg : args)
				{
					unsigned char* argData = data.dataAs<unsigned char>() + tell + size;
					
					if (arg.m_eArgumentType == script::ScrArgumentType::Ref)
					{
						int32_t jump{};

						if (arg.m_iSize == sizeof(int16_t))
						{
							jump = static_cast<int16_t>(
								static_cast<uint16_t>(argData[0]) |
								(static_cast<uint16_t>(argData[1]) << 8)
								);
						}
						else
						{
							jump = static_cast<int32_t>(
								static_cast<uint32_t>(argData[0]) |
								(static_cast<uint32_t>(argData[1]) << 8) |
								(static_cast<uint32_t>(argData[2]) << 16) |
								(static_cast<uint32_t>(argData[3]) << 24)
							);
						}

						const int32_t endOfArgument =
							static_cast<int32_t>(tell) +
							static_cast<int32_t>(size) +
							static_cast<int32_t>(arg.m_iSize);

						size_t jumpTo = static_cast<size_t>(endOfArgument + jump);
					
						for (auto& diffOffset : diffByOffset)
						{
							// Case 1: before -> after.
							if (endOfArgument <= static_cast<int32_t>(diffOffset.second.m_iEndOfInstr) && jumpTo > static_cast<int32_t>(diffOffset.second.m_iEndOfInstr))
							{
								jump += diffOffset.second.m_iDiff;
							}
							// Case 2: after -> before.
							else if (endOfArgument > static_cast<int32_t>(diffOffset.second.m_iEndOfInstr) && jumpTo <= static_cast<int32_t>(diffOffset.second.m_iEndOfInstr))
							{
								jump -= diffOffset.second.m_iDiff;
							}
						}

						if (arg.m_iSize == sizeof(int16_t))
						{
							int16_t jump16 = static_cast<int16_t>(jump);
							memcpy(argData, &jump16, sizeof(jump16));
						}
						else
						{
							memcpy(argData, &jump, sizeof(jump));
						}
					}

					size += arg.m_iSize;
				}

				// Go to next arg.
				tell += size;
			}

			





		}



		return true;
	}
}