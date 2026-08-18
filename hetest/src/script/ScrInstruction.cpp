#include "./ScrInstruction.h"

#include "script/ScrArgument.h"

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	// ScrInstruction
	//---------------------------------------------------------------------
	ScrInstruction::ScrInstruction(uint8_t a_iByteCode) :
		m_iByteCode(a_iByteCode)
	{ }
	
	//---------------------------------------------------------------------
	size_t ScrInstruction::GetSize() const
	{
		size_t size = 0;
		for (const ScrArgument& argument : m_aArguments)
		{
			size += argument.GetData().size();
		}
		return size;
	}

	//---------------------------------------------------------------------
	uint8_t ScrInstruction::GetByteCode() const
	{
		return m_iByteCode;
	}
}
