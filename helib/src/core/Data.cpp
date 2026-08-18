#include "Data.h"

// standard
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace humongousexplorer::core
{
	//---------------------------------------------------------------------
	// Data
	//---------------------------------------------------------------------
	Data::Data(const void* a_pData, size_t a_iSize)
		: Data(reinterpret_cast<const char*>(a_pData), a_iSize)
	{
	}

	//---------------------------------------------------------------------
	Data::Data(const char* a_pData, size_t a_iSize)
		: m_pData(nullptr)
		, m_iSize(a_iSize)
	{
		assert(a_iSize > 0);

		if (m_iSize > 0)
		{
			m_pData = std::malloc(m_iSize);
		}

		if (m_pData && a_pData && m_iSize > 0)
		{
			std::memcpy(m_pData, a_pData, m_iSize);
		}
	}

	//---------------------------------------------------------------------
	Data::Data(size_t a_iSize)
		: m_pData(nullptr)
		, m_iSize(a_iSize)
	{
		assert(a_iSize > 0);

		if (m_iSize > 0)
		{
			m_pData = std::malloc(m_iSize);

			if (m_pData)
			{
				std::memset(m_pData, 0, m_iSize);
			}
		}
	}

	//---------------------------------------------------------------------
	Data::Data(const Data& a_Rhs)
		: m_pData(nullptr)
		, m_iSize(a_Rhs.m_iSize)
	{
		if (m_iSize > 0)
		{
			m_pData = std::malloc(m_iSize);

			if (m_pData && a_Rhs.m_pData)
			{
				std::memcpy(m_pData, a_Rhs.m_pData, m_iSize);
			}
		}
	}

	//---------------------------------------------------------------------
	Data::Data(Data&& a_Other) noexcept
		: m_pData(a_Other.m_pData)
		, m_iSize(a_Other.m_iSize)
	{
		a_Other.m_pData = nullptr;
		a_Other.m_iSize = 0;
	}

	//---------------------------------------------------------------------
	Data& Data::operator=(const Data& a_Other)
	{
		if (this != &a_Other)
		{
			Free();

			m_iSize = a_Other.m_iSize;

			if (m_iSize > 0)
			{
				m_pData = std::malloc(m_iSize);
			}

			if (m_pData && a_Other.m_pData)
			{
				std::memcpy(m_pData, a_Other.m_pData, m_iSize);
			}
		}

		return *this;
	}

	//---------------------------------------------------------------------
	Data& Data::operator=(Data&& a_Other) noexcept
	{
		if (this != &a_Other)
		{
			Free();

			m_pData = a_Other.m_pData;
			m_iSize = a_Other.m_iSize;

			a_Other.m_pData = nullptr;
			a_Other.m_iSize = 0;
		}

		return *this;
	}

	//---------------------------------------------------------------------
	Data::~Data()
	{
		Free();
	}

	//---------------------------------------------------------------------
	void Data::Free()
	{
		if (m_pData)
		{
			std::free(m_pData);
		}

		m_pData = nullptr;
		m_iSize = 0;
	}

	//---------------------------------------------------------------------
	void Data::Clear()
	{
		if (m_pData && m_iSize > 0)
		{
			std::memset(m_pData, 0, m_iSize);
		}
	}

	//---------------------------------------------------------------------
	unsigned char Data::operator[](size_t a_iIndex) const
	{
		assert(a_iIndex < m_iSize);
		return reinterpret_cast<unsigned char*>(m_pData)[a_iIndex];
	}

	//---------------------------------------------------------------------
	unsigned char& Data::operator[](size_t a_iIndex)
	{
		assert(a_iIndex < m_iSize);
		return reinterpret_cast<unsigned char*>(m_pData)[a_iIndex];
	}
}