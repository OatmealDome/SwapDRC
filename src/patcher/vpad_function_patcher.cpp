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

#if !LITE
	// switch on L and SELECT
	if ((buffer[0].btns_h & (VPAD_BUTTON_MINUS | VPAD_BUTTON_L)) == (VPAD_BUTTON_MINUS | VPAD_BUTTON_L) &&
		gAppStatus != 2) {
		if (buffer->btns_d & (VPAD_BUTTON_MINUS | VPAD_BUTTON_L)) {
			drcSwap();
		}
	}
#endif

	// switch on TV button
	if (buffer[0].btns_h & VPAD_BUTTON_TV && (gCoolDown == 0 && gAppStatus != 2)) {
		gCoolDown = 0x1F;
		drcSwap();
	}
	else if (gCoolDown > 0) {
		gCoolDown--;
	}

	// switch on/off gamepad screen
	if (buffer[0].btns_d & VPAD_BUTTON_STICK_R && gLCDMode != 0xFF) {
		VPADSetLcdMode(0, gLCDMode = 0xFF); // Turn it on
	}
	else if (buffer[0].btns_h & VPAD_BUTTON_STICK_R) {
		if (gLCDDelay == 0xB0) {
			if (gLCDMode != 1) {
				VPADSetLcdMode(0, gLCDMode = 1); // Turn it off
			}
			else {
				VPADSetLcdMode(0, gLCDMode = 0xFF); // Turn it on
			}

			// reset delay
			gLCDDelay = 0;
		}
		gLCDDelay++;
	}
	else {
		gLCDDelay = 0;
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
