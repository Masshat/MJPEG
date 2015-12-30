#include <srl.h>
#include "split_proto.h"

#include "../common/block_io.h"
#include "../common/jpeg.h"

FUNC(split)
{
    srl_mwmr_t input = GET_ARG(input);
    srl_mwmr_t output[2] = { GET_ARG(output0), GET_ARG(output1) };

    int which = -1;
    uint8_t in[BLOCK_SIZE];
    uint8_t out[BLOCK_SIZE];
    block_io bin;
    block_io bout[2];

    srl_log(TRACE, "SPLIT thread is alive!\n");

    block_io_init_in(&bin, in, sizeof(in), input);
    block_io_init_out(&bout[0], out, sizeof(out), output[0]);
    block_io_init_out(&bout[1], out, sizeof(out), output[1]);

    while (1) {
        uint8_t b = block_io_read_int8(&bin);

        if (b != 0xFF) {
            block_io_write_int8(&bout[which], b);
        } else {
            uint8_t m = block_io_read_int8(&bin);

            if (m == 0xD8) {
                int i;

                if (which >= 0) {
                    int left = bout[which].left;
                    
                    for (i = 0; i < left; i++)
                        block_io_write_int8(&bout[which], 0xFF);

                    block_io_flush(&bout[which]);
                }
                
                which = !which;
            }

            block_io_write_int8(&bout[which], b);
            block_io_write_int8(&bout[which], m);
        }
    }
}
