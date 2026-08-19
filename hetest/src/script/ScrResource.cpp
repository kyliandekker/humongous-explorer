#include "ScrResource.h"

#include <cassert>

#include "core/DataStream.h"

#include "script/ScrArgument.h"
#include "script/ScrInstruction.h"
#include "script/ScrArgumentType.h"
#include "script/OPCodesHE.h"

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	// ScrResource
	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrInstruction>>& ScrResource::GetInstructions()
	{
		return m_aInstructions;
	}

	//---------------------------------------------------------------------
	const std::vector<std::unique_ptr<ScrInstruction>>& ScrResource::GetInstructions() const
	{
		return m_aInstructions;
	}

	//---------------------------------------------------------------------
	bool ScrResource::Parse(const core::Data& a_Data, const OPCodeMap& a_mScrCodes)
	{
		// First parse the script and get instructions + arguments.
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

			m_aInstructions.push_back(it->second.m_fnSize(code, pureDat + 1));
			ScrInstruction& instruction = *m_aInstructions[m_aInstructions.size() - 1];

			size_t skip = instruction.GetArgumentsSize();
			tell += skip + 1;
		}

		// Afterwards check jump validity.
		size_t pos = 0;
		for (const std::unique_ptr<ScrInstruction>& instruction : m_aInstructions)
		{
			bool valid = CheckJumpValidity(*instruction, a_mScrCodes, pos);
			assert(valid);
			if (!valid)
			{
				return false;
			}

			pos += instruction->GetSize();
		}


		return true;
	}

	//---------------------------------------------------------------------
	bool ScrResource::Build(core::DataStream& a_Data, const OPCodeMap& a_mScrCodes)
	{
		a_Data = core::DataStream(GetScriptSize());

		size_t pos = 0;
		for (const std::unique_ptr<ScrInstruction>& instruction : m_aInstructions)
		{
			assert(CheckJumpValidity(*instruction, a_mScrCodes, pos));

			uint8_t byteCode = instruction->GetByteCode();
			a_Data.Write(&byteCode, sizeof(byteCode));
			for (const ScrArgument& argument : instruction->GetArguments())
			{
				a_Data.Write(argument.GetData().data(), argument.GetData().size());
			}
			pos += instruction->GetSize();
		}

		return true;
	}

	//---------------------------------------------------------------------
	size_t ScrResource::GetScriptSize() const
	{
		size_t size = 0;
		for (const std::unique_ptr<ScrInstruction>& instruction : m_aInstructions)
		{
			size += instruction->GetSize();
		}
		return size;
	}

	//---------------------------------------------------------------------
	ScrInstruction* ScrResource::GetInstructionAtOffset(size_t a_iOffset)
	{
		int64_t pos = 0;
		for (const std::unique_ptr<ScrInstruction>& instruction : m_aInstructions)
		{
			if (a_iOffset == pos)
			{
				return instruction.get();
			}
			pos += instruction->GetSize();
		}
		return nullptr;
	}

	//---------------------------------------------------------------------
	bool ScrResource::CheckJumpValidity(const ScrInstruction& a_Instruction, const OPCodeMap& a_mScrCodes, size_t a_iCmdPos)
	{
		size_t cmdSizeTillJump = sizeof(a_Instruction.GetByteCode());
		for (const ScrArgument& argument : a_Instruction.GetArguments())
		{
			if (argument.GetArgumentType() == ScrArgumentType::Ref)
			{
				int32_t endOfArgument = static_cast<int32_t>(a_iCmdPos) + static_cast<int32_t>(cmdSizeTillJump) + static_cast<int32_t>(argument.GetData().size()); // End of argument.

				int32_t jump = argument.GetRefJump();
				int32_t posInScript = endOfArgument + jump;

				// Check if the position is within the bounds of the script (cheap).
				if (posInScript < 0 || posInScript >= static_cast<int32_t>(GetScriptSize()))
				{
					return false;
				}

				// Get the (supposed) instruction at this byte offset. If it is null, it means that the offset was not valid.
				ScrInstruction* instruction = GetInstructionAtOffset(posInScript);
				if (!instruction)
				{
					return false;
				}

				// If there was an instruction, check if the byte code is valid.
				auto it = a_mScrCodes.find(instruction->GetByteCode());
				if (it == a_mScrCodes.end())
				{
					return false;
				}
			}
			cmdSizeTillJump += argument.GetData().size();
		}

		return true;
	}
}