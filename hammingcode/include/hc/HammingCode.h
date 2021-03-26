#ifndef __HC_HAMMINGCODE_H__
#define __HC_HAMMINGCODE_H__
#include "HammingCodeApi.h"

typedef unsigned char parity_t;
typedef unsigned char hc_mode_t;
typedef signed char hc_status_t;

#define parity_even 0ui8
#define parity_odd  1ui8

#define hc_mode_sec 0ui8
#define hc_mode_secded 1ui8

#define hamming_3_1 2ui8
#define hamming_7_4 3ui8
#define hamming_15_11 4ui8
#define hamming_31_26 5ui8
#define hamming_63_57 6ui8
#define hamming_127_120 7ui8
#define hamming_255_247 8ui8


#define hc_status_info_buffer_too_small 1
#define hc_status_ok 0
#define hc_status_error_unknown -1
#define hc_status_error_overflow -2
#define hc_status_error_invalid_parity -3
#define hc_status_error_too_many_flips -4
#define hc_status_error_invalid_nb_parity_bits -5
#define hc_status_error_invalid_mode -6

typedef struct hammingEncodeResult {
	hc_status_t status;
	unsigned long long bytes;
	unsigned long long bits;
	unsigned long long blocks;
	unsigned long long n;
	unsigned long long k;
} HammingEncodeResult_t;

typedef struct hammingDecodeResult {
	hc_status_t status;
	unsigned long long bytes;
	unsigned long long bits;
	unsigned long long blocks;
	unsigned long long n;
	unsigned long long k;
	unsigned long long errors;
	unsigned long long corrected;
} HammingDecodeResult_t;

/// <summary>
/// Encodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to encode</param>
/// <param name="data_size">the size of the data array in bytes</param>
/// <param name="buffer">the buffer array to put the encoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bytes</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>a structure containing information about the encoded data</returns>
HC_API HammingEncodeResult_t hc_encode_bytes(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parity_t parity, unsigned char parity_bits, hc_mode_t hc_mode);

/// <summary>
/// Decodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to decode</param>
/// <param name="data_size">the size of the data array in bytes</param>
/// <param name="buffer">the buffer array to put the decoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bytes</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>a structure containing information about the decoded data</returns>
HC_API HammingDecodeResult_t hc_decode_bytes(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parity_t parity, unsigned char parity_bits, hc_mode_t hc_mode);

/// <summary>
/// Encodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to encode</param>
/// <param name="data_size">the size of the data array in bits</param>
/// <param name="buffer">the buffer array to put the encoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bits</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>a structure containing information about the encoded data</returns>
HC_API HammingEncodeResult_t hc_encode(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parity_t parity, unsigned char parity_bits, hc_mode_t hc_mode);

/// <summary>
/// Decodes the data into the buffer using the given parameters
/// </summary>
/// <param name="data">the data array to decode</param>
/// <param name="data_size">the size of the data array in bits</param>
/// <param name="buffer">the buffer array to put the decoded data into</param>
/// <param name="buffer_size">the size of the buffer array in bits</param>
/// <param name="parity">the parity to use (even: 0 or odd: 1)</param>
/// <param name="parity_bits">the number of parity bits to use per block</param>
/// <returns>a structure containing information about the decoded data</returns>
HC_API HammingDecodeResult_t hc_decode(const void* data, unsigned long long data_size, void* buffer, unsigned long long buffer_size, parity_t parity, unsigned char parity_bits, hc_mode_t hc_mode);

#endif