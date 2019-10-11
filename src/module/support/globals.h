
/**
 * History:
 * ================================================================
 * 2019-10-10 qing.zou created
 *
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

void* log_default_config(void);
void log_set_config(void *config);

void* global_default_config(void);
void global_set_config(void *config);

void* global_default_cache(void);
void global_set_cache(void *cache);

#ifdef __cplusplus
}
#endif

#endif //GLOBALS_H
