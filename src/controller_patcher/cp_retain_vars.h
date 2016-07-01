#ifndef CP_RETAINS_VARS_H_
#define CP_RETAINS_VARS_H_

#include "dynamic_libs/syshid_functions.h"
#include "controller_patcher/controller_patcher.h"

extern u8 config_done;

extern u8 gHIDSetupDone;
extern u16 gHIDAttached;
extern u16 gHIDCurrentDevice;
extern HIDClient gHIDClient;

extern HID_DEVICE_DATA gHID_Devices[CONTRPD_MAX_VALUE];

extern HID_Mouse gHID_Mouse;

extern u8 gHID_Mouse_Mode;

extern u8 config_controller[CONTRPD_MAX_VALUE][CONTRPS_MAX_VALUE][2];
extern u16 config_controller_list[CONTRPD_MAX_VALUE];
extern u32 config_controller_data_ptr[CONTRPD_MAX_VALUE][4]; //currently max per device

#endif // CP_RETAINS_VARS_H_
