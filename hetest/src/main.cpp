#include "core/Log.h"
#include "archive/ArchiveSet.h"

#include "script/OPCodesHE.h"
#include "script/ScrResource.h"
#include "script/ScrArgumentType.h"

#include "core/DataStream.h"

#include "parsing/Chunk.h"
#include "parsing/ChunkIDs.h"

using namespace humongousexplorer;

//---------------------------------------------------------------------
struct TalkInstructionCall
{
	size_t talkiePosVal;
	size_t talkieSizeVal;

	size_t offsetTalkiePos;
	size_t offsetEndTalkieSize;

	std::string debugStr;
};

//---------------------------------------------------------------------
struct TalkInstruction
{
	int32_t instructionIndex = -1;
	int32_t argumentIndex = -1;
	size_t size = 0;
	size_t argumentOffsetInScript = 0;
	size_t instructionOffsetInScript = 0;
	std::vector<TalkInstructionCall> talkCalls;
};

//---------------------------------------------------------------------
bool TryFindTalkInstructionCalls(const script::ScrArgument& a_Argument, std::vector<TalkInstructionCall>& a_aCalls)
{
	const core::Data& data = a_Argument.GetData();
	std::string talk = a_Argument.GetString();

	for (size_t i = 0; i < data.size(); i++)
	{
		// Find talkie terminator and T.
		if (data[i] == 0x7F && i + 1 < data.size() && data[i + 1] == 0x54)
		{
			size_t startPos = i + 2;
			size_t endTalkie = data.size();

			// Find talkie terminator within bounds.
			for (size_t j = startPos; j < data.size(); j++)
			{
				if (data[j] == 0x7F)
				{
					endTalkie = j;
					break;
				}
			}

			// Find last comma between startPos and endTalkie.
			int32_t commaPos = -1;
			for (size_t j = startPos; j < endTalkie; j++)
			{
				if (data[j] == ',')
				{
					commaPos = static_cast<int32_t>(j);
				}
			}

			TalkInstructionCall call;
			call.offsetTalkiePos = startPos;
			call.offsetEndTalkieSize = endTalkie;

			call.debugStr = talk.substr(i, endTalkie + 1);
			call.talkiePosVal = std::stoi(talk.substr(startPos, commaPos));
			call.talkieSizeVal = std::stoi(talk.substr(commaPos + 1, endTalkie));

			a_aCalls.push_back(call);

			i = endTalkie;
		}
	}

	return !a_aCalls.empty();
}

//---------------------------------------------------------------------
std::vector<TalkInstruction> GetTalkInstructions(const script::ScrResource& script)
{
	std::vector<TalkInstruction> instructions;

	size_t instructionOffsetInScript = 0;
	size_t argumentOffsetInScript = 0;

	size_t instructionIndex = 0;
	size_t argumentIndex = 0;
	for (const auto& instruction : script.GetInstructions())
	{
		argumentOffsetInScript += sizeof(instruction->GetByteCode());

		argumentIndex = 0;
		for (const auto& argument : instruction->GetArguments())
		{
			if (argument.GetArgumentType() == script::ScrArgumentType::String)
			{
				std::vector<TalkInstructionCall> calls;
				if (TryFindTalkInstructionCalls(argument, calls))
				{
					TalkInstruction talkInstruction;
					talkInstruction.instructionIndex = instructionIndex;
					talkInstruction.argumentIndex = argumentIndex;
					talkInstruction.size = argument.GetData().size();
					talkInstruction.argumentOffsetInScript = argumentOffsetInScript;
					talkInstruction.instructionOffsetInScript = instructionOffsetInScript;
					talkInstruction.talkCalls = calls;

					instructions.push_back(talkInstruction);
				}
			}
			argumentIndex++;

			argumentOffsetInScript += argument.GetData().size();
		}

		instructionOffsetInScript += instruction->GetSize();

		instructionIndex++;
	}

	return instructions;
}

//---------------------------------------------------------------------
int main()
{
	core::InitializeLog();

	std::string archivesPath = "C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2";

	archive::ArchiveSet set;
	bool loaded = set.LoadArchives(archivesPath);

	if (!loaded)
	{
		core::DestroyLog();
		return 1;
	}

	core::Data data;
	if (!file::LoadFile(archivesPath, data))
	{
		core::Log(core::LogLevel::Error, "Could not find file: \"" + archivesPath + "\".");
		core::DestroyLog();
		return 0;
	}

	script::OPCodeMap map;
	script::GetOPCodeTable(map, set.GetScriptVersion(), set.GetHEVersion());

	// Determine (A).
	archive::Archive* a = nullptr;
	for (const std::unique_ptr<archive::Archive>& archiveEntry : set.GetArchives())
	{
		if (archiveEntry->GetType() == archive::ArchiveType::A)
		{
			a = archiveEntry.get();
			break;
		}
	}

	if (!a)
	{
		core::Log(core::LogLevel::Error, "Could not find (a): \"" + archivesPath + "\".");
		core::DestroyLog();
		return 0;
	}

	std::vector<parsing::Chunk*> scripts;
	a->GetRoot().TryFindChildren({
		parsing::SCRP_CHUNK_ID,
		parsing::LSCR_CHUNK_ID,
		parsing::LSC2_CHUNK_ID,
		parsing::ENCD_CHUNK_ID,
		parsing::EXCD_CHUNK_ID,
		parsing::VERB_CHUNK_ID,
	}, scripts);

	for (parsing::Chunk* chunk : scripts)
	{
		// LOAD AND PARSE.
		script::ScrResource script;
		if (!script.Parse(chunk, map))
		{
			core::Log(core::LogLevel::Error, "Could not parse script data: \"" + archivesPath + "\".");
			core::DestroyLog();
			return 0;
		}
	}

	// REPLACE TEST
	//const std::vector<TalkInstruction> talks = GetTalkInstructions(script);
	//script::ScrInstruction* instruction = script.GetInstruction(talk.instructionIndex);
	//script::ScrArgument& argument = instruction->GetArgument(talk.argumentIndex);

	//std::string newString = "hello";
	//int32_t difSize = (newString.size() + 1) - argument.GetData().size();

	//script.UpdateJumps(talk.instructionOffsetInScript, talk.argumentOffsetInScript, talk.instructionIndex, talk.argumentIndex, difSize);
	//int32_t difSize2 = argument.SetString(newString);

	// BUILD TEST
	//core::DataStream newData;
	//if (!script.Build(newData, map))
	//{
	//	core::Log(core::LogLevel::Error, "Could not build script data.");
	//	core::DestroyLog();
	//	return 0;
	//}

	//std::string saveTestDataPath = "./testdata/test.test";
	//if (!file::SaveFile(saveTestDataPath, newData))
	//{
	//	core::Log(core::LogLevel::Error, "Could not save script file: \"" + saveTestDataPath + "\".");
	//	core::DestroyLog();
	//	return 0;
	//}

	//core::Log(core::LogLevel::Success, "Parsed, built and saved script file: \"" + saveTestDataPath + "\".");

	core::DestroyLog();

	return 0;
}
