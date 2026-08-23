#include "Memory.h"

// external
#include <cstdint>
#include <cstring>

namespace humongousexplorer::core
{
	//---------------------------------------------------------------------
	std::string SizeToString(size_t a_Size)
	{
		if (a_Size >= _1MB)
		{
			return std::to_string(a_Size / _1MB) + "MB";
		}

		if (a_Size >= KB(1))
		{
			return std::to_string(a_Size / KB(1)) + "KB";
		}

		return std::to_string(a_Size) + "B";
	}

	//---------------------------------------------------------------------
	void* add(void* a_pPtr, size_t a_iSize)
	{
		return static_cast<void*>(static_cast<unsigned char*>(a_pPtr) + a_iSize);
	}

	//---------------------------------------------------------------------
	const void* add(const void* a_pPtr, size_t a_iSize)
	{
		return static_cast<const void*>(static_cast<const unsigned char*>(a_pPtr) + a_iSize);
	}

	//---------------------------------------------------------------------
	void reverseBytes(unsigned char* a_Start, size_t a_Size)
	{
		if (!a_Start || a_Size < 2)
		{
			return;
		}

		unsigned char* lo = a_Start;
		unsigned char* hi = a_Start + a_Size - 1;

		while (lo < hi)
		{
			unsigned char temp = *lo;
			*lo++ = *hi;
			*hi-- = temp;
		}
	}

	//---------------------------------------------------------------------
	void xorShift(unsigned char*& a_sData, size_t a_iSize, char a_cKey)
	{
		for (size_t i = 0; i < a_iSize; i++)
		{
			a_sData[i] = a_sData[i] ^ a_cKey;
		}
	}

	//---------------------------------------------------------------------
	uint32_t ReadBE32(const unsigned char* a_pData)
	{
		return (uint32_t(a_pData[0]) << 24) | (uint32_t(a_pData[1]) << 16) |
			(uint32_t(a_pData[2]) << 8) | uint32_t(a_pData[3]);
	}

	//---------------------------------------------------------------------
	void WriteBE32(unsigned char* a_pData, uint32_t a_iValue)
	{
		a_pData[0] = static_cast<unsigned char>(a_iValue >> 24);
		a_pData[1] = static_cast<unsigned char>(a_iValue >> 16);
		a_pData[2] = static_cast<unsigned char>(a_iValue >> 8);
		a_pData[3] = static_cast<unsigned char>(a_iValue);
	}
}