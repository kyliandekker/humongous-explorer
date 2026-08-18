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
	private:
		core::Data m_Data;
		ScrArgumentType m_eArgumentType;
	};
}