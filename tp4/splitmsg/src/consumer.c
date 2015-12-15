#include <srl.h>
#include "consumer_proto.h"

FUNC(cons_func)
{
        srl_mwmr_t input = GET_ARG(input);
        char buf[32];
        srl_mwmr_read(input, buf, 32);
        srl_log_printf(NONE, "Consumer : %s\n\n", buf);
}
