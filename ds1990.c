#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "devices/common.h"
#include "onewire.h"

int
main(int argc, char *argv[])
{
	int i;
	unsigned char data[8];
	
	if(argc < 2){
		fprintf(stderr, "usage: %s COM\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
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
	
	return 0;
}
