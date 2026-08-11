#pragma once

// external
#include <cstdint>

namespace humongousexplorer::core
{
#define _KB(x) (x * 1024)
#define _MB(x) (x * 1024 * 1024)

#define _64KB _KB(64)
#define _1MB _MB(1)
#define _2MB _MB(2)
#define _4MB _MB(4)
#define _8MB _MB(8)
#define _16MB _MB(16)
#define _32MB _MB(32)
#define _64MB _MB(64)
#define _128MB _MB(128)
#define _256MB _MB(256)

	inline size_t NearestByteSize(size_t size)
	{
		static const size_t sizes[] = {
			_64KB,
			_1MB,
			_2MB,
			_4MB,
			_8MB,
			_16MB,
			_32MB,
			_64MB,
			_128MB,
			_256MB,
		};

		size_t nearest = sizes[0];
		size_t min_diff = (size > nearest) ? size - nearest : nearest - size;

		for (size_t i = 1; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
			size_t diff = (size > sizes[i]) ? size - sizes[i] : sizes[i] - size;

			if (diff < min_diff) {
				min_diff = diff;
				nearest = sizes[i];
			}
		}

		return nearest;
	}

	/// <summary>
	/// Adds specific size to a pointer.
	/// </summary>
	/// <param name="a_pPtr">The pointer that will be offset.</param>
	/// <param name="a_iSize">The amount that the pointer will be offset with.</param>
	/// <returns>A pointer with the new offset.</returns>
	inline void* add(void* a_pPtr, size_t a_iSize)
	{
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(a_pPtr) + a_iSize);
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

	inline int getBit(char i, size_t a_Position)
	{
		return (i & (1 << a_Position)) >> a_Position;
	}

	inline int chunkcmp(const char* a_ChunkID1, const char* a_ChunkID2, size_t a_iSize = 4)
	{
		return strncmp(a_ChunkID1, a_ChunkID2, a_iSize);
	}

	inline int unsignedCharCmp(unsigned char a_ChunkID1, unsigned char a_ChunkID2)
	{
		return a_ChunkID1 == a_ChunkID2;
	}

	inline void reverseBytes(unsigned char* a_Start, int a_Size)
	{
		unsigned char* lo = a_Start;
		unsigned char* hi = a_Start + a_Size - 1;
		unsigned char swap;
		while (lo < hi)
		{
			swap = *lo;
			*lo++ = *hi;
			*hi-- = swap;
		}
	}

	inline void xorShift(unsigned char*& a_Data, size_t a_Size, char a_Key)
	{
		for (int i = 0; i < a_Size; i++)
		{
			a_Data[i] = a_Data[i] ^ a_Key;
		}
	}

	template <class T>
	inline T reverseBytesC(unsigned char* a_Number)
	{
		reverseBytes(a_Number, sizeof(T));
		return *reinterpret_cast<T*>(a_Number);
	}
}