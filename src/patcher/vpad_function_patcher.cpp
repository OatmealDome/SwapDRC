#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "vpad_function_patcher.h"
#include "voice_swapping_function_patcher.h"
#include "gambit_functions.h"
#include "utils/logger.h"
#include "retain_vars.h"

//VPAD FUNCTIONS
DECL(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
	int result = real_VPADRead(chan, buffer, buffer_size, error);
	if(result <= 0) return result;

	uint32_t bCombo[2] = {
		buffer[0].btns_h  & (VPAD_BUTTON_MINUS | VPAD_BUTTON_L),
		buffer[0].btns_h & VPAD_BUTTON_TV
	};

	// switch on L and SELECT and TV button
	if (((bCombo[0] == (VPAD_BUTTON_MINUS | VPAD_BUTTON_L)) ||
		(bCombo[1] == VPAD_BUTTON_TV)) && (
		gHomeCoolDown == 0 && (gAppStatus != 2))) {
        gHomeCoolDown = bCombo[0]? 0x20 : 0x5A;
		drcSwap();
	}
	else if (gHomeCoolDown > 0) {
		gHomeCoolDown--;
	}

	// patches splatoon enhanced controls
	if (isSplatoon) {
		gambitPatches(buffer);
		gambitDRC();
	}

	return result;
}

DECL(void, VPADGetTPCalibratedPoint, int chan, VPADTPData *screen, VPADTPData *raw) {
	real_VPADGetTPCalibratedPoint(chan, screen, raw);

	if (isSplatoon) {
		// handles modified touch input for super jumps
		gambitTouch(screen);
	}
}

DECL(void, VPADSetSensorBar, s32 chan, bool on){
	real_VPADSetSensorBar(chan, on);
}

void drcSwap() {
    log_printf("Swapping Screens!\n");
	// swap drc modes
	drcMode = !drcMode;

	// swap audio
	swapVoices();

	// enable/disable sensor bar
	real_VPADSetSensorBar(0, drcMode);
}

hooks_magic_t method_hooks_vpad[] __attribute__((section(".data"))) = {
	MAKE_MAGIC(VPADRead,                    LIB_VPAD, STATIC_FUNCTION),
	MAKE_MAGIC(VPADGetTPCalibratedPoint,    LIB_VPAD, STATIC_FUNCTION),
    MAKE_MAGIC(VPADSetSensorBar,            LIB_VPAD, STATIC_FUNCTION),
};
u32 method_hooks_size_vpad __attribute__((section(".data"))) = sizeof(method_hooks_vpad) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_vpad[sizeof(method_hooks_vpad) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));
