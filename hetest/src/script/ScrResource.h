#pragma once

#include <vector>
#include <memory>

#include "script/OPCodesHE.h"

namespace humongousexplorer::core
{
	class Data;
	class DataStream;
}
namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::script
{
	class ScrInstruction;

	class ScrResource
	{
	public:
		std::vector<std::unique_ptr<ScrInstruction>>& GetInstructions();
		const std::vector<std::unique_ptr<ScrInstruction>>& GetInstructions() const;

		bool Parse(parsing::Chunk* a_pChunk, const OPCodeMap& a_mScrCodes);
		bool Build(core::DataStream& a_Data, const OPCodeMap& a_mScrCodes);

		size_t GetScriptSize() const;

		ScrInstruction* GetInstruction(size_t a_iIndex);
		ScrInstruction* GetInstructionAtOffset(size_t a_iOffset);

		void UpdateJumps(size_t a_iInstructionOffsetInScript, size_t a_iArgumentOffsetInScript, size_t a_iIndex, size_t a_iArgumentIndex, int32_t a_iDifference);
	private:
		bool CheckJumpValidity(const ScrInstruction& a_Instruction, const OPCodeMap& a_mScrCodes, size_t a_iCmdPos);

		std::vector<std::unique_ptr<ScrInstruction>> m_aInstructions;
		parsing::Chunk* m_pChunk = nullptr;
	};
}