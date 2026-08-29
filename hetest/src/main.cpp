#include <cassert>

#include <helib/archive/ArchiveSet.h>
#include <helib/core/DataStream.h>
#include <helib/core/Log.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkIDs.h>
#include <helib/resources/resources/TalkResource.h>
#include <helib/building/HE4Builder.h>
#include <helib/building/HE0Builder.h>
#include <helib/building/ScriptBuilder.h>

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
//
////---------------------------------------------------------------------
//struct TalkInstruction
//{
//	uint8_t bytecode = -1;
//	int32_t instructionIndex = -1;
//	int32_t argumentIndex = -1;
//	size_t argumentOffsetInScript = 0;
//	size_t instructionOffsetInScript = 0;
//	std::vector<TalkInstructionCall> talkCalls;
//};
//
////---------------------------------------------------------------------
//bool TryFindTalkInstructionCalls(const script::ScrArgument& a_Argument, std::vector<TalkInstructionCall>& a_aCalls)
//{
//	const humongousexplorer::core::Data& data = a_Argument.GetData();
//	std::string talk = a_Argument.GetString();
//
//	for (size_t i = 0; i < data.size(); i++)
//	{
//		// Find talkie terminator and T.
//		if (data[i] == 0x7F && i + 1 < data.size() && data[i + 1] == 0x54)
//		{
//			size_t startPos = i + 2;
//			size_t endTalkie = data.size();
//
//			// Find talkie terminator within bounds.
//			for (size_t j = startPos; j < data.size(); j++)
//			{
//				if (data[j] == 0x7F)
//				{
//					endTalkie = j;
//					break;
//				}
//			}
//
//			// Find last comma between startPos and endTalkie.
//			int32_t commaPos = -1;
//			for (size_t j = startPos; j < endTalkie; j++)
//			{
//				if (data[j] == ',')
//				{
//					commaPos = static_cast<int32_t>(j);
//				}
//			}
//
//			TalkInstructionCall call;
//			call.offsetTalkiePos = startPos;
//			call.offsetEndTalkieSize = endTalkie;
//
//			call.debugStr = talk.substr(i, endTalkie + 1);
//			call.talkiePosVal = std::stoi(talk.substr(startPos, commaPos));
//			call.talkieSizeVal = std::stoi(talk.substr(commaPos + 1, endTalkie));
//
//			a_aCalls.push_back(call);
//
//			i = endTalkie;
//		}
//	}
//
//	return !a_aCalls.empty();
//}
//
////---------------------------------------------------------------------
//std::vector<TalkInstruction> GetTalkInstructions(const script::ScrResource& script)
//{
//	std::vector<TalkInstruction> instructions;
//
//	size_t instructionOffsetInScript = 0;
//	size_t argumentOffsetInScript = 0;
//
//	size_t instructionIndex = 0;
//	size_t argumentIndex = 0;
//	for (const auto& instruction : script.GetInstructions())
//	{
//		argumentOffsetInScript += sizeof(instruction->GetByteCode());
//
//		argumentIndex = 0;
//		for (const auto& argument : instruction->GetArguments())
//		{
//			if (argument.GetArgumentType() == script::ScrArgumentType::String)
//			{
//				std::vector<TalkInstructionCall> calls;
//				if (TryFindTalkInstructionCalls(argument, calls))
//				{
//					TalkInstruction talkInstruction;
//					talkInstruction.bytecode = instruction->GetByteCode();
//					talkInstruction.instructionIndex = instructionIndex;
//					talkInstruction.argumentIndex = argumentIndex;
//					talkInstruction.argumentOffsetInScript = argumentOffsetInScript;
//					talkInstruction.instructionOffsetInScript = instructionOffsetInScript;
//					talkInstruction.talkCalls = calls;
//
//					instructions.push_back(talkInstruction);
//				}
//			}
//			argumentIndex++;
//
//			argumentOffsetInScript += argument.GetData().size();
//		}
//
//		instructionOffsetInScript += instruction->GetSize();
//
//		instructionIndex++;
//	}
//
//	return instructions;
//}

//---------------------------------------------------------------------
int main()
{
	core::InitializeLog();

	fs::path archivesPath = "C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2";

	archive::ArchiveSet set;
	bool loaded = set.LoadArchives(archivesPath);

	if (!loaded)
	{
		core::DestroyLog();
		return 1;
	}

	archive::Archive* he4 = nullptr;
	archive::Archive* he2 = nullptr;
	archive::Archive* he0 = nullptr;
	archive::Archive* a = nullptr;
	for (std::unique_ptr<archive::Archive>& archive : set.GetArchives())
	{
		if (archive->GetType() == archive::ArchiveType::HE4)
		{
			he4 = archive.get();
		}
		else if (archive->GetType() == archive::ArchiveType::HE2)
		{
			he2 = archive.get();
		}
		else if (archive->GetType() == archive::ArchiveType::HE0)
		{
			he0 = archive.get();
		}
		else if (archive->GetType() == archive::ArchiveType::A)
		{
			a = archive.get();
		}
	}

	if (!he4)
	{
		return 0;
	}

	building::HE4Builder he4Builder;
	if (!he4Builder.Bind(set))
	{
		core::Log(core::LogLevel::Error, "Could not bind HE4.");
		core::DestroyLog();
		return 0;
	}

	building::ScriptBuilder scriptBuilder;
	if (!scriptBuilder.Bind(set))
	{
		core::Log(core::LogLevel::Error, "Could not bind scripts.");
		core::DestroyLog();
		return 0;
	}

	building::HE0Builder he0Builder;
	if (!he0Builder.Bind(set))
	{
		core::Log(core::LogLevel::Error, "Could not bind HE0.");
		core::DestroyLog();
		return 0;
	}

	// do something in between.

	// First HE4. It does not change anything in HE0 or (A).
	if (!he4Builder.Build())
	{
		core::Log(core::LogLevel::Error, "Could not build HE4.");
		core::DestroyLog();
		return 0;
	}

	// Then scripts. This changes (A) and HE0.
	if (!scriptBuilder.Build())
	{
		core::Log(core::LogLevel::Error, "Could not build scripts.");
		core::DestroyLog();
		return 0;
	}

	// Last, build the HE0 again because all the changes in scripts, (A), HE2 are done.
	if (!he0Builder.Build())
	{
		core::Log(core::LogLevel::Error, "Could not build HE0.");
		core::DestroyLog();
		return 0;
	}

	fs::path newArchiveFolderPath = archivesPath.parent_path().string() + "/build";
	file::CreateDirectory(newArchiveFolderPath);
	for (std::unique_ptr<archive::Archive>& archive : set.GetArchives())
	{
		core::DataStream data;
		archive->Build(data);
		fs::path newArchivePath = newArchiveFolderPath.string() + "/" + archive->GetName();
		file::SaveFile(newArchivePath, data);
	}

	// SCRIPT

	//script::OPCodeMap map;
	//script::GetOPCodeTable(map, set.GetScriptVersion(), set.GetHEVersion());

	//std::vector<parsing::Chunk*> scripts;
	//a->GetRoot().TryFindChildren({
	//	parsing::SCRP_CHUNK_ID,
	//	parsing::LSCR_CHUNK_ID,
	//	parsing::LSC2_CHUNK_ID,
	//	parsing::ENCD_CHUNK_ID,
	//	parsing::EXCD_CHUNK_ID,
	//	parsing::VERB_CHUNK_ID,
	//}, scripts);

	//for (parsing::Chunk* chunk : scripts)
	//{
	//	// LOAD AND PARSE.
	//	script::ScrResource script(chunk);
	//	if (!script.Parse(chunk, map))
	//	{
	//		core::Log(core::LogLevel::Error, "Could not parse script data: \"" + archivesPath.string() + "\".");
	//		core::DestroyLog();
	//		auto test = GetTalkInstructions(script);
	//		for (auto& talk : test)
	//		{
	//			assert(talk.bytecode == 0xBA);
	//		}
	//		return 0;
	//	}
	//}

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
