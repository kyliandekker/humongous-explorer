#include <cassert>

#include "core/Data.h"
#include "core/DataStream.h"

#include "script/OPCodesHE.h"
#include "script/ScrInstruction.h"
#include "script/ScrArgumentType.h"

#include "file/file.h"

using namespace humongousexplorer::script;
using namespace humongousexplorer;

//---------------------------------------------------------------------
bool CheckJumpValidity(const ScrInstruction& a_Instruction, const OPCodeMap& a_mScrCodes, size_t a_iCmdPos, const core::Data& a_Data)
{
	size_t cmdSizeTillJump = sizeof(a_Instruction.GetByteCode());
	for (const ScrArgument& argument : a_Instruction.GetArguments())
	{
		if (argument.GetArgumentType() == ScrArgumentType::Ref)
		{
			int32_t endOfArgument = static_cast<int32_t>(a_iCmdPos) + static_cast<int32_t>(cmdSizeTillJump) + static_cast<int32_t>(argument.GetData().size()); // End of argument.

			int32_t jump = argument.GetRefJump();
			int32_t posInScript = endOfArgument + jump;

			if (posInScript < 0 || posInScript >= static_cast<int32_t>(a_Data.size()))
			{
				return false;
			}

			auto it = a_mScrCodes.find(a_Data[posInScript]);
			if (it == a_mScrCodes.end())
			{
				return false;
			}
		}
		cmdSizeTillJump += argument.GetData().size();
	}

	return true;
}

//---------------------------------------------------------------------
bool ParseScript(const core::Data& a_Data, const OPCodeMap& a_mScrCodes, std::vector<std::unique_ptr<ScrInstruction>>& a_aInstructions)
{
	std::vector<std::string> instructionsStrings;

	size_t tell = 0;
	while (tell < a_Data.size())
	{
		const unsigned char* pureDat = a_Data.dataAs<unsigned char>() + tell;
		uint8_t code = *pureDat;

		auto it = a_mScrCodes.find(code);

		assert(it != a_mScrCodes.end());
		if (it == a_mScrCodes.end())
		{
			return false;
		}

		instructionsStrings.push_back(it->second.m_sName);

		a_aInstructions.push_back(it->second.m_fnSize(code, pureDat + 1));
		ScrInstruction& instruction = *a_aInstructions[a_aInstructions.size() - 1];

		assert(CheckJumpValidity(instruction, a_mScrCodes, tell, a_Data));

		size_t skip = instruction.GetArgumentsSize();
		tell += skip + 1;
	}

	return true;
}

//---------------------------------------------------------------------
bool BuildScript(core::DataStream& a_Data, const OPCodeMap& a_mScrCodes, const std::vector<std::unique_ptr<ScrInstruction>>& a_aInstructions)
{
	size_t totalSize = 0;
	for (const std::unique_ptr<ScrInstruction>& instruction : a_aInstructions)
	{
		size_t instructionSize = 0;
		instructionSize += sizeof(uint8_t); // Scr code.

		for (const ScrArgument& argument : instruction->GetArguments())
		{
			instructionSize += argument.GetData().size();
		}
		totalSize += instructionSize;
	}
	a_Data = core::DataStream(totalSize);

	size_t pos = 0;
	for (const std::unique_ptr<ScrInstruction>& instruction : a_aInstructions)
	{
		assert(CheckJumpValidity(*instruction, a_mScrCodes, pos, a_Data));

		uint8_t byteCode = instruction->GetByteCode();
		a_Data.Write(&byteCode, sizeof(byteCode));
		for (const ScrArgument& argument : instruction->GetArguments())
		{
			a_Data.Write(argument.GetData().data(), argument.GetData().size());
		}
		pos += instruction->GetArgumentsSize() + 1;
	}

	return true;
}

int main()
{
	core::Data data;
	if (!file::LoadFile("./testdata/test.data", data))
	{
		return 0;
	}

	OPCodeMap map;
	GetOPCodeTable(map, 6, 98);

	std::vector<std::unique_ptr<ScrInstruction>> instructions;
	if (!ParseScript(data, map, instructions))
	{
		return 0;
	}

	core::DataStream newData;
	BuildScript(newData, map, instructions);
	if (!file::SaveFile("./testdata/test2.test", newData))
	{
		return 0;
	}

	return 0;
}