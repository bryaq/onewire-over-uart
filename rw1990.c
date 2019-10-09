#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "devices/common.h"
#include "onewire.h"

int
main(int argc, char *argv[])
{
	int i, j;
	unsigned char key[8], byte[3], data[8];
	
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
		key[6 - i] = strtol((char *)byte, NULL, 0x10);
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
	
	ow_reset();
	ow_byte_wr(0xd1);
	ow_byte_wr(0x00);	/* write enable */
	
	ow_reset();
	ow_byte_wr(0xb5);
	printf("write enable = %02x\n", ow_byte_rd());
	
	ow_reset();
	ow_byte_wr(0xd5);
	for(i = 0; i < 8; i++){
		byte[0] = 0;
		for(j = 0; j < 8; j++)
			if((key[i] & (1 << j)) == 0)
				byte[0] |= (1 << (7 - j));
		ow_byte_wr(~key[i]);
	}
	
	ow_reset();
	ow_byte_wr(0xd1);
	ow_byte_wr(0xff);	/* write disable */
	
	ow_reset();
	ow_byte_wr(0xb5);
	printf("write enable = %02x\n", ow_byte_rd());
	
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
