/**
 * History:
 * ================================================================
 * 2019-09-11 qing.zou created
 *
 */

#ifndef SMM_H
#define SMM_H

#ifdef __cplusplus
extern "C" {
#endif

  void smm_init(int argc, char **argv);

  void smm_start(void);

  void smm_done(void);

#ifdef __cplusplus
}
#endif

#endif // SMM_H