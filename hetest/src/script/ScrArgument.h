#pragma once

#include <string>

#include "core/Data.h"

namespace humongousexplorer::script
{
	enum class ScrArgumentType;
	
	class ScrArgument
	{
	public:
		ScrArgument() = default;
		ScrArgument(const core::Data& a_Data, ScrArgumentType a_eArgumentType);

		ScrArgumentType GetArgumentType() const;
		const core::Data& GetData() const;
		
		const std::string GetString() const;
		int32_t GetRefJump() const;
		
		void SetData(const core::Data& a_Data);
		void SetByte(uint8_t a_iValue);
		void SetInt16(int16_t a_iValue);
		void SetInt32(int32_t a_iValue);
		void SetString(const std::string& a_sString);
	private:
		core::Data m_Data;
		ScrArgumentType m_eArgumentType;
	};
}