#include <hc/HammingCode.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_hex(const char* prefix, const char d[], unsigned long long length)
{
	int i;
	printf("%s hex : ", prefix);
	for (i = 0; (d[i] != 0 && length == 0) || i < length; ++i)
		printf("%02x", d[i]);
	printf("00\n");
}
void print_bin(const char* prefix, const char d[], unsigned long long length)
{
	int i, j;
	printf("%s bin : ", prefix);
	for (i = 0; (d[i] != 0 && length == 0) || i < length; ++i)
		for (j = 0; j < 8; ++j)
			if ((d[i] & (1 << j)) > 0)
				printf("1");
			else
				printf("0");
	printf("00000000\n");
}
void print_all(const char* prefix, const char d[], unsigned long long length)
{
	print_hex(prefix, d, length);
	print_bin(prefix, d, length);
}

int flip_bit(void* data, unsigned long long size, unsigned long long index)
{
	unsigned long long i;
	unsigned char b, c;
	if (index >= size)
		return -1;
	i = index / 8ui64;
	b = 1 << (index % 8ui8);
	c = ((unsigned char*)data)[i];
	c ^= b;
	((unsigned char*)data)[i] = c;
	return 0;
}


int test_config(const char* data, char* b1, char* b2, unsigned long long data_size, unsigned long long b1_size, unsigned long long b2_size, parity_t parity, hc_mode_t hc_mode, int nb_parity_bits, int flip)
{
	HammingDecodeResult_t decodeResult;
	HammingEncodeResult_t encodeResult;
	printf("HAMMING %d ", nb_parity_bits);
	if (parity == parity_even)
		printf("EVEN");
	else
		printf("ODD ");
	printf(" ");
	if (hc_mode == hc_mode_sec)
		printf("SEC   ");
	else
		printf("SECDEC");

	printf(" flip %d : ", flip);
	encodeResult = hc_encode(data, data_size * 8, b1, b1_size * 8, parity, nb_parity_bits, hc_mode);
	flip_bit(b1, encodeResult.bits, flip);
	decodeResult = hc_decode(b1, encodeResult.bits, b2, b2_size * 8, parity, nb_parity_bits, hc_mode);
	b2[b2_size - 1] = 0ui8;
	if (strcmp(data, b2) != 0)
	{
		printf("ERROR\n");
		print_all("data", data, 0);
		print_all("b1  ", b1, encodeResult.bytes);
		print_all("b2  ", b2, decodeResult.bytes);
		printf("result : %s\n", b2);
		return 1;
	}
	else
		printf("OK : %llu\n", decodeResult.blocks);
	return 0;
}


int main()
{
	const char data[] = "Hello World";
	unsigned long long data_size = sizeof(data);
	char buffer[sizeof(data) * 2];
	unsigned long long buffer_size = sizeof(buffer);
	hc_encode(data, data_size * 8, buffer, buffer_size * 8, parity_odd, hamming_7_4, hc_mode_secded);
	print_bin("", buffer, buffer_size);
	return 0;

	/*
	const unsigned long long size = 1024 + data_size * 3;
	unsigned long long r_b1_size = size;
	unsigned long long r_b2_size = size;
	int errors = 0, i, j;
	char* b1 = (char*)malloc(r_b1_size);
	char* b2 = (char*)malloc(r_b2_size);
	if (!b1)
		return -1;
	if (!b2)
		return -2;
	for (i = 2; i < 12; ++i)
	{
		for (j = 0; j < data_size * 8; ++j)
		{
			errors += test_config(data, b1, b2, data_size, r_b1_size, r_b2_size, parity_even, hc_mode_sec, i, j);
			errors += test_config(data, b1, b2, data_size, r_b1_size, r_b2_size, parity_odd,  hc_mode_sec, i, j);
			errors += test_config(data, b1, b2, data_size, r_b1_size, r_b2_size, parity_even, hc_mode_secded, i, j);
			errors += test_config(data, b1, b2, data_size, r_b1_size, r_b2_size, parity_odd,  hc_mode_secded, i, j);
		}
	}
	return errors;
	*/
}