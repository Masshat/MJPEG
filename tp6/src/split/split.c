#include "srl.h"
#include "split_proto.h"
#include "srl_endianness.h"

#include "../common/jpeg.h"

#include "../common/block_io.h"


FUNC(split)
{
        uint8_t input_buffer[32];
        block_io input;
        uint8_t output_buffer[2][32];
        block_io output[2];
        int channel = 1;
        uint8_t b;
        uint8_t m;

        srl_mwmr_t input_mwmr = GET_ARG(input);
        srl_mwmr_t output_mwmr[2] = {GET_ARG(output0), GET_ARG(output1)};

        block_io_init_in(&input, input_buffer, 32, input_mwmr);
        block_io_init_out(&output[0], output_buffer[0], 32, output_mwmr[0]);
        block_io_init_out(&output[1], output_buffer[1], 32, output_mwmr[1]);


        while(1)
        {
                b = block_io_read_int8(&input);
                if (b == 0xff)
                {
                        m = block_io_read_int8(&input);
                        if (m == 0xd8)
                        {
                                block_io_set_left(&output[channel], 0xff);
                                block_io_flush(&output[channel]);
//                                channel = (channel + 1) % 2;
                        }
                        block_io_write_int8(&output[channel], b);
                        block_io_write_int8(&output[channel], m);

                        continue;
                }
                block_io_write_int8(&output[channel], b);
        }
}

