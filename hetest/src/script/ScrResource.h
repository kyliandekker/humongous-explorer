#pragma once

#include <vector>
#include <memory>

#include "script/OPCodesHE.h"

namespace humongousexplorer::core
{
	class Data;
	class DataStream;
}
namespace humongousexplorer::script
{
	class ScrInstruction;

	class ScrResource
	{
	public:
		std::vector<std::unique_ptr<ScrInstruction>>& GetInstructions();
		const std::vector<std::unique_ptr<ScrInstruction>>& GetInstructions() const;

		bool Parse(const core::Data& a_Data, const OPCodeMap& a_mScrCodes);
		bool Build(core::DataStream& a_Data, const OPCodeMap& a_mScrCodes);

		size_t GetScriptSize() const;

		ScrInstruction* GetInstructionAtOffset(size_t a_iOffset);
	private:
		bool CheckJumpValidity(const ScrInstruction& a_Instruction, const OPCodeMap& a_mScrCodes, size_t a_iCmdPos);

		std::vector<std::unique_ptr<ScrInstruction>> m_aInstructions;
	};
}