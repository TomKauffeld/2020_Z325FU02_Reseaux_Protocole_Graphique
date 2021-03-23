#include "HammingCode.h"
#include <string.h>

/// <summary>
/// Reads a bit from the data array
/// </summary>
/// <param name="data">the data array to read from</param>
/// <param name="size">the size of the data array in bits</param>
/// <param name="index">the index of the bit to read</param>
/// <returns>the bit read</returns>
unsigned char hc_read_data(const void* data, unsigned long long size, unsigned long long index)
{
	unsigned long long i;
	unsigned char b;
	if (index >= size)
		return 0ui8;
	i = index / 8ui64;
	b = 1 << (index % 8ui8);
	if ((((const unsigned char*)data)[i] & b) > 0)
		return 1ui8;
	return 0ui8;
}

/// <summary>
/// Write a bit ti the data array
/// </summary>
/// <param name="data">the data array to write to</param>
/// <param name="size">the size of the data array in bits</param>
/// <param name="index">the index of the bit to write</param>
/// <param name="bit">the bit to write</param>
/// <returns>0 if successfull, -1 if the data array is too small</returns>
int hc_write_data(void* data, unsigned long long size, unsigned long long index, unsigned char bit)
{
	unsigned long long i;
	unsigned char b;
	if (index >= size)
		return -1;
	i = index / 8ui64;
	b = 1 << (index % 8ui8);
	if (bit)
		((unsigned char*)data)[i] |= b;
	else
		((unsigned char*)data)[i] &= ~b;
	return 0;
}

/// <summary>
/// Encodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to encode</param>
/// <param name="data_size">the size of the data array in bytes</param>
/// <param name="buffer">the buffer array to put the encoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bytes</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>the length of the encoded data in bytes</returns>
unsigned long long hc_encode_bytes(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits)
{
	unsigned long long s, i;
	if (data_size * 8 < data_size || buffer_size * 8 < buffer_size)
		return 0;
	s = hc_encode(data, data_size * 8, buffer, buffer_size * 8, parity, parity_bits);
	for (i = s; i % 8 > 0; ++i)
		hc_write_data(buffer, buffer_size * 8, i, 0);
	if (s % 8 == 0)
		return s / 8;
	return s / 8 + 1;
}

/// <summary>
/// Decodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to decode</param>
/// <param name="data_size">the size of the data array in bytes</param>
/// <param name="buffer">the buffer array to put the decoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bytes</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>the length of the decoded data in bytes</returns>
unsigned long long hc_decode_bytes(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits)
{
	unsigned long long s, i;
	if (data_size * 8 < data_size || buffer_size * 8 < buffer_size)
		return 0;
	s = hc_decode(data, data_size * 8, buffer, buffer_size * 8, parity, parity_bits);
	for (i = s; i % 8 > 0; ++i)
		hc_write_data(buffer, buffer_size * 8, i, 0);
	if (s % 8 == 0)
		return s / 8;
	return s / 8 + 1;
}

/// <summary>
/// Encodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to encode</param>
/// <param name="data_size">the size of the data array in bits</param>
/// <param name="buffer">the buffer array to put the encoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bits</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>the length of the encoded data in bits</returns>
unsigned long long hc_encode(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits)
{
	unsigned long long di = 0, bi = 0, i, n;
	unsigned char b, pi;
	unsigned char p[sizeof(unsigned long long) * 8 - 1];
	if (parity_bits < 2 || (parity != parity_even && parity != parity_odd) || data_size < 1 || parity_bits > sizeof(unsigned long long) * 8 - 1)
		return 0;
	n = (1ui64 << parity_bits) - 1;
	while (di < data_size)
	{
		memset(p, parity, sizeof(unsigned long long) * 8 - 1);
		for (i = 0; i < n; ++i)
		{
			if (((i + 1) & i) == 0)
				b = 0ui8;
			else
				b = hc_read_data(data, data_size, di++);
			hc_write_data(buffer, buffer_size, bi + i, b);
			for (pi = 0; pi < parity_bits; ++pi)
				if (((1ui64 << pi) & (i + 1)) > 0)
					p[pi] ^= b;
		}
		for (pi = 0; pi < parity_bits; ++pi)
			hc_write_data(buffer, buffer_size, bi + (1ui64 << pi) - 1, p[pi]);
		bi += n;
	}
	if (bi > buffer_size)
		return buffer_size;
	return bi;
}

/// <summary>
/// Decodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to decode</param>
/// <param name="data_size">the size of the data array in bits</param>
/// <param name="buffer">the buffer array to put the decoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bits</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>the length of the decoded data in bits</returns>
unsigned long long hc_decode(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits)
{
	unsigned long long di = 0, bi = 0, i, n, e;
	unsigned char b, pi;
	unsigned char p[sizeof(unsigned long long) * 8 - 1];
	if (parity_bits < 2 || (parity != parity_even && parity != parity_odd) || data_size < 1 || parity_bits > sizeof(unsigned long long) * 8 - 1)
		return 0;
	if (data_size * 8 < data_size || buffer_size * 8 < buffer_size)
		return 0;
	n = (1ui64 << parity_bits) - 1;
	while (di < data_size)
	{
		memset(p, parity, sizeof(unsigned long long) * 8 - 1);
		for (i = 0; i < n; ++i)
		{
			b = hc_read_data(data, data_size, di + i);
			for (pi = 0; pi < parity_bits; ++pi)
				if (((1ui64 << pi) & (i + 1)) > 0)
					p[pi] ^= b;
		}
		e = 0;
		for (pi = 0; pi < parity_bits; ++pi)
			if (p[pi] > 0)
				e += 1ui64 << pi;
		for (i = 0; i < n; ++i)
		{
			b = hc_read_data(data, data_size, di + i);
			if (i + 1 == e)
				b ^= 1ui8;
			if (((i + 1) & i) != 0)
				hc_write_data(buffer, buffer_size, bi++, b);
		}
		di += n;
	}
	if (bi > buffer_size)
		return buffer_size;
	return bi;
}
