#ifndef _GAMBIT_FUNCTIONS_H_
#define _GAMBIT_FUNCTIONS_H_

#include "dynamic_libs/vpad_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern void gambitPatcher(VPADData *buffer);
	extern int gambitDRC(void);

#ifdef __cplusplus
}
#endif

#endif /* _FS_H */
