#include "./ScriptBuilder.h"

#include <cassert>
#include <map>

#include <helib/archive/Archive.h>
#include <helib/archive/ArchiveSet.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/core/Memory.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/script/ScriptArgType.h>

namespace humongousexplorer::building
{
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

		size_t offsetInStr;
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
					ref.strSize = posStr.size() + sizeof(',') + sizeStr.size();
					ref.offsetInStr = a;
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
		m_pA = nullptr;
		m_pHE2 = nullptr;
		m_aTalkScripts.clear();
		m_mOPCodeMap.clear();

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

		// Cache TALK chunks in a map beforehand for faster lookup.
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

			size_t startTell = GetStartOfByteCode(chunk);
			core::Data data = chunk->GetData();

			Script script;
			script.m_pChunk = chunk;
			bool isTalkScript = false;

			{
				size_t tell = startTell;

				while (tell < data.size())
				{
					const unsigned char* pureDat = data.dataAs<unsigned char>() + tell;
					uint8_t code = *pureDat;

					auto it = m_mOPCodeMap.find(code);

					assert(it != m_mOPCodeMap.end());
					if (it == m_mOPCodeMap.end())
					{
						core::Log(core::LogLevel::Error, "Could not bind scripts: Script encountered invalid byte code.");
						return false;
					}

					std::unique_ptr<ScriptInstruction> instruction = std::make_unique<ScriptInstruction>();
					instruction->m_iCode = code;
					if (script.m_aInstructions.size() > 0)
					{
						ScriptInstruction* prevInstruction = script.m_aInstructions[script.m_aInstructions.size() - 1].get();
						instruction->m_pPrevious = prevInstruction;
						prevInstruction->m_pNext = instruction.get();
					}

					std::vector<script::ArgInfo> args = it->second.GetSizeFn()(code, pureDat + 1);

					size_t size = sizeof(code);

					for (const script::ArgInfo& argInfo : args)
					{
						const unsigned char* argData = data.dataAs<unsigned char>() + tell + size;
						instruction->m_aArgs.push_back({ core::Data(argData, argInfo.m_iSize), argInfo.m_eArgumentType, instruction.get() });

						if (argInfo.m_eArgumentType == script::ScriptArgType::String)
						{
							// So contrary to popular belief, talkActor is not the only code for calling TALKies.
							// These are known to call TALKs as of right now:
							//	* o72_getScriptString: By far the most calls.
							//	* o72_talkActor: Second-most calls.
							//	* o72_talkEgo: Third-most calls.
							//	* o6_printLine: Really only occasionally, but still a significant amount.
							//	* o6_printActor: Weirdly few calls.
							std::vector<TalkRef> talks = GetTalkRefs(core::Data(argData, argInfo.m_iSize));
							if (!talks.empty())
							{
								isTalkScript = true;
							}
						}

						size += argInfo.m_iSize;
					}

					script.m_aInstructions.push_back(std::move(instruction));

					// Go to next arg.
					tell += size;
				}
			}

			if (isTalkScript)
			{
				std::unordered_map<std::size_t, ScriptInstruction*> instructionsTable;
				size_t tell = 0;
				for (std::unique_ptr<ScriptInstruction>& instruction : script.m_aInstructions)
				{
					instructionsTable[tell] = instruction.get();
					tell += instruction->GetSize();
				}

				for (std::unique_ptr<ScriptInstruction>& instruction : script.m_aInstructions)
				{
					for (ScriptArg& arg : instruction->m_aArgs)
					{
						if (arg.m_eArgumentType == script::ScriptArgType::String)
						{
							std::vector<TalkRef> talks = GetTalkRefs(arg.m_Data);
							for (const TalkRef& talkRef : talks)
							{
								auto talkIt = talkChunkTable.find(talkRef.pos);

								assert(talkIt != talkChunkTable.end());
								if (talkIt == talkChunkTable.end())
								{
									core::Log(core::LogLevel::Error, "Could not bind scripts: Script referenced invalid TALK chunk.");
									return false;
								}

								parsing::Chunk* referencedTALK = talkIt->second;

								assert(referencedTALK->WholeChunkSize() == talkRef.size);
								if (referencedTALK->WholeChunkSize() != talkRef.size)
								{
									core::Log(core::LogLevel::Error, "Could not bind scripts: Referenced TALK chunk in script was not the same size.");
									return false;
								}

								arg.m_pTALKChunk = referencedTALK;
							}
						}
						else if (arg.m_eArgumentType == script::ScriptArgType::Ref)
						{
							int32_t jumpSize = arg.GetRefJump();

							int32_t endOfArgumentPos = arg.GetOffsetFromInstruction() + instruction->GetOffsetFromFirstInstruction() + arg.m_Data.size();
							int32_t jumpTo = endOfArgumentPos + jumpSize;

							auto instructionIt = instructionsTable.find(static_cast<size_t>(jumpTo));

							assert(instructionIt != instructionsTable.end());
							if (instructionIt == instructionsTable.end())
							{
								core::Log(core::LogLevel::Error, "Could not bind scripts: Script jumped to unknown bytecode.");
								return false;
							}

							arg.m_pJumpTo = instructionIt->second;
						}
					}
				}

				m_aTalkScripts.push_back(std::move(script));
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

		// Cache TALK chunks in a map beforehand for faster lookup.
		std::unordered_map<std::size_t, parsing::Chunk*> talkChunkTable;
		std::vector<parsing::Chunk*> talkChunks;
		m_pHE2->GetRoot().TryFindChildren(parsing::TALK_CHUNK_ID, talkChunks);
		for (parsing::Chunk* talkChunk : talkChunks)
		{
			talkChunkTable[talkChunk->GetOffsetFromRoot()] = talkChunk;
		}

		for (Script& script : m_aTalkScripts)
		{
			for (std::unique_ptr<ScriptInstruction>& instruction : script.m_aInstructions)
			{
				for (ScriptArg& arg : instruction->m_aArgs)
				{
					if (arg.m_eArgumentType == script::ScriptArgType::String)
					{
						std::vector<TalkRef> talks = GetTalkRefs(arg.m_Data);
						for (auto it = talks.rbegin(); it != talks.rend(); ++it)
						{
							const TalkRef& talkRef = *it;

							parsing::Chunk* talkChunk = arg.m_pTALKChunk;
							std::string newTalkRef = std::to_string(talkChunk->GetOffsetFromRoot()) + "," + std::to_string(talkChunk->WholeChunkSize());

							size_t restOfData = arg.m_Data.size() - (talkRef.offsetInStr + talkRef.strSize);
							core::DataStream newData(talkRef.offsetInStr + newTalkRef.size() + restOfData);
							newData.Write(arg.m_Data.data(), talkRef.offsetInStr);
							newData.Write(newTalkRef.data(), newTalkRef.size());
							newData.Write(arg.m_Data.dataAs<unsigned char>() + talkRef.offsetInStr + talkRef.strSize, restOfData);

							arg.SetData(newData);
						}
					}
				}
			}
		}

		for (Script& script : m_aTalkScripts)
		{
			std::unordered_map<std::size_t, ScriptInstruction*> instructionsTable;
			size_t tell = 0;
			for (std::unique_ptr<ScriptInstruction>& instruction : script.m_aInstructions)
			{
				instructionsTable[tell] = instruction.get();
				tell += instruction->GetSize();
			}

			for (std::unique_ptr<ScriptInstruction>& instruction : script.m_aInstructions)
			{
				for (ScriptArg& arg : instruction->m_aArgs)
				{
					if (arg.m_eArgumentType == script::ScriptArgType::Ref)
					{
						int32_t jumpSize = arg.GetRefJump();

						int32_t endOfArgumentPos = arg.GetOffsetFromInstruction() + instruction->GetOffsetFromFirstInstruction() + arg.m_Data.size();

						int32_t jumpTo = endOfArgumentPos + jumpSize;
						size_t actualOffset = arg.m_pJumpTo->GetOffsetFromFirstInstruction();
						if (jumpTo != actualOffset)
						{
							int32_t newJumpSize = actualOffset - endOfArgumentPos;
							arg.SetRefJump(newJumpSize);
						}
					}
				}
			}

			size_t size = script.GetSize();
			size_t startTell = GetStartOfByteCode(script.m_pChunk);

			core::DataStream newData(size + startTell);
			if (startTell > 0)
			{
				newData.Write(script.m_pChunk->GetData().data(), startTell);
			}

			core::DataStream scriptData;
			script.ToData(newData);

			script.m_pChunk->SetData(newData);
		}

		return true;
	}
}