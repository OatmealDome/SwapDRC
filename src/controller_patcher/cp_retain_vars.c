#include <gctypes.h>
#include "dynamic_libs/syshid_functions.h"
#include "controller_patcher/controller_patcher.h"

u8 config_done __attribute__((section(".data"))) = 0;

u8 gHIDSetupDone __attribute__((section(".data"))) = 0;
u16 gHIDAttached __attribute__((section(".data"))) = 0;
u16 gHIDCurrentDevice __attribute__((section(".data"))) = 0;

HIDClient gHIDClient __attribute__((section(".data")));

HID_DEVICE_DATA gHID_Devices[CONTRPD_MAX_VALUE] __attribute__((section(".data")));

HID_Mouse gHID_Mouse __attribute__((section(".data")));

u8 gHID_Mouse_Mode __attribute__((section(".data"))) = HID_MOUSE_MODE_TOUCH;

u8 config_controller[CONTRPD_MAX_VALUE][CONTRPS_MAX_VALUE][2] __attribute__((section(".data")));
u16 config_controller_list[CONTRPD_MAX_VALUE] __attribute__((section(".data")));
u32 config_controller_data_ptr[CONTRPD_MAX_VALUE][4] __attribute__((section(".data")));
