#include "ScrArgument.h"

#include <cassert>

#include "core/Data.h"
#include "script/ScrArgumentType.h"

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

	//---------------------------------------------------------------------
	void ScrArgument::SetData(const core::Data& a_Data)
	{
		m_Data = a_Data;
	}

	//---------------------------------------------------------------------
	void ScrArgument::SetByte(uint8_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int8_t));
		assert(m_eArgumentType == ScrArgumentType::Byte);
		m_Data = core::Data(&a_iValue, sizeof(uint8_t));
	}

	//---------------------------------------------------------------------
	void ScrArgument::SetInt16(int16_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int16_t));
		assert(m_eArgumentType == ScrArgumentType::Int16 || m_eArgumentType == ScrArgumentType::Ref);
		m_Data = core::Data(&a_iValue, sizeof(int16_t));
	}

	//---------------------------------------------------------------------
	void ScrArgument::SetInt32(int32_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int32_t));
		assert(m_eArgumentType == ScrArgumentType::Int32 || m_eArgumentType == ScrArgumentType::Ref);
		m_Data = core::Data(&a_iValue, sizeof(int32_t));
	}

	//---------------------------------------------------------------------
	void ScrArgument::SetString(const std::string& a_sString)
	{
		assert(m_eArgumentType == ScrArgumentType::String);
		m_Data = core::Data(a_sString.c_str(), a_sString.size() + 1);
	}
}