#include "srl.h"
#include "srl_endianness.h"
#include "libu_proto.h"

#include "../common/jpeg.h"


/*
bloc : 8x8 pixels
buffer : WIDTH*8 pixels

Pour chaque 0 .. BLOCKS_H:
Pour chaque 0 .. BLOCKS_W:
Lire un bloc
Pour chaque ligne du bloc
Copier les 8 pixels en les mettant à leur place dans buffer
Pour chacune des 8 lignes du buffer:
Envoyer la ligne
*/


FUNC(libu)
{
        uint8_t block[BLOCK_SIZE];
        uint64_t buffer[WIDTH];
        int bh;
        int bw;
        int lines;
        int channel = 1;
        int pictures = 0;

        srl_mwmr_t input[2] = {GET_ARG(input0), GET_ARG(input1)};
        srl_mwmr_t output = GET_ARG(output);

        while (1)
        {
                for (bh = 0; bh < BLOCKS_H; bh++)
                {
                        for (bw = 0; bw < BLOCKS_W; bw++)
                        {
                                srl_mwmr_read(input[channel], block, BLOCK_SIZE*sizeof(*block));
                                for (lines = 0; lines < BLOCK_HEIGHT; lines++)
                                        buffer[bw + lines * BLOCKS_W] = ((uint64_t *)block)[lines];
                        }
                        srl_mwmr_write(output, &buffer, WIDTH * sizeof(*buffer));
                }
//                channel = (channel + 1) % 2;
                srl_log_printf(TRACE, "################## pictures %d #################\n", pictures++);
        }

}

