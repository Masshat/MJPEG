#include "../common/jpeg.h"
#include "libu_proto.h"

#define size BLOCK_WIDTH * WIDTH
FUNC(libu)
{
	uint8_t i, j, z;
    srl_mwmr_t input_mwmr[2] = {GET_ARG(input1),GET_ARG(input2)};
    srl_mwmr_t output_mwmr = GET_ARG(output);

	int8_t bloc [BLOCK_WIDTH * BLOCK_HEIGHT];
	int8_t buffer [size];
	int8_t pipe = 0;
	while (1){
	for (i=0; i<BLOCKS_H; i++){
		for (j=0; j<BLOCKS_W; j++){
			srl_mwmr_read(input_mwmr[pipe],(void *)bloc , BLOCK_SIZE);
			for (z=0; z<BLOCK_WIDTH; z++)
				memcpy(&buffer[j*BLOCK_WIDTH+z*WIDTH],&bloc[BLOCK_WIDTH*z],BLOCK_WIDTH);
		}
			
		srl_mwmr_write(output_mwmr, (void *)buffer, size *sizeof(uint8_t));
	}
	pipe = ! pipe;
}
}
