#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "video_swapping_function_patcher.h"
#include "utils/logger.h"
#include "retain_vars.h"

// GX2 FUNCTIONS
DECL(void, GX2CopyColorBufferToScanBuffer, GX2ColorBuffer *colorBuffer, s32 scan_target) {
	// GX2 destinations:
	// 0x1 = TV
	// 0x4 = 1st GamePad

	// drcMode values:
	// 0x0 = normal
	// 0x1 = swap

	// check drc swap and force the drcMode to default when inkstrike is activated
	if (drcMode == 0 || gSwapForce || gAppStatus == 2) {
		real_GX2CopyColorBufferToScanBuffer(colorBuffer, scan_target);
	}
	else {
		switch (scan_target)
		{
		case 0x1:
			real_GX2CopyColorBufferToScanBuffer(colorBuffer, 0x4);
			break;
		case 0x4:
			real_GX2CopyColorBufferToScanBuffer(colorBuffer, 0x1);
			break;
		}
	}
}

hooks_magic_t method_hooks_video_swapping[] __attribute__((section(".data"))) = {
	MAKE_MAGIC(GX2CopyColorBufferToScanBuffer, LIB_GX2, STATIC_FUNCTION),
};
u32 method_hooks_size_video_swapping __attribute__((section(".data"))) = sizeof(method_hooks_video_swapping) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_video_swapping[sizeof(method_hooks_video_swapping) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));
