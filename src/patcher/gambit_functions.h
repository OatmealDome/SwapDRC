#ifndef _GAMBIT_FUNCTIONS_H_
#define _GAMBIT_FUNCTIONS_H_

#include "dynamic_libs/vpad_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum GambitDPAD
	{
		D_NEUTRAL,
		D_LEFT,
		D_UP,
		D_RIGHT,
		D_DOWN
	} GambitDPAD;

	extern void gambitDRC(void);
	extern void gambitJump(VPADData *buffer);
	extern void gambitTouch(VPADTPData *screen);
	extern void gambitPatches(VPADData *buffer);

#ifdef __cplusplus
}
#endif

#endif /* _GAMBIT_FUNCTIONS_H_ */
