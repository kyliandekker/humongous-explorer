#include "./ScrArgument.h"

#include "core/Data.h"

namespace humongousexplorer::script
{
	//---------------------------------------------------------------------
	// ScrArgument
	//---------------------------------------------------------------------
	ScrArgument::ScrArgument(const core::Data& a_Data, ScrArgumentType a_eArgumentType) :
		m_Data(a_Data),
		m_eArgumentType(a_eArgumentType)
	{ }

	//---------------------------------------------------------------------
	ScrArgumentType ScrArgument::GetArgumentType() const
	{
		return m_eArgumentType;
	}

	//---------------------------------------------------------------------
	const core::Data& ScrArgument::GetData() const
	{
		return m_Data;
	}

	//---------------------------------------------------------------------
	const std::string ScrArgument::GetString() const
	{
		return std::string(m_Data.dataAs<const char>(), m_Data.size());
	}

	//---------------------------------------------------------------------
	int32_t ScrArgument::GetRefJump() const
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
}