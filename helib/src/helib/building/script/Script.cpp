#include "./Script.h"

#include <cassert>

#include <helib/core/DataStream.h>

namespace humongousexplorer::building
{
	//---------------------------------------------------------------------
	// ScriptArg
	//---------------------------------------------------------------------
	const std::string ScriptArg::GetString() const
	{
		return std::string(m_Data.dataAs<const char>(), m_Data.size());
	}

	//---------------------------------------------------------------------
	int32_t ScriptArg::GetRefJump() const
	{
		if (m_Data.size() == sizeof(int32_t))
		{
			int32_t len =
				static_cast<int32_t>(m_Data[0]) |
				(static_cast<int32_t>(m_Data[1]) << 8) |
				(static_cast<int32_t>(m_Data[2]) << 16) |
				(static_cast<int32_t>(m_Data[3]) << 24);

			return static_cast<int32_t>(len);
		}
		else if (m_Data.size() == sizeof(int16_t))
		{
			int16_t len =
				static_cast<int16_t>(m_Data[0]) |
				(static_cast<int16_t>(m_Data[1]) << 8);

			return static_cast<int32_t>(len);
		}

		return 0;
	}

	//---------------------------------------------------------------------
	void ScriptArg::SetData(const core::Data& a_Data)
	{
		m_Data = a_Data;
	}

	//---------------------------------------------------------------------
	void ScriptArg::SetByte(uint8_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int8_t));
		assert(m_eArgumentType == script::ScriptArgType::Byte);
		m_Data = core::Data(&a_iValue, sizeof(uint8_t));
	}

	//---------------------------------------------------------------------
	void ScriptArg::SetInt16(int16_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int16_t));
		assert(m_eArgumentType == script::ScriptArgType::Int16 || m_eArgumentType == script::ScriptArgType::Ref);
		m_Data = core::Data(&a_iValue, sizeof(int16_t));
	}

	//---------------------------------------------------------------------
	void ScriptArg::SetInt32(int32_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int32_t));
		assert(m_eArgumentType == script::ScriptArgType::Int32 || m_eArgumentType == script::ScriptArgType::Ref);
		m_Data = core::Data(&a_iValue, sizeof(int32_t));
	}

	//---------------------------------------------------------------------
	int32_t ScriptArg::SetString(const std::string& a_sString)
	{
		int32_t oldSize = m_Data.size();
		assert(m_eArgumentType == script::ScriptArgType::String);
		m_Data = core::Data(a_sString.c_str(), a_sString.size() + 1);
		return static_cast<int32_t>(oldSize) - static_cast<int32_t>(m_Data.size());
	}

	//---------------------------------------------------------------------
	void ScriptArg::SetRefJump(int32_t a_iValue)
	{
		if (m_Data.size() == sizeof(int32_t))
		{
			m_Data = core::Data(&a_iValue, sizeof(int32_t));
		}
		else if (m_Data.size() == sizeof(int16_t))
		{
			int16_t val = static_cast<int16_t>(a_iValue);
			m_Data = core::Data(&val, sizeof(int16_t));
		}
	}

	//---------------------------------------------------------------------
	size_t ScriptArg::GetOffsetFromInstruction() const
	{
		size_t offset = sizeof(m_pOwnerInstruction->m_iCode);
		for (const ScriptArg& arg : m_pOwnerInstruction->m_aArgs)
		{
			if (&arg == this)
			{
				break;
			}
			offset += arg.m_Data.size();
		}
		return offset;
	}

	//---------------------------------------------------------------------
	// ScriptInstruction
	//---------------------------------------------------------------------
	size_t ScriptInstruction::GetDataSize() const
	{
		size_t size = 0;
		for (const ScriptArg& argument : m_aArgs)
		{
			size += argument.m_Data.size();
		}
		return size;
	}

	//---------------------------------------------------------------------
	size_t ScriptInstruction::GetSize() const
	{
		return GetDataSize() + sizeof(m_iCode);
	}

	//---------------------------------------------------------------------
	size_t ScriptInstruction::GetOffsetFromFirstInstruction() const
	{
		size_t offset = 0;
		const ScriptInstruction* current = m_pPrevious;
		while (current != nullptr)
		{
			offset += current->GetSize();
			current = current->m_pPrevious;
		}
		return offset;
	}

	//---------------------------------------------------------------------
	void ScriptInstruction::ToData(core::DataStream& a_Data) const
	{
		a_Data.Write(&m_iCode, sizeof(m_iCode));
		for (const ScriptArg& argument : m_aArgs)
		{
			a_Data.Write(argument.m_Data.data(), argument.m_Data.size());
		}
	}

	//---------------------------------------------------------------------
	// Script
	//---------------------------------------------------------------------
	ScriptInstruction* Script::GetInstructionAtOffset(size_t a_iStartingPoint)
	{
		size_t tell = a_iStartingPoint;
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			if (tell == a_iStartingPoint)
			{
				return instruction.get();
			}
			tell += instruction->GetSize();
		}
		return nullptr;
	}

	//---------------------------------------------------------------------
	size_t Script::GetSize() const
	{
		size_t size = 0;
		for (const std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			size += instruction->GetSize();
		}
		return size;
	}

	//---------------------------------------------------------------------
	void Script::ToData(core::DataStream& a_Data) const
	{
		for (const std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			core::DataStream instructionData(instruction->GetSize());
			instruction->ToData(instructionData);

			a_Data.Write(instructionData.data(), instructionData.size());
		}
	}
}