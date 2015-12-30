#include <srl.h>
#include "libu_proto.h"

#include "../common/jpeg.h"

FUNC(libu)
{
    srl_mwmr_t input[2] = { GET_ARG(input0), GET_ARG(input1) };
    srl_mwmr_t output = GET_ARG(output);

    uint8_t in[BLOCK_SIZE];
    uint8_t out[WIDTH * BLOCK_HEIGHT];
    int i, j, k, l;
    int which = 0;

    srl_log(TRACE, "LIBU thread is alive!\n");

    while (1) {
        for ( i = 0; i < BLOCKS_H; i++ ) {
            for ( j = 0; j < BLOCKS_W; j++ ) {
                srl_mwmr_read( input[which], in, sizeof(in) );

                for ( k = 0; k < BLOCK_HEIGHT; k++ )
                    for ( l = 0; l < BLOCK_WIDTH; l++ )
                        out[k * WIDTH + j * BLOCK_WIDTH + l] = in[k * BLOCK_WIDTH + l];
            }

            srl_mwmr_write( output, out, sizeof(out));
        }

        which = !which;
    }
}
