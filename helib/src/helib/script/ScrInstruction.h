#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <utility>

#include <helib/script/ScrArgument.h>

namespace humongousexplorer::script
{
	class ScrInstruction
	{
	public:
		ScrInstruction(uint8_t a_iByteCode);

		size_t GetArgumentsSize() const;
		size_t GetSize() const;

		void AddArgument(std::unique_ptr<ScrArgument>& a_aArguments);
		std::vector<std::unique_ptr<ScrArgument>>& GetArguments();

		uint8_t GetByteCode() const;
	private:
		uint8_t m_iByteCode = 0;
		std::vector<std::unique_ptr<ScrArgument>> m_aArguments;
	};
}