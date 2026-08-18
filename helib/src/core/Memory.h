#pragma once

// external
#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <cstring>

namespace humongousexplorer::core
{
	constexpr size_t KB(size_t a_Size)
	{
		return a_Size * 1024;
	}

	constexpr size_t MB(size_t a_Size)
	{
		return a_Size * 1024 * 1024;
	}

	constexpr size_t _64KB = KB(64);
	constexpr size_t _1MB = MB(1);
	constexpr size_t _2MB = MB(2);
	constexpr size_t _4MB = MB(4);
	constexpr size_t _8MB = MB(8);
	constexpr size_t _16MB = MB(16);
	constexpr size_t _32MB = MB(32);
	constexpr size_t _64MB = MB(64);
	constexpr size_t _128MB = MB(128);
	constexpr size_t _256MB = MB(256);

	//---------------------------------------------------------------------
	inline std::string SizeToString(size_t a_Size)
	{
		if (a_Size >= _1MB)
			return std::to_string(a_Size / _1MB) + "MB";

		if (a_Size >= KB(1))
			return std::to_string(a_Size / KB(1)) + "KB";

		return std::to_string(a_Size) + "B";
	}

	/// <summary>
	/// Adds specific size to a pointer.
	/// </summary>
	/// <param name="a_pPtr">The pointer that will be offset.</param>
	/// <param name="a_iSize">The amount that the pointer will be offset with.</param>
	/// <returns>A pointer with the new offset.</returns>
	inline void* add(void* a_pPtr, size_t a_iSize)
	{
		return static_cast<void*>(static_cast<unsigned char*>(a_pPtr) + a_iSize);
	}

	/// <summary>
	/// Adds specific size to a pointer.
	/// </summary>
	/// <param name="a_pPtr">The pointer that will be offset.</param>
	/// <param name="a_iSize">The amount that the pointer will be offset with.</param>
	/// <returns>A pointer with the new offset.</returns>
	inline const void* add(const void* a_pPtr, size_t a_iSize)
	{
		return static_cast<const void*>(static_cast<const unsigned char*>(a_pPtr) + a_iSize);
	}

	/// <summary>
	/// Adds specific size to a pointer and casts it to a specified type.
	/// </summary>
	/// <typeparam name="T">The type the pointer will be cast to.</typeparam>
	/// <param name="a_pPtr">The pointer that will be offset.</param>
	/// <param name="a_iSize">The amount that the pointer will be offset with.</param>
	/// <returns>A pointer of type T with the new offset.</returns>
	template<typename T>
	inline T* addAs(T* a_pPtr, size_t a_iSize)
	{
		return reinterpret_cast<T*>(add(a_pPtr, a_iSize));
	}

	//---------------------------------------------------------------------
	inline int chunkcmp(const std::string_view& a_ChunkID1, const std::string_view& a_ChunkID2, size_t a_iSize = 4)
	{
		return std::strncmp(a_ChunkID1.data(), a_ChunkID2.data(), a_iSize);
	}

	//---------------------------------------------------------------------
	inline void reverseBytes(unsigned char* a_Start, size_t a_Size)
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
	inline void xorShift(unsigned char*& a_sData, size_t a_iSize, char a_cKey)
	{
		for (size_t i = 0; i < a_iSize; i++)
		{
			a_sData[i] = a_sData[i] ^ a_cKey;
		}
	}

	//---------------------------------------------------------------------
	inline uint32_t ReadBE32(const unsigned char* a_pData)
	{
		return (uint32_t(a_pData[0]) << 24) | (uint32_t(a_pData[1]) << 16) |
			(uint32_t(a_pData[2]) << 8) | uint32_t(a_pData[3]);
	}

	//---------------------------------------------------------------------
	inline void WriteBE32(unsigned char* a_pData, uint32_t a_iValue)
	{
		a_pData[0] = static_cast<unsigned char>(a_iValue >> 24);
		a_pData[1] = static_cast<unsigned char>(a_iValue >> 16);
		a_pData[2] = static_cast<unsigned char>(a_iValue >> 8);
		a_pData[3] = static_cast<unsigned char>(a_iValue);
	}
}