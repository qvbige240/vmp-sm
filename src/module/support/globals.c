
/**
 * History:
 * ================================================================
 * 2019-10-10 qing.zou created
 *
 */

#include "globals.h"

struct globals
{
    void        *log;
    void        *config;
    void        *cache;
} g_globals;

void *log_default_config(void)
{
    return g_globals.log;
}

void log_set_config(void *config)
{
    g_globals.log = config;
}

void *global_default_config(void)
{
    return g_globals.config;
}

void global_set_config(void *config)
{
    g_globals.config = config;
}

void *global_default_cache(void)
{
    return g_globals.cache;
}

void global_set_cache(void *cache)
{
    g_globals.cache = cache;
}
