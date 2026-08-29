#pragma once

// external
#include <cstdint>
#include <string>

namespace humongousexplorer::core
{
	/// <summary>
	/// Converts a size value to kilobytes.
	/// </summary>
	/// <param name="a_Size">The number of kilobytes to convert.</param>
	/// <returns>The equivalent size in bytes.</returns>
	constexpr size_t KB(size_t a_Size)
	{
		return a_Size * 1024;
	}

	/// <summary>
	/// Converts a size value to megabytes.
	/// </summary>
	/// <param name="a_Size">The number of megabytes to convert.</param>
	/// <returns>The equivalent size in bytes.</returns>
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

	/// <summary>
	/// Converts a byte size to a human-readable string with B, KB, or MB suffix.
	/// </summary>
	/// <param name="a_Size">The size in bytes.</param>
	/// <returns>A string representation of the size (e.g., "512KB", "2MB").</returns>
	std::string SizeToString(size_t a_Size);

	/// <summary>
	/// Adds specific size to a pointer.
	/// </summary>
	/// <param name="a_pPtr">The pointer that will be offset.</param>
	/// <param name="a_iSize">The amount that the pointer will be offset with.</param>
	/// <returns>A pointer with the new offset.</returns>
	void* add(void* a_pPtr, size_t a_iSize);

	/// <summary>
	/// Adds specific size to a pointer.
	/// </summary>
	/// <param name="a_pPtr">The pointer that will be offset.</param>
	/// <param name="a_iSize">The amount that the pointer will be offset with.</param>
	/// <returns>A pointer with the new offset.</returns>
	const void* add(const void* a_pPtr, size_t a_iSize);

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

	/// <summary>
	/// Reverses the byte order of a memory region in place.
	/// </summary>
	/// <param name="a_Start">Pointer to the start of the byte array.</param>
	/// <param name="a_Size">The number of bytes to reverse.</param>
	void reverseBytes(unsigned char* a_Start, size_t a_Size);

	/// <summary>
	/// XORs each byte in the data buffer with the given key.
	/// </summary>
	/// <param name="a_sData">Reference to the pointer to the byte array to XOR.</param>
	/// <param name="a_iSize">The number of bytes to process.</param>
	/// <param name="a_cKey">The XOR key byte.</param>
	void xorShift(unsigned char*& a_sData, size_t a_iSize, char a_cKey);

	/// <summary>
	/// Reads a big-endian 32-bit unsigned integer from a byte array.
	/// </summary>
	/// <param name="a_pData">Pointer to the 4-byte big-endian value.</param>
	/// <returns>The decoded 32-bit unsigned integer in native byte order.</returns>
	uint32_t ReadBE32(const unsigned char* a_pData);

	/// <summary>
	/// Writes a 32-bit unsigned integer as big-endian bytes into a byte array.
	/// </summary>
	/// <param name="a_pData">Pointer to the destination byte array (at least 4 bytes).</param>
	/// <param name="a_iValue">The 32-bit value to encode in big-endian format.</param>
	void WriteBE32(unsigned char* a_pData, uint32_t a_iValue);

	/// <summary>
	/// Reads a little-endian 16-bit unsigned integer from a byte array.
	/// </summary>
	/// <param name="a_pData">Pointer to the 2-byte little-endian value.</param>
	/// <returns>The decoded 16-bit unsigned integer in native byte order.</returns>
	uint16_t ReadLE16(const unsigned char* a_pData);

	/// <summary>
	/// Reads a little-endian 32-bit unsigned integer from a byte array.
	/// </summary>
	/// <param name="a_pData">Pointer to the 4-byte little-endian value.</param>
	/// <returns>The decoded 32-bit unsigned integer in native byte order.</returns>
	uint32_t ReadLE32(const unsigned char* a_pData);

	/// <summary>
	/// Writes a 16-bit unsigned integer as little-endian bytes into a byte array.
	/// </summary>
	/// <param name="a_pData">Pointer to the destination byte array (at least 2 bytes).</param>
	/// <param name="a_iValue">The 16-bit value to encode in little-endian format.</param>
	void WriteLE16(unsigned char* a_pData, uint16_t a_iValue);

	/// <summary>
	/// Writes a 32-bit unsigned integer as little-endian bytes into a byte array.
	/// </summary>
	/// <param name="a_pData">Pointer to the destination byte array (at least 4 bytes).</param>
	/// <param name="a_iValue">The 32-bit value to encode in little-endian format.</param>
	void WriteLE32(unsigned char* a_pData, uint32_t a_iValue);
}