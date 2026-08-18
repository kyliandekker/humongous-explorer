#pragma once

#include <cstdint>
#include <vector>
#include <utility>

#include "script/ScrArgument.h"

namespace humongousexplorer::script
{
	class ScrInstruction
	{
	public:
		ScrInstruction(uint8_t a_iByteCode);

		template<typename... Args>
		void AddArgument(Args&&... args)
		{
			m_aArguments.emplace_back(std::forward<Args>(args)...);
		}

		size_t GetSize() const;

		const std::vector<ScrArgument>& GetArguments() const
		{
			return m_aArguments;
		}

		std::vector<ScrArgument>& GetArguments()
		{
			return m_aArguments;
		}

		uint8_t GetByteCode() const;
	private:
		uint8_t m_iByteCode = 0;
		std::vector<ScrArgument> m_aArguments;
	};
}