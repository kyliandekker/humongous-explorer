#include "ScrInstruction.h"

#include <helib/script/ScrArgument.h>

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	// ScrInstruction
	//---------------------------------------------------------------------
	ScrInstruction::ScrInstruction(uint8_t a_iByteCode) :
		m_iByteCode(a_iByteCode)
	{ }

	//---------------------------------------------------------------------
	size_t ScrInstruction::GetArgumentsSize() const
	{
		size_t size = 0;
		for (const std::unique_ptr<ScrArgument>& argument : m_aArguments)
		{
			size += argument->GetData().size();
		}
		return size;
	}

	//---------------------------------------------------------------------
	size_t ScrInstruction::GetSize() const
	{
		return GetArgumentsSize() + 1;
	}

	//---------------------------------------------------------------------
	void ScrInstruction::AddArgument(std::unique_ptr<ScrArgument>& a_aArguments)
	{
		m_aArguments.push_back(std::move(a_aArguments));
	}

	//---------------------------------------------------------------------
	std::vector<std::unique_ptr<ScrArgument>>& ScrInstruction::GetArguments()
	{
		return m_aArguments;
	}

	//---------------------------------------------------------------------
	uint8_t ScrInstruction::GetByteCode() const
	{
		return m_iByteCode;
	}
}
