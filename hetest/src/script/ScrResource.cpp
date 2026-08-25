#include "ScrResource.h"

#include <cassert>

#include <helib/core/DataStream.h>
#include <helib/parsing/Chunk.h>
#include <helib/parsing/ChunkIDs.h>

#include "script/ScrArgument.h"
#include "script/ScrInstruction.h"
#include "script/ScrArgumentType.h"
#include "script/OPCodesHE.h"

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	// ScrResource
	//---------------------------------------------------------------------
	ScrResource::ScrResource(parsing::Chunk* a_pChunk) : m_pChunk(a_pChunk)
	{}

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
	bool ScrResource::Parse(parsing::Chunk* a_pChunk, const OPCodeMap& a_mScrCodes)
	{
		core::Data& data = a_pChunk->GetData();

		size_t tell = 0;
		if (
			a_pChunk->GetTag() == parsing::SCRP_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::ENCD_CHUNK_ID ||
			a_pChunk->GetTag() == parsing::EXCD_CHUNK_ID
		)
		{
			tell = 0;
		}
		else if (a_pChunk->GetTag() == parsing::LSCR_CHUNK_ID)
		{
			tell = 1;
		}
		else if (a_pChunk->GetTag() == parsing::LSC2_CHUNK_ID)
		{
			tell = 4;
		}
		else if (a_pChunk->GetTag() == parsing::VERB_CHUNK_ID)
		{
			tell = 0;
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
		}
		else
		{
			return false;
		}

		while (tell < data.size())
		{
			const unsigned char* pureDat = data.dataAs<unsigned char>() + tell;
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
	bool ScrResource::Rebuild(const OPCodeMap& a_mScrCodes)
	{
		size_t skipBytes = 0;
		if (
			m_pChunk->GetTag() == parsing::SCRP_CHUNK_ID ||
			m_pChunk->GetTag() == parsing::ENCD_CHUNK_ID ||
			m_pChunk->GetTag() == parsing::EXCD_CHUNK_ID
		)
		{
			skipBytes = 0;
		}
		else if (m_pChunk->GetTag() == parsing::LSCR_CHUNK_ID)
		{
			skipBytes = 1;
		}
		else if (m_pChunk->GetTag() == parsing::LSC2_CHUNK_ID)
		{
			skipBytes = 4;
		}
		else if (m_pChunk->GetTag() == parsing::VERB_CHUNK_ID)
		{
			skipBytes = 0;
			size_t dataSize = m_pChunk->GetData().size();
			while (skipBytes < dataSize)
			{
				uint8_t key = m_pChunk->GetData()[skipBytes];
				skipBytes += 1; // key byte
				if (key == 0x00)
				{
					break;
				}
				skipBytes += 2; // 2-byte offset
			}
		}
		else
		{
			return false;
		}

		size_t scriptSize = GetScriptSize();

		// Create the full script including skipped bytes.
		core::DataStream newData(skipBytes + scriptSize);
		if (!newData.Write(m_pChunk->GetData().data(), skipBytes))
		{
			return false;
		}

		// So for some reaon, some scripts in the game are completely empty.
		// No clue why, but this check is a safeguard.
		if (scriptSize > 0)
		{
			core::DataStream scriptData;
			if (!BuildScriptInstructions(scriptData, a_mScrCodes))
			{
				return false;
			}

			if (!newData.Write(scriptData.data(), scriptData.size()))
			{
				return false;
			}
		}

		m_pChunk->SetData(newData);
		return true;
	}

	//---------------------------------------------------------------------
	bool ScrResource::BuildScriptInstructions(core::DataStream& a_Data, const OPCodeMap& a_mScrCodes)
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
	ScrInstruction* ScrResource::GetInstruction(size_t a_iIndex)
	{
		return m_aInstructions[a_iIndex].get();
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
	void ScrResource::RecalculateJumps(size_t a_iInstructionOffsetInScript, size_t a_iArgumentOffsetInScript, size_t a_iIndex, size_t a_iArgumentIndex, int32_t a_iDiff)
	{
		if (a_iDiff == 0)
		{
			return;
		}

		const std::unique_ptr<ScrInstruction>& changedInstruction = m_aInstructions[a_iIndex];
		const ScrArgument& changedArgument = changedInstruction->GetArgument(a_iArgumentIndex);
		size_t endOfChangedArgument = a_iArgumentOffsetInScript + changedArgument.GetData().size();

		int64_t instructionPos = 0;
		for (std::unique_ptr<ScrInstruction>& instruction : m_aInstructions)
		{
			int64_t argumentPos = 0;
			argumentPos += sizeof(instruction->GetByteCode());

			for (ScrArgument& argument : instruction->GetArguments())
			{
				if (argument.GetArgumentType() == ScrArgumentType::Ref)
				{
					int32_t jumpArgPos = instructionPos + argumentPos;
					int32_t endOfArgument = jumpArgPos + static_cast<int32_t>(argument.GetData().size());
					int32_t jumpSize = argument.GetRefJump();
					int32_t jumpedTo = endOfArgument + jumpSize;

					// Case 1: before -> after.
					if (endOfArgument <= static_cast<int32_t>(endOfChangedArgument) && jumpedTo > static_cast<int32_t>(endOfChangedArgument))
					{
						argument.SetRefJump(jumpSize + a_iDiff);
					}
					// Case 2: after -> before.
					else if (endOfArgument > static_cast<int32_t>(endOfChangedArgument) && jumpedTo <= static_cast<int32_t>(endOfChangedArgument))
					{
						argument.SetRefJump(jumpSize - a_iDiff);
					}
				}
				argumentPos += argument.GetData().size();
			}

			instructionPos += instruction->GetSize();
		}
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