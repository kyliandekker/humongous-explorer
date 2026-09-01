#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <helib/core/Data.h>
#include <helib/script/ScriptArgType.h>

namespace humongousexplorer::core
{
	class DataStream;
}
namespace humongousexplorer::parsing
{
	class Chunk;
}
namespace humongousexplorer::building
{
	class ScriptInstruction;

	//---------------------------------------------------------------------
	struct ScriptArg
	{
		const std::string GetString() const;
		int32_t GetRefJump() const;

		void SetData(const core::Data& a_Data);

		// Public setters and getters.
		void SetByte(uint8_t a_iValue);
		void SetInt16(int16_t a_iValue);
		void SetInt32(int32_t a_iValue);
		int32_t SetString(const std::string& a_sString);
		void SetRefJump(int32_t a_iValue);

		size_t GetOffsetFromInstruction() const;

		core::Data m_Data;
		script::ScriptArgType m_eArgumentType;
		ScriptInstruction* m_pOwnerInstruction = nullptr;

		parsing::Chunk* m_pTALKChunk = nullptr; // Optional.
		ScriptInstruction* m_pJumpTo = nullptr; // Optional.
	};

	//---------------------------------------------------------------------
	struct ScriptInstruction
	{
		uint8_t m_iCode = 0;
		ScriptInstruction* m_pPrevious = nullptr;
		ScriptInstruction* m_pNext = nullptr;
		std::vector<ScriptArg> m_aArgs;

		size_t GetSize() const;
		size_t GetDataSize() const;
		size_t GetOffsetFromFirstInstruction() const;
		void ToData(core::DataStream& a_Data) const;
	};

	//---------------------------------------------------------------------
	struct Script
	{
		parsing::Chunk* m_pChunk = nullptr;
		std::vector<std::unique_ptr<ScriptInstruction>> m_aInstructions;

		ScriptInstruction* GetInstructionAtOffset(size_t a_iStartingPoint);
		size_t GetSize() const;
		void ToData(core::DataStream& a_Data) const;
	};
}