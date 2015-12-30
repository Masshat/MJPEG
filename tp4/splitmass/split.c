#include "srl.h"
#include "split_proto.h"
#include "srl_endianness.h"
#include "stdbool.h"

#include "../common/jpeg.h"
#include "../common/block_io.h"

FUNC(split) {

	srl_mwmr_t input_mwmr   = GET_ARG(input);
    	srl_mwmr_t output_mwmr[2] = {GET_ARG(output1), GET_ARG(output2)};

	uint8_t input_buffer[32];
	uint8_t output_buffer[32];

	block_io in, out[2];

	block_io_init_in  (&in    , input_buffer , 32, input_mwmr);
	block_io_init_out (&out[0], output_buffer, 32, output_mwmr[0]);
	block_io_init_out (&out[1], output_buffer, 32, output_mwmr[1]);
	
	srl_log(TRACE, "SPLIT thread is alive!\n");

	uint8_t b, m;
	uint8_t  pipe = 0;
	while(1){
		b = block_io_read_int8(&in);
		if (b == 0xFF){	
			m = block_io_read_int8(&in);
			if (m == 0xD8){
				block_io_set_left(&out[pipe],0XFF);
				block_io_flush(&out[pipe]);
				pipe = ! pipe;
			}
			block_io_write_int8(&out[pipe], b);
			block_io_write_int8(&out[pipe], m);
			continue;
		}
		block_io_write_int8(&out[pipe], b);
	}
}
