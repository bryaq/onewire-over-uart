#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "devices/common.h"
#include "onewire.h"

static void
ppulse(void)
{
	usleep(600);
	ow_bit(1);
	usleep(50000);
}

int
main(int argc, char *argv[])
{
	int i, j;
	unsigned char key[8], byte[3], data[8], b;
	
	if(argc < 3){
		fprintf(stderr, "usage: %s COM ID\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if(strlen(argv[2]) != 12){
		fprintf(stderr, "ID must be a 12-digit hex number.\n");
		exit(EXIT_FAILURE);
	}
	
	byte[2] = '\0';
	for(i = 0; i < 6; i++){
		byte[0] = argv[2][i * 2];
		byte[1] = argv[2][i * 2 + 1];
		key[i + 1] = strtol((char *)byte, NULL, 0x10);
	}
	key[0] = 0x01;
	key[7] = crc8(key, 7);
	
	if(ow_init(argv[1]) == OW_ERR_UART){
		fprintf(stderr, "Bus INIT failed. Check COM port.\n");
		exit(EXIT_FAILURE);
	}
	
	ow_reset();
	ow_byte_wr(0x33);
	for(i = 0; i < 8; i++){
		data[i] = ow_byte_rd();
		printf("%02x", data[i]);
	}
	printf(" read from the device; crc = %02x\n", crc8(data, 7));
	
	for(i = 0; i < 8; i++){
		ow_reset();
		j = 0;
		data[j] = 0x3c;		/* write ROM */
		ow_byte_wr(data[j++]);
		data[j] = i;		/* address LSB */
		ow_byte_wr(data[j++]);
		data[j] = 0x00;		/* address MSB */
		ow_byte_wr(data[j++]);
		data[j] = key[i];
		ow_byte_wr(data[j++]);
		b = ow_byte_rd();
		printf("crc expected %02x got %02x\n", crc8(data, j), b);
		if(b != crc8(data, j))
			break;
		ppulse();
		b = ow_byte_rd();
		printf("data expected %02x got %02x\n", key[i], b);
		if(b != key[i])
			break;
	}
	if(i < 8)
		printf("write error\n");
	
	ow_reset();
	ow_byte_wr(0x33);
	for(i = 0; i < 8; i++){
		data[i] = ow_byte_rd();
		printf("%02x", data[i]);
	}
	printf(" read from the device; crc = %02x\n", crc8(data, 7));
	
	ow_finit();
	
	return 0;
}
