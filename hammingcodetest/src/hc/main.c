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

int main()
{
	const char data[] = "Hello world, how are you?";
	unsigned long long data_size = sizeof(data);
	const unsigned long long size = 1024 + data_size * 3;
	unsigned long long r_b1_size = size;
	unsigned long long r_b2_size = size;
	unsigned long long b1_size, b2_size, r;
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
			printf("HAMMING %d EVEN flip %d : ", i, j);
			b1_size = hc_encode_bytes(data, data_size, b1, r_b1_size, parity_even, i);
			flip_bit(b1, b1_size, j);
			b2_size = hc_decode_bytes(b1, b1_size, b2, r_b2_size, parity_even, i);
			b2[r_b2_size - 1] = 0ui8;
			if (strcmp(data, b2) != 0)
			{
				printf("ERROR\n");
				print_all("data", data, 0);
				print_all("b1  ", b1, b1_size);
				print_all("b2  ", b2, b2_size);
				printf("result : %s\n", b2);
				++errors;
			}
			else
				printf("OK\n");

			printf("HAMMING %d ODD  flip %d : ", i, j);
			b1_size = hc_encode_bytes(data, data_size, b1, r_b1_size, parity_odd, i);
			flip_bit(b1, b1_size, j);
			b2_size = hc_decode_bytes(b1, b1_size, b2, r_b2_size, parity_odd, i);
			b2[r_b2_size - 1] = 0ui8;
			if (strcmp(data, b2) != 0)
			{
				printf("ERROR\n");
				print_all("data", data, 0);
				print_all("b1  ", b1, b1_size);
				print_all("b2  ", b2, b2_size);
				printf("result : %s\n", b2);
				++errors;
			}
			else
				printf("OK\n");
		}
	}
	return errors;
}