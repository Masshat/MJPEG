#include "srl.h"
#include "iqzz_proto.h"

#include "../common/jpeg.h"
#include "../common/block_io.h"
#include "srl_endianness.h"


typedef struct iqtable_s
{
        uint8_t table [BLOCK_SIZE];
}iqtable_t;

typedef struct in_blocks_s
{
        int16_t blocks [BLOCK_SIZE];
}in_blocks_t;

typedef struct out_blocks_s
{
        int32_t blocks [BLOCK_SIZE];
}out_blocks_t;

FUNC(iqzz)
{
        srl_mwmr_t input_mwmr = GET_ARG(input);
        srl_mwmr_t output = GET_ARG(output);
        srl_mwmr_t quanti_mwmr = GET_ARG(quanti);

        uint8_t zz_table [BLOCK_SIZE] = {
                               0,  1,  8,  16,  9,  2,  3,  10,
                               17, 24, 32, 25,  18, 11, 4,  5,
                               12, 19, 26, 33,  40, 48, 41, 34,
                               27, 20, 13, 6,   7,  14, 21, 28,
                               35, 42, 49, 56,  57, 50, 43, 36,
                               29, 22, 15, 23,  30, 37, 44, 51,
                               58, 59, 52, 45,  38, 31, 39, 46,
                               53, 60, 61, 54,  47, 55, 62, 63};
        iqtable_t iqtable;
        in_blocks_t iblocks;
        out_blocks_t oblocks;
        int blocks;
        int i;

        srl_mwmr_read(quanti_mwmr, iqtable.table, BLOCK_SIZE * sizeof(uint8_t));
        for(blocks = 0; blocks < NBLOCKS; blocks++)
        {
                srl_mwmr_read(input_mwmr, iblocks.blocks, BLOCK_SIZE * sizeof(int16_t));
                for (i = 0; i < BLOCK_SIZE; i++)
                        oblocks.blocks[zz_table[i]] = iblocks.blocks[i] * iqtable.table[i];
                srl_mwmr_write(output, oblocks.blocks, BLOCK_SIZE * sizeof(int32_t));
        }
}

