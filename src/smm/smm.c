/**
 * History:
 * ================================================================
 * 2019-09-11 qing.zou created
 *
 */

#include <unistd.h>

#include "smm.h"
#include "smm_typedef.h"
#include "smm_core.h"

void smm_init(int argc, char **argv)
{
    smm_core_init();
}

void smm_start(void)
{
    while (1)
    {
        sleep(10);
    }
}

void smm_done(void)
{
    smm_core_done();
}