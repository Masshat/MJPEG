#include "../common/jpeg.h"
#include "iqzz_proto.h"

const static uint8_t ZZ[BLOCK_SIZE] =
  { 0,  1,  8, 16,  9,  2,  3, 10, 
    17, 24, 32, 25, 18, 11,  4,  5, 
    12, 19, 26, 33, 40, 48, 41, 34, 
    27, 20, 13,  6,  7, 14, 21, 28, 
    35, 42, 49, 56, 57, 50, 43, 36, 
    29, 22, 15, 23, 30, 37, 44, 51, 
    58, 59, 52, 45, 38, 31, 39, 46, 
    53, 60, 61, 54, 47, 55, 62, 63
  }; 

FUNC(iqzz)
{
	uint8_t i, j;
    srl_mwmr_t input_mwmr = GET_ARG(input);
    srl_mwmr_t output_mwmr = GET_ARG(output);
    srl_mwmr_t huffman_mwmr = GET_ARG(quanti);

	uint8_t quantif [BLOCK_SIZE];
	int16_t Fn [BLOCK_WIDTH * BLOCK_HEIGHT];
	int32_t Fns [BLOCK_WIDTH * BLOCK_HEIGHT]; // Fns represente  Fn'.

	srl_mwmr_read(huffman_mwmr,(void *)quantif , BLOCK_SIZE *sizeof(uint8_t));

	for (i=0; i<NBLOCKS; i++){
		
		srl_mwmr_read(input_mwmr,(void *)Fn , BLOCK_SIZE *sizeof(uint16_t));
		for (j=0; j<BLOCK_SIZE; j++)
			Fns[ZZ[j]] = Fn[j]*quantif[j];
		srl_mwmr_write(output_mwmr,(void *)Fns , BLOCK_SIZE *sizeof(uint32_t));
	}
}
