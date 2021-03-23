#ifndef __INTERNAL__HC_HAMMINGCODE_H__
#define __INTERNAL__HC_HAMMINGCODE_H__
#include <hc/HammingCode.h>



/// <summary>
/// Reads a bit from the data array
/// </summary>
/// <param name="data">the data array to read from</param>
/// <param name="size">the size of the data array in bits</param>
/// <param name="index">the index of the bit to read</param>
/// <returns>the bit read</returns>
unsigned char hc_read_data(const void* data, unsigned long long size, unsigned long long index);

/// <summary>
/// Write a bit ti the data array
/// </summary>
/// <param name="data">the data array to write to</param>
/// <param name="size">the size of the data array in bits</param>
/// <param name="index">the index of the bit to write</param>
/// <param name="bit">the bit to write</param>
/// <returns>0 if successfull, -1 if the data array is too small</returns>
int hc_write_data(void* data, unsigned long long size, unsigned long long index, unsigned char byte);

#endif