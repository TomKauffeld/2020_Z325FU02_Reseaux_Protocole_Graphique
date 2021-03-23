#ifndef __HC_HAMMINGCODE_H__
#define __HC_HAMMINGCODE_H__
#include "HammingCodeApi.h"


typedef unsigned char parityt;

const parityt parity_even = 0ui8;
const parityt parity_odd  = 1ui8;

const unsigned char hamming_3_1 = 2ui8;
const unsigned char hamming_7_4 = 3ui8;
const unsigned char hamming_15_11 = 4ui8;
const unsigned char hamming_31_26 = 5ui8;
const unsigned char hamming_63_57 = 6ui8;
const unsigned char hamming_127_120 = 7ui8;
const unsigned char hamming_255_247 = 8ui8;

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
HC_API unsigned long long hc_encode_bytes(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits);

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
HC_API unsigned long long hc_decode_bytes(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits);

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
HC_API unsigned long long hc_encode(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits);

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
HC_API unsigned long long hc_decode(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parityt parity, unsigned char parity_bits);

#endif