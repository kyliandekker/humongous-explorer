#include "./DataStream.h"

// standard
#include <cstring>
#include <utility>

// core
#include <helib/core/Memory.h>

namespace humongousexplorer::core
{
	//---------------------------------------------------------------------
	// DataStream
	//---------------------------------------------------------------------
	DataStream::DataStream(void* a_pData, size_t a_iSize)
		: Data(a_pData, a_iSize)
	{
	}

	//---------------------------------------------------------------------
	DataStream::DataStream(const char* a_pData, size_t a_iSize)
		: Data(a_pData, a_iSize)
	{
	}

	//---------------------------------------------------------------------
	DataStream::DataStream(size_t a_iSize)
		: Data(a_iSize)
	{
	}

	//---------------------------------------------------------------------
	DataStream::DataStream(const DataStream& a_Rhs)
		: Data(a_Rhs)
		, m_iPos(a_Rhs.m_iPos)
	{
	}

	//---------------------------------------------------------------------
	DataStream::DataStream(const Data& a_Rhs)
		: Data(a_Rhs)
		, m_iPos(0)
	{
	}

	//---------------------------------------------------------------------
	DataStream& DataStream::operator=(const DataStream& a_Other)
	{
		if (this != &a_Other)
		{
			Data::operator=(a_Other);
			m_iPos = a_Other.m_iPos;
		}

		return *this;
	}

	//---------------------------------------------------------------------
	DataStream::DataStream(DataStream&& a_Other) noexcept
		: Data(std::move(a_Other))
		, m_iPos(a_Other.m_iPos)
	{
		a_Other.m_iPos = 0;
	}

	//---------------------------------------------------------------------
	DataStream& DataStream::operator=(DataStream&& a_Other) noexcept
	{
		if (this != &a_Other)
		{
			Data::operator=(std::move(a_Other));

			m_iPos = a_Other.m_iPos;
			a_Other.m_iPos = 0;
		}

		return *this;
	}

	//---------------------------------------------------------------------
	void DataStream::Free()
	{
		Data::Free();
		m_iPos = 0;
	}

	//---------------------------------------------------------------------
	bool DataStream::Read(void* a_Data, size_t a_DataSize, size_t a_Size)
	{
		if (!a_Data || a_DataSize == 0 || a_Size == 0)
		{
			return false;
		}

		if (a_DataSize > m_iSize / a_Size)
		{
			return false;
		}

		const size_t totalSize = a_DataSize * a_Size;

		if (m_iPos > m_iSize - totalSize)
		{
			return false;
		}

		std::memcpy(a_Data, addAs(m_pData, m_iPos), totalSize);
		Seek(totalSize, SEEK_CUR);

		return true;
	}

	//---------------------------------------------------------------------
	bool DataStream::Write(void const* a_Data, size_t a_Size)
	{
		if (!a_Data || a_Size == 0)
		{
			return false;
		}

		if (m_iPos > m_iSize - a_Size)
		{
			return false;
		}

		std::memcpy(addAs(m_pData, m_iPos), a_Data, a_Size);
		Seek(a_Size, SEEK_CUR);

		return true;
	}

	//---------------------------------------------------------------------
	bool DataStream::ReadLE16(uint16_t& a_Value)
	{
		unsigned char buf[2];
		if (!Read(buf, sizeof(buf), 1))
		{
			return false;
		}
		a_Value = ::humongousexplorer::core::ReadLE16(buf);
		return true;
	}

	//---------------------------------------------------------------------
	bool DataStream::ReadLE32(uint32_t& a_Value)
	{
		unsigned char buf[4];
		if (!Read(buf, sizeof(buf), 1))
		{
			return false;
		}
		a_Value = ::humongousexplorer::core::ReadLE32(buf);
		return true;
	}

	//---------------------------------------------------------------------
	bool DataStream::WriteLE16(uint16_t a_Value)
	{
		unsigned char buf[2];
		::humongousexplorer::core::WriteLE16(buf, a_Value);
		return Write(buf, sizeof(buf));
	}

	//---------------------------------------------------------------------
	bool DataStream::WriteLE32(uint32_t a_Value)
	{
		unsigned char buf[4];
		::humongousexplorer::core::WriteLE32(buf, a_Value);
		return Write(buf, sizeof(buf));
	}

	//---------------------------------------------------------------------
	bool DataStream::Seek(int64_t a_Offset, int a_Whence)
	{
		int64_t newPos = 0;

		switch (a_Whence)
		{
			case SEEK_SET:
			{
				if (a_Offset < 0 || static_cast<uint64_t>(a_Offset) > m_iSize)
				{
					return false;
				}
				newPos = a_Offset;
				break;
			}

			case SEEK_CUR:
			{
				newPos = static_cast<int64_t>(m_iPos) + a_Offset;
				if (newPos < 0 || static_cast<uint64_t>(newPos) > m_iSize)
				{
					return false;
				}
				break;
			}

			case SEEK_END:
			{
				newPos = static_cast<int64_t>(m_iSize) + a_Offset;
				if (newPos < 0 || static_cast<uint64_t>(newPos) > m_iSize)
				{
					return false;
				}
				break;
			}

			default:
			{
				return false;
			}
		}

		m_iPos = static_cast<size_t>(newPos);
		return true;
	}

	//---------------------------------------------------------------------
	size_t DataStream::Tell() const
	{
		return m_iPos;
	}
}