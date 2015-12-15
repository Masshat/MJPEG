#include <srl.h>
#include "producer_proto.h"

FUNC(prod_func)
{
        srl_mwmr_t output = GET_ARG(output);
        char buf[32] = "...World";
        srl_log_printf(NONE, "Producer : Hello...\n");
        srl_mwmr_write(output, buf, 32);
}
