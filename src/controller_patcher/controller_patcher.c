#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "controller_patcher.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/syshid_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/padscore_functions.h"
#include "cp_retain_vars.h"
#include "utils/logger.h"

void setConfigValue(u8 * dest, u8 first, u8 second){
    dest[0] = first;
    dest[1] = second;
}

void init_config_controller(){
    InitOSFunctionPointers();
    InitSysHIDFunctionPointers();
    InitVPadFunctionPointers();
    InitPadScoreFunctionPointers();

    if(!config_done){
        config_done = 1;
        memset(config_controller,CONTROLLER_PATCHER_INVALIDVALUE,sizeof(config_controller)); // Init / Invalid everything
        memset(config_controller_list,-1,sizeof(config_controller_list)); // Init / Invalid everything
        memset(config_controller_data_ptr,0,sizeof(config_controller_data_ptr)); // Init / Invalid everything

        config_controller_list[CONTRPD_GC] =                                                        HID_LIST_GC;
        config_controller_list[CONTRPD_DS3] =                                                       HID_LIST_DS3;
        config_controller_list[CONTRPD_DS4] =                                                       HID_LIST_DS4;
        config_controller_list[CONTRPD_SP2600] =                                                    HID_LIST_SP2600;
        config_controller_list[CONTRPD_KEYBOARD] =                                                  HID_LIST_KEYBOARD;
        config_controller_list[CONTRPD_PS2_ADAPTER] =                                               HID_LIST_PS2_ADAPTER;
        config_controller_list[CONTRPD_POKKEN] =                                                    HID_LIST_POKKEN;
        config_controller_list[CONTRPD_PS2] =                                                       HID_LIST_PS2;
        //config_controller_list[CONTRPD_YOUR_DEVICE] =                                             HID_LIST_YOUR_DEVICE;

        //Set data for each pad. Currenty 4 Pads for each device support. May need other extra treatment
        config_controller_data_ptr[CONTRPD_GC][0] =                                                 (u32)&(gHID_Devices[CONTRPD_GC]).pad_data[0]; //GC Pad Port 1
        config_controller_data_ptr[CONTRPD_GC][1] =                                                 (u32)&(gHID_Devices[CONTRPD_GC]).pad_data[1]; //GC Pad Port 2
        config_controller_data_ptr[CONTRPD_GC][2] =                                                 (u32)&(gHID_Devices[CONTRPD_GC]).pad_data[2]; //GC Pad Port 3
        config_controller_data_ptr[CONTRPD_GC][3] =                                                 (u32)&(gHID_Devices[CONTRPD_GC]).pad_data[3]; //GC Pad Port 4
        config_controller_data_ptr[CONTRPD_DS3][0] =                                                (u32)&(gHID_Devices[CONTRPD_DS3]).pad_data[0];
        config_controller_data_ptr[CONTRPD_DS4][0] =                                                (u32)&(gHID_Devices[CONTRPD_DS4]).pad_data[0];
        config_controller_data_ptr[CONTRPD_SP2600][0] =                                             (u32)&(gHID_Devices[CONTRPD_SP2600]).pad_data[0];
        config_controller_data_ptr[CONTRPD_KEYBOARD][0] =                                           (u32)&(gHID_Devices[CONTRPD_KEYBOARD]).pad_data[0];
        config_controller_data_ptr[CONTRPD_PS2_ADAPTER][0] =                                        (u32)&(gHID_Devices[CONTRPD_PS2_ADAPTER]).pad_data[0];
        config_controller_data_ptr[CONTRPD_POKKEN][0] =                                             (u32)&(gHID_Devices[CONTRPD_POKKEN]).pad_data[0];
        config_controller_data_ptr[CONTRPD_PS2][0] =                                                (u32)&(gHID_Devices[CONTRPD_PS2]).pad_data[0];


        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! GC-Adapter
        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VID],                            0x05,0x7e);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_PID],                            0x03,0x37);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_A],                  0x01,HID_GC_BUTTON_A);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_B],                  0x01,HID_GC_BUTTON_B);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_X],                  0x01,HID_GC_BUTTON_X);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_Y],                  0x01,HID_GC_BUTTON_Y);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_DPAD_MODE],                      CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Normal);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_LEFT],               0x01,HID_GC_BUTTON_LEFT);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_RIGHT],              0x01,HID_GC_BUTTON_RIGHT);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_DOWN],               0x01,HID_GC_BUTTON_DOWN);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_UP],                 0x01,HID_GC_BUTTON_UP);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_MINUS],              0x02,HID_GC_BUTTON_START);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L],                  0x07,HID_GC_BUTTON_L);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R],                  0x08,HID_GC_BUTTON_R);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_PLUS],               0x02,HID_GC_BUTTON_START);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_ZL],                 0x07,HID_GC_BUTTON_L);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_ZR],                 0x08,HID_GC_BUTTON_R);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_STICK_L],            0x01,HID_GC_BUTTON_A);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_STICK_R],            0x01,HID_GC_BUTTON_B);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_DOUBLE_USE],                     CONTROLLER_PATCHER_VALUE_SET,CONTROLLER_PATCHER_GC_DOUBLE_USE);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_DOUBLE_USE_BUTTON],              0x02,HID_GC_BUTTON_Z);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_PAD_COUNT],                      CONTROLLER_PATCHER_VALUE_SET,HID_GC_PAD_COUNT);

        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L_STICK_X],          0x03,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L_STICK_X_DEADZONE], CONTROLLER_PATCHER_VALUE_SET,0x09);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX],   0x1E,0xE5);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L_STICK_Y],          0x04,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L_STICK_Y_DEADZONE], CONTROLLER_PATCHER_VALUE_SET,0x09);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX],   0x18,0xE4);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R_STICK_X],          0x05,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX],   0x26,0xE1);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R_STICK_X_DEADZONE], CONTROLLER_PATCHER_VALUE_SET,0x09);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R_STICK_Y],          0x06,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX],   0x1A,0xDB);
        setConfigValue((u8*)&config_controller[CONTRPD_GC][CONTRPS_VPAD_BUTTON_R_STICK_Y_DEADZONE], CONTROLLER_PATCHER_VALUE_SET,0x09);

        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! DS3
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VID],                           0x05,0x4c);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_PID],                           0x02,0x68);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_BUF_SIZE],                      CONTROLLER_PATCHER_VALUE_SET,128);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_A],                 0x03,HID_DS3_BUTTON_CIRCLE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_B],                 0x03,HID_DS3_BUTTON_CROSS);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_X],                 0x03,HID_DS3_BUTTON_TRIANGLE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_Y],                 0x03,HID_DS3_BUTTON_SQUARE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_DPAD_MODE],                     CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Normal);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_LEFT],              0x02,HID_DS3_BUTTON_LEFT);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_RIGHT],             0x02,HID_DS3_BUTTON_RIGHT);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_DOWN],              0x02,HID_DS3_BUTTON_DOWN);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_UP],                0x02,HID_DS3_BUTTON_UP);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_PLUS],              0x02,HID_DS3_BUTTON_START);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_MINUS],             0x02,HID_DS3_BUTTON_SELECT);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L],                 0x03,HID_DS3_BUTTON_L1);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R],                 0x03,HID_DS3_BUTTON_R1);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_ZL],                0x03,HID_DS3_BUTTON_L2);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_ZR],                0x03,HID_DS3_BUTTON_R2);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_STICK_L],           0x02,HID_DS3_BUTTON_L3);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_STICK_R],           0x02,HID_DS3_BUTTON_R3);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_HOME],              0x04,HID_DS3_BUTTON_GUIDE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_PAD_COUNT],                     CONTROLLER_PATCHER_VALUE_SET,HID_DS3_PAD_COUNT);

        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_X],         0x06,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_X_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x06);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_Y],         0x07,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_Y_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x06);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_L_STICK_Y_INVERT],  CONTROLLER_PATCHER_VALUE_SET,0x01);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_X],         0x08,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_X_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x08);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_Y],         0x09,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_Y_INVERT],  CONTROLLER_PATCHER_VALUE_SET,0x01);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_Y_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x06);
        setConfigValue((u8*)&config_controller[CONTRPD_DS3][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX],  0x00,0xFF);

        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! DS4
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VID],                           0x05,0x4c);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_PID],                           0x05,0xc4);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_BUF_SIZE],                      CONTROLLER_PATCHER_VALUE_SET,128);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_A],                 0x05,HID_DS4_BUTTON_CIRCLE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_B],                 0x05,HID_DS4_BUTTON_CROSS);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_X],                 0x05,HID_DS4_BUTTON_TRIANGLE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_Y],                 0x05,HID_DS4_BUTTON_SQUARE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_DPAD_MODE],                     CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Hat);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_DPAD_MASK],                     CONTROLLER_PATCHER_VALUE_SET,0x0F);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_N],            0x05,HID_DS4_BUTTON_DPAD_N);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_NE],           0x05,HID_DS4_BUTTON_DPAD_NE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_E],            0x05,HID_DS4_BUTTON_DPAD_E);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_SE],           0x05,HID_DS4_BUTTON_DPAD_SE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_S],            0x05,HID_DS4_BUTTON_DPAD_S);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_SW],           0x05,HID_DS4_BUTTON_DPAD_SW);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_W],            0x05,HID_DS4_BUTTON_DPAD_W);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_NW],           0x05,HID_DS4_BUTTON_DPAD_NW);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL],      0x05,HID_DS4_BUTTON_DPAD_NEUTRAL);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_PLUS],              0x06,HID_DS4_BUTTON_OPTIONS);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_MINUS],             0x06,HID_DS4_BUTTON_SHARE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L],                 0x06,HID_DS4_BUTTON_L1);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R],                 0x06,HID_DS4_BUTTON_R1);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_ZL],                0x06,HID_DS4_BUTTON_L2);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_ZR],                0x06,HID_DS4_BUTTON_R2);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_STICK_L],           0x06,HID_DS4_BUTTON_L3);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_STICK_R],           0x06,HID_DS4_BUTTON_R3);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_HOME],              0x07,HID_DS4_BUTTON_GUIDE);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_PAD_COUNT],                     CONTROLLER_PATCHER_VALUE_SET,HID_DS4_PAD_COUNT);

        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_X],         0x01,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_X_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x06);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_Y],         0x02,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_Y_INVERT],  CONTROLLER_PATCHER_VALUE_SET,0x01);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_Y_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x05);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_X],         0x03,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_X_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x07);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_Y],         0x04,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_Y_INVERT],  CONTROLLER_PATCHER_VALUE_SET,0x01);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_Y_DEADZONE],CONTROLLER_PATCHER_VALUE_SET,0x09);
        setConfigValue((u8*)&config_controller[CONTRPD_DS4][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX],  0x00,0xFF);

        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! Saitek P2600
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VID],                        0x06,0xA3);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_PID],                        0x5F,0x0D);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_A],              0x05,HID_SP2600_BUTTON_3);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_B],              0x05,HID_SP2600_BUTTON_2);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_X],              0x05,HID_SP2600_BUTTON_4);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_Y],              0x05,HID_SP2600_BUTTON_1);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_DPAD_MODE],                  CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Hat);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_DPAD_MASK],                     CONTROLLER_PATCHER_VALUE_SET,0xF0);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_N],         0x06,HID_SP2600_BUTTON_DPAD_N);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_NE],        0x06,HID_SP2600_BUTTON_DPAD_NE);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_E],         0x06,HID_SP2600_BUTTON_DPAD_E);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_SE],        0x06,HID_SP2600_BUTTON_DPAD_SE);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_S],         0x06,HID_SP2600_BUTTON_DPAD_S);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_SW],        0x06,HID_SP2600_BUTTON_DPAD_SW);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_W],         0x06,HID_SP2600_BUTTON_DPAD_W);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_NW],        0x06,HID_SP2600_BUTTON_DPAD_NW);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL],   0x06,HID_SP2600_BUTTON_DPAD_NEUTRAL);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_PLUS],           0x06,HID_SP2600_BUTTON_FPS);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_MINUS],          0x06,HID_SP2600_BUTTON_Analog);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_L],              0x05,HID_SP2600_BUTTON_5);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_R],              0x05,HID_SP2600_BUTTON_6);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_ZL],             0x05,HID_SP2600_BUTTON_7);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_ZR],             0x05,HID_SP2600_BUTTON_8);

        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_PAD_COUNT],                  CONTROLLER_PATCHER_VALUE_SET,HID_SP2600_PAD_COUNT);

        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_L_STICK_X],         0x01,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_L_STICK_Y],         0x02,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_R_STICK_X],         0x03,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_R_STICK_Y],         0x04,0x80);
        setConfigValue((u8*)&config_controller[CONTRPD_SP2600][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX],  0x00,0xFF);

        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! PS2_ADAPTER
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VID],                           0x0e,0x8f);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_PID],                           0x00,0x03);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_A],                 0x05,HID_PS2_ADAPTER_BUTTON_CIRCLE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_B],                 0x05,HID_PS2_ADAPTER_BUTTON_CROSS);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_X],                 0x05,HID_PS2_ADAPTER_BUTTON_TRIANGLE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_Y],                 0x05,HID_PS2_ADAPTER_BUTTON_SQUARE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_PLUS],              0x06,HID_PS2_ADAPTER_BUTTON_START);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_MINUS],             0x06,HID_PS2_ADAPTER_BUTTON_SELECT);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_L],                 0x06,HID_PS2_ADAPTER_BUTTON_L1);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_R],                 0x06,HID_PS2_ADAPTER_BUTTON_R1);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_ZL],                0x06,HID_PS2_ADAPTER_BUTTON_L2);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_ZR],                0x06,HID_PS2_ADAPTER_BUTTON_R2);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_DPAD_MODE],                     CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Hat);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_DPAD_MASK],                     CONTROLLER_PATCHER_VALUE_SET,0x0F);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_N],            0x05,HID_PS2_ADAPTER_BUTTON_DPAD_N);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_NE],           0x05,HID_PS2_ADAPTER_BUTTON_DPAD_NE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_E],            0x05,HID_PS2_ADAPTER_BUTTON_DPAD_E);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_SE],           0x05,HID_PS2_ADAPTER_BUTTON_DPAD_SE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_S],            0x05,HID_PS2_ADAPTER_BUTTON_DPAD_S);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_SW],           0x05,HID_PS2_ADAPTER_BUTTON_DPAD_SW);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_W],            0x05,HID_PS2_ADAPTER_BUTTON_DPAD_W);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_NW],           0x05,HID_PS2_ADAPTER_BUTTON_DPAD_NW);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL],      0x05,HID_PS2_ADAPTER_BUTTON_DPAD_NEUTRAL);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_PAD_COUNT],                     CONTROLLER_PATCHER_VALUE_SET,HID_PS2_ADAPTER_PAD_COUNT);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_L_STICK_X],         0x02,0x7B);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_L_STICK_Y],         0x03,0x84);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_R_STICK_X],         0x00,0x7B);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_R_STICK_Y],         0x01,0x84);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2_ADAPTER][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX],  0x00,0xFF);

        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! Pokken
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VID],                           0x0f,0x0d);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_PID],                           0x00,0x92);

        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_A],                 0x05,HID_POKKEN_BUTTON_1);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_B],                 0x05,HID_POKKEN_BUTTON_2);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_X],                 0x05,HID_POKKEN_BUTTON_3);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_Y],                 0x05,HID_POKKEN_BUTTON_4);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_PLUS],              0x06,HID_POKKEN_BUTTON_5);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_MINUS],             0x06,HID_POKKEN_BUTTON_6);

        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_L],                 0x06,HID_POKKEN_BUTTON_7);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_R],                 0x06,HID_POKKEN_BUTTON_8);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_ZL],                0x06,HID_POKKEN_BUTTON_9);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_ZR],                0x06,HID_POKKEN_BUTTON_10);

        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_DPAD_MODE],                     CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Hat);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_DPAD_MASK],                     CONTROLLER_PATCHER_VALUE_SET,0x0F);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_N],            0x05,HID_POKKEN_BUTTON_DPAD_N);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_NE],           0x05,HID_POKKEN_BUTTON_DPAD_NE);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_E],            0x05,HID_POKKEN_BUTTON_DPAD_E);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_SE],           0x05,HID_POKKEN_BUTTON_DPAD_SE);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_S],            0x05,HID_POKKEN_BUTTON_DPAD_S);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_SW],           0x05,HID_POKKEN_BUTTON_DPAD_SW);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_W],            0x05,HID_POKKEN_BUTTON_DPAD_W);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_NW],           0x05,HID_POKKEN_BUTTON_DPAD_NW);
        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL],      0x05,HID_POKKEN_BUTTON_DPAD_NEUTRAL);

        setConfigValue((u8*)&config_controller[CONTRPD_POKKEN][CONTRPS_PAD_COUNT],                     CONTROLLER_PATCHER_VALUE_SET,HID_POKKEN_PAD_COUNT);

        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! PS2
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VID],                           0x08,0x10);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_PID],                           0x00,0x01);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_A],                 0x05,HID_PS2_BUTTON_CIRCLE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_B],                 0x05,HID_PS2_BUTTON_CROSS);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_X],                 0x05,HID_PS2_BUTTON_TRIANGLE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_Y],                 0x05,HID_PS2_BUTTON_SQUARE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_PLUS],              0x06,HID_PS2_BUTTON_START);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_MINUS],             0x06,HID_PS2_BUTTON_SELECT);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_L],                 0x06,HID_PS2_BUTTON_L1);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_R],                 0x06,HID_PS2_BUTTON_R1);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_ZL],                0x06,HID_PS2_BUTTON_L2);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_ZR],                0x06,HID_PS2_BUTTON_R2);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_DPAD_MODE],                     CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Hat);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_DPAD_MASK],                     CONTROLLER_PATCHER_VALUE_SET,0x0F);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_N],            0x05,HID_PS2_BUTTON_DPAD_N);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_NE],           0x05,HID_PS2_BUTTON_DPAD_NE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_E],            0x05,HID_PS2_BUTTON_DPAD_E);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_SE],           0x05,HID_PS2_BUTTON_DPAD_SE);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_S],            0x05,HID_PS2_BUTTON_DPAD_S);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_SW],           0x05,HID_PS2_BUTTON_DPAD_SW);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_W],            0x05,HID_PS2_BUTTON_DPAD_W);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_NW],           0x05,HID_PS2_BUTTON_DPAD_NW);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL],      0x05,HID_PS2_BUTTON_DPAD_NEUTRAL);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_PAD_COUNT],                     CONTROLLER_PATCHER_VALUE_SET,HID_PS2_PAD_COUNT);

        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_L_STICK_X],         0x03,0x7B);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_L_STICK_Y],         0x04,0x84);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_R_STICK_X],         0x02,0x7B);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX],  0x00,0xFF);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_R_STICK_Y],         0x01,0x84);
        setConfigValue((u8*)&config_controller[CONTRPD_PS2][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX],  0x00,0xFF);


        //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //! Keyboard
        //!---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_A],                 0x00,HID_KEYBOARD_BUTTON_E);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_B],                 0x00,HID_KEYBOARD_BUTTON_Q);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_X],                 0x00,HID_KEYBOARD_BUTTON_SPACE);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_Y],                 0x00,HID_KEYBOARD_BUTTON_R);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_DPAD_MODE],                     CONTROLLER_PATCHER_VALUE_SET,CONTRPDM_Normal);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_LEFT],              0x00,HID_KEYBOARD_BUTTON_LEFT);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_RIGHT],             0x00,HID_KEYBOARD_BUTTON_RIGHT);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_DOWN],              0x00,HID_KEYBOARD_BUTTON_DOWN);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_UP],                0x00,HID_KEYBOARD_BUTTON_UP);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_PLUS],              0x00,HID_KEYBOARD_BUTTON_ENTER);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_MINUS],             0x00,HID_KEYBOARD_BUTTON_MINUS);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_L],                 0x00,HID_KEYBOARD_BUTTON_V);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_R],                 0x00,HID_KEYBOARD_BUTTON_B);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_ZL],                0x00,HID_KEYBOARD_BUTTON_SHIFT);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_ZR],                0x00,HID_KEYBOARD_BUTTON_N);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_STICK_L],           0x00,HID_KEYBOARD_BUTTON_F);
        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_VPAD_BUTTON_STICK_R],           0x00,HID_KEYBOARD_BUTTON_TAB);

        setConfigValue((u8*)&config_controller[CONTRPD_KEYBOARD][CONTRPS_PAD_COUNT],                     CONTROLLER_PATCHER_VALUE_SET,HID_KEYBOARD_PAD_COUNT);
    }else{
        log_print("Config already done!\n");
    }
    if(!gHIDSetupDone){
        HIDSetup();
        gHIDSetupDone = 1;
    }

    if(!gHIDAttached){
        HIDAddClient(&gHIDClient, my_attach_cb);
    }
}

void deinit_config_controller(){
    if(gHIDSetupDone){
        if(gHIDAttached) HIDDelClient(&gHIDClient);
        gHIDAttached = 0;
        gHIDSetupDone = 0;
        gHIDCurrentDevice = 0;
        //HIDTeardown(); // seems to result in a creash sometimes
        log_printf("Exit\n");
    }
}


#define SWAP16(x) ((x>>8) | ((x&0xFF)<<8))
#define SWAP8(x) ((x>>4) | ((x&0xF)<<4))
int my_attach_cb(HIDClient *p_client, HIDDevice *p_device, unsigned int attach)
{
    if(attach){
        log_printf("vid              %04x\n", SWAP16(p_device->vid));
        log_printf("pid              %04x\n", SWAP16(p_device->pid));
        log_printf("interface index  %02x\n", p_device->interface_index);
        log_printf("sub class        %02x\n", p_device->sub_class);
        log_printf("protocol         %02x\n", p_device->protocol);
        log_printf("max packet in    %02x\n", p_device->max_packet_size_rx);
        log_printf("max packet out   %02x\n", p_device->max_packet_size_tx);
    }

    int device = 0;
    int deviceSlot = 0;
    for(int i = 0;i< CONTRPD_MAX_VALUE;i++){
        u16 vid = config_controller[i][CONTRPS_VID][0] * 0x100 + config_controller[i][CONTRPS_VID][1];
        u16 pid = config_controller[i][CONTRPS_PID][0] * 0x100 + config_controller[i][CONTRPS_PID][1];
        if(SWAP16(p_device->vid) == vid && SWAP16(p_device->pid) == pid){
            device = config_controller_list[i];
            deviceSlot = i;
            break;
        }
    }
    if ((p_device->sub_class == 1) && (p_device->protocol == 1)) { //Keyboard
        device = HID_LIST_KEYBOARD;
        deviceSlot = CONTRPD_KEYBOARD;
    }else if ((p_device->sub_class == 1) && (p_device->protocol == 2)){ // MOUSE
        device = HID_LIST_MOUSE;
    }

    if(device){
        if(attach){
            int bufSize = 64;
            if(device != HID_LIST_MOUSE && config_controller[deviceSlot][CONTRPS_BUF_SIZE][0] == CONTROLLER_PATCHER_VALUE_SET){
                bufSize = config_controller[deviceSlot][CONTRPS_BUF_SIZE][1];
            }
            unsigned char *buf = memalign(64,bufSize);
            memset(buf,0,bufSize);
            my_cb_user *usr = memalign(64,sizeof(my_cb_user));
            usr->buf = buf;
            usr->hid = device;

            usr->transfersize = p_device->max_packet_size_rx;
            usr->handle = p_device->handle;
            if(device != HID_LIST_MOUSE ){
                usr->deviceSlot = deviceSlot;
                memset(&gHID_Devices[deviceSlot],0,sizeof(HID_DEVICE_DATA));
                gHID_Devices[deviceSlot].user_data = usr;
            }else{
                memset(&gHID_Mouse,0,sizeof(HID_Mouse));
                gHID_Mouse.user_data = usr;
            }

            gHIDAttached |= device;
            gHIDCurrentDevice |= device;

            log_printf("%08X(%d) attached!\n",device,deviceSlot);

            if(device == HID_LIST_GC){ // GC PAD
                buf[0] = 0x13;
                HIDWrite(p_device->handle, usr->buf, 1, my_gc_write_cb, usr);
            }else if (device == HID_LIST_MOUSE){
                gHID_Mouse_Mode = HID_MOUSE_MODE_TOUCH;
                HIDRead(p_device->handle, buf, p_device->max_packet_size_rx, my_ms_read_cb, usr);
            }else if (device == HID_LIST_KEYBOARD){
                HIDSetProtocol(p_device->handle, p_device->interface_index, 1, 0, 0);
                HIDSetIdle(p_device->handle, p_device->interface_index, 0, 0, 0);
                HIDRead(p_device->handle, buf, p_device->max_packet_size_rx, my_read_cb, usr);
            }else if (device == HID_LIST_DS3){
                HIDSetProtocol(p_device->handle, p_device->interface_index, 1, 0, 0);
                buf[0] = 0x42; buf[1] = 0x0c; buf[2] = 0x00; buf[3] = 0x00;
                HIDSetReport(p_device->handle, HID_REPORT_FEATURE, PS3_F4_REPORT_ID, buf, PS3_F4_REPORT_LEN, NULL, NULL);
                HIDRead(p_device->handle, usr->buf, p_device->max_packet_size_rx, my_read_cb, usr);
            }else{
                HIDRead(p_device->handle, usr->buf, p_device->max_packet_size_rx, my_read_cb, usr);
            }
            return HID_DEVICE_ATTACH;

        }else{
            my_cb_user * user_data = NULL;
            if(device == HID_LIST_MOUSE){
                user_data = (my_cb_user *) gHID_Mouse.user_data;
            }else{
                user_data = (my_cb_user *) gHID_Devices[deviceSlot].user_data;
            }
            if(user_data){
                if(user_data->buf){
                    free(user_data->buf);
                    user_data->buf = NULL;
                }
                free(user_data);
                user_data = NULL;
            }
            gHIDAttached &= ~device;
            gHIDCurrentDevice &= ~device;
            if (device == HID_LIST_MOUSE){
                gHID_Mouse_Mode = HID_MOUSE_MODE_AIM;
            }
            log_printf("%d(%d) detached!\n",device,deviceSlot);
        }
    }else{
        log_print("HID-Device currently not supported!\n");
	}
	return HID_DEVICE_DETACH;
}

void setControllerReleasePressData(HID_Data_Struct data_cur, HID_Data_Struct data_last,VPADData * buffer,int VPADButton){
    int press = getButtonPressed(data_cur,VPADButton);
    if(press) buffer->btns_h |= VPADButton;
    if(press != getButtonPressed(data_last,VPADButton)){
        if(!press) buffer->btns_r |= VPADButton;
        if(press) buffer->btns_d |= VPADButton;
    }
}

u32 getEmulatedSticks(VPADData * buffer){
    u32 emulatedSticks = 0;
    int l_x_full = (buffer->lstick.x > 0.5f || buffer->lstick.x < -0.5f)? 1:0;
    int l_y_full = (buffer->lstick.y > 0.5f || buffer->lstick.y < -0.5f)? 1:0;
    int r_x_full = (buffer->rstick.x > 0.5f || buffer->rstick.x < -0.5f)? 1:0;
    int r_y_full = (buffer->rstick.y > 0.5f || buffer->rstick.y < -0.5f)? 1:0;

    if((buffer->lstick.x > 0.5f) || (buffer->lstick.x > 0.1f && !l_y_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_RIGHT;
    }
    if((buffer->lstick.x < -0.5f) || (buffer->lstick.x < -0.1f && !l_y_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_LEFT;
    }
    if((buffer->lstick.y > 0.5f) || (buffer->lstick.y > 0.1f && !l_x_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_UP;
    }
    if((buffer->lstick.y < -0.5f) || (buffer->lstick.y < -0.1f && !l_x_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_DOWN;
    }

    if((buffer->rstick.x > 0.5f) || (buffer->rstick.x > 0.1f && !r_y_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_RIGHT;
    }
    if((buffer->rstick.x < -0.5f) || (buffer->rstick.x < -0.1f && !r_y_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_LEFT;
    }
    if((buffer->rstick.y > 0.5f) || (buffer->rstick.y > 0.1f && !r_x_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_UP;
    }
    if((buffer->rstick.y < -0.5f) || (buffer->rstick.y < -0.1f && !r_x_full)){
        emulatedSticks |= VPAD_STICK_L_EMULATION_DOWN;
    }
    return emulatedSticks;
}

static u32 last_emulatedSticks = 0;

void setControllerDataFromHID(VPADData * buffer,int hid){
    int size = 0;
    HID_Data_Struct * data = getHIDDataAll(hid,&size);
    if(!data || !size || size < 0){
        if(data)free(data);
        return;
    }

    HID_Data_Struct data_cur;
    HID_Data_Struct data_last;
    for(int i = 0;i<size;i++){
        data_cur = data[i];

        HID_Data_Struct * data_last_ptr = getLastData(data_cur);
        if(!data_last_ptr) return;
        data_last = data_last_ptr[0];

        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_A);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_B);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_X);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_Y);

        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_LEFT);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_RIGHT);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_DOWN);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_UP);

        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_MINUS);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_L);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_R);

        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_PLUS);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_ZL);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_ZR);

        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_HOME);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_STICK_L);
        setControllerReleasePressData(data_cur,data_last,buffer,VPAD_BUTTON_STICK_R);

        convertAnalogSticks(data_cur,buffer);

        u32 emulatedSticks = getEmulatedSticks(buffer);

        //Setting the emulated sticks
        buffer->btns_h |= emulatedSticks;
        buffer->btns_d |= (emulatedSticks & (~last_emulatedSticks));
        buffer->btns_r |= (last_emulatedSticks & (~emulatedSticks));

        last_emulatedSticks = emulatedSticks;

        checkMouseMode(data_cur,data_last);

        setTouch(data_cur,buffer);

        setLastData(data_cur,data_last);

        if(data_last_ptr){
            free(data_last_ptr);
            data_last_ptr = NULL;
        }

    }
    if(data){
        free(data);
        data = NULL;
    }
    int current_rumblestatus = !!VPADBASEGetMotorOnRemainingCount(0);

    setRumble(hid,current_rumblestatus);
}


int getDevice(int hid){
    for(int i = 0;i < CONTRPD_MAX_VALUE;i++){
        if(hid & config_controller_list[i]){
              return i;
        }
    }
    return -1;
}

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Shared functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int getActivePad(int hid){
     if(hid & HID_LIST_GC){
        if (!(((gHID_Devices[CONTRPD_GC].pad_data[0].hid_data[0] & 0x10) == 0) && ((gHID_Devices[CONTRPD_GC].pad_data[0].hid_data[0] & 0x22) != 0x22))) return 0;
        if (!(((gHID_Devices[CONTRPD_GC].pad_data[1].hid_data[0] & 0x10) == 0) && ((gHID_Devices[CONTRPD_GC].pad_data[1].hid_data[0] & 0x22) != 0x22))) return 1;
        if (!(((gHID_Devices[CONTRPD_GC].pad_data[2].hid_data[0] & 0x10) == 0) && ((gHID_Devices[CONTRPD_GC].pad_data[2].hid_data[0] & 0x22) != 0x22))) return 2;
        if (!(((gHID_Devices[CONTRPD_GC].pad_data[3].hid_data[0] & 0x10) == 0) && ((gHID_Devices[CONTRPD_GC].pad_data[3].hid_data[0] & 0x22) != 0x22))) return 3;
        return -1;
     }
     return 0;
}

void setRumble(int hid,int rumble){
    int device = getDevice(hid);
    if(device == -1) return;
    int pad = getActivePad(hid);
    if(pad < 0) return;
    ((HID_Data *)config_controller_data_ptr[device][pad])->rumbleActive = rumble;
}

HID_Data_Struct * getHIDDataAll(int hid,int * size){
    *size = 0;
    int bit = 1;
    for (int i = 0;i < CONTRPD_MAX_VALUE; i++){
       *size += (hid & bit)>>i;
       bit <<= 1;
    }

    HID_Data_Struct * data = malloc(sizeof(HID_Data_Struct)*(*size));
    if(!data) return NULL;
    int i = 0;

    //!Mouse always need extra treatment
    if(hid & HID_LIST_MOUSE){
        unsigned char * src = (unsigned char *) &(gHID_Mouse.pad_data[getActivePad(HID_LIST_MOUSE)].data[0]);
        data[i].src = src;
        data[i].hid = HID_LIST_MOUSE;
        i++;
    }

    for(int j = 0;j < CONTRPD_MAX_VALUE;j++){
        if(hid & config_controller_list[j]){
            int pad = getActivePad(config_controller_list[j]);
            if(pad < 0){ //Not pad connected to adapter
                (*size)--;
                continue;
            }
            data[i].hid = config_controller_list[j];
            data[i].src = (u8*)&(((HID_Data *)config_controller_data_ptr[j][pad])->hid_data);
            if(!data[i].src){
                (*size)--;
                continue;
            }
            i++;
        }
    }
    return data;
}

int getPadCount(int hid){
    int device = getDevice(hid);
    if(device == -1) return 0;
    if(config_controller[device][CONTRPS_PAD_COUNT][0] == CONTROLLER_PATCHER_VALUE_SET){
        return config_controller[device][CONTRPS_PAD_COUNT][1];
    }

    return 0;
}


void checkMouseMode(HID_Data_Struct data,HID_Data_Struct data_last){
    unsigned char * src = data.src;
    unsigned char * src_last = data_last.src;
    int hid = data.hid;

    if(hid & HID_LIST_KEYBOARD){
        if(wasInKeyboard(src,HID_KEYBOARD_BUTTON_F1) && (wasInKeyboard(src,HID_KEYBOARD_BUTTON_F1) != wasInKeyboard(src_last,HID_KEYBOARD_BUTTON_F1))){
            if(gHID_Mouse_Mode == HID_MOUSE_MODE_AIM){
                gHID_Mouse_Mode = HID_MOUSE_MODE_TOUCH;
                log_print("Mouse mode changed! to touch \n");
            }else if(gHID_Mouse_Mode == HID_MOUSE_MODE_TOUCH){
                log_print("Mouse mode changed! to aim \n");
                gHID_Mouse_Mode = HID_MOUSE_MODE_AIM;
            }
        }
    }
}

void setLastData(HID_Data_Struct data,HID_Data_Struct data_last){
    unsigned char * src = data.src;
    unsigned char * src_last = data_last.src;
    int hid = data.hid;

    if(!src || !src_last) return;

    if(hid & HID_LIST_MOUSE){
        memcpy(src_last,src,sizeof(HID_Mouse_Data));
    }

    memcpy(src_last,src,HID_MAX_DATA_LENGTH_PER_PAD);
}


HID_Data_Struct * getLastData(HID_Data_Struct data){
    int hid = data.hid;
    unsigned char * src = data.src;
    HID_Data_Struct * last_data = malloc(sizeof(HID_Data_Struct));
    if(!last_data) return NULL;

    if(hid & HID_LIST_MOUSE){
        last_data->hid = HID_LIST_KEYBOARD;
        last_data->src = (unsigned char *) &(gHID_Mouse.pad_data[0].data[1]);// further testing
        return last_data;
    }

    int device = getDevice(hid);
    if(device == -1) return NULL;

    last_data->hid =       config_controller_list[device];
    last_data->src = src + HID_MAX_DATA_LENGTH_PER_PAD;
    return last_data;
}

int getButtonPressed(HID_Data_Struct data, int VPADButton){
    unsigned char * src = data.src;
    int hid = data.hid;


   if(hid & HID_LIST_MOUSE){
        if(gHID_Mouse_Mode == HID_MOUSE_MODE_TOUCH){
            HID_Mouse_Data *  ms_data = (HID_Mouse_Data *) src;
            if(VPADButton == VPAD_BUTTON_TOUCH){
                if(ms_data->left_click & 0x01){
                        return 1;
                }
            }
        }else if(gHID_Mouse_Mode == HID_MOUSE_MODE_AIM){
            HID_Mouse_Data *  ms_data = (HID_Mouse_Data *) src;
            if(VPADButton == VPAD_BUTTON_ZR){
                if(ms_data->left_click & 0x01){
                        return 1;
                }
            }else if(VPADButton == VPAD_BUTTON_R){
                if(ms_data->right_click & 0x01){
                        return 1;
                }
            }
        }
        return 0;
    }

    int device = getDevice(hid);
    if(device == -1) return 0;

    int cur_config = 0;

    if(VPADButton == VPAD_BUTTON_A){
        cur_config = CONTRPS_VPAD_BUTTON_A;
    }else if(VPADButton == VPAD_BUTTON_B){
        cur_config = CONTRPS_VPAD_BUTTON_B;
    }else if(VPADButton == VPAD_BUTTON_X){
        cur_config = CONTRPS_VPAD_BUTTON_X;
    }else if(VPADButton == VPAD_BUTTON_Y){
        cur_config = CONTRPS_VPAD_BUTTON_Y;
    }else if(VPADButton == VPAD_BUTTON_L){
        cur_config = CONTRPS_VPAD_BUTTON_L;
    }else if(VPADButton == VPAD_BUTTON_R){
        cur_config = CONTRPS_VPAD_BUTTON_R;
    }else if(VPADButton == VPAD_BUTTON_ZL){
        cur_config = CONTRPS_VPAD_BUTTON_ZL;
    }else if(VPADButton == VPAD_BUTTON_ZR){
        cur_config = CONTRPS_VPAD_BUTTON_ZR;
    }else if(VPADButton == VPAD_BUTTON_STICK_L){
        cur_config = CONTRPS_VPAD_BUTTON_STICK_L;
    }else if(VPADButton == VPAD_BUTTON_STICK_R){
        cur_config = CONTRPS_VPAD_BUTTON_STICK_R;
    }else if(VPADButton == VPAD_BUTTON_PLUS){
        cur_config = CONTRPS_VPAD_BUTTON_PLUS;
    }else if(VPADButton == VPAD_BUTTON_MINUS){
        cur_config = CONTRPS_VPAD_BUTTON_MINUS;
    }else if(VPADButton == VPAD_BUTTON_HOME){
        cur_config = CONTRPS_VPAD_BUTTON_HOME;
    }

    //! Special DPAD treatment.
    if(config_controller[device][CONTRPS_DPAD_MODE][0] == CONTROLLER_PATCHER_VALUE_SET){
        if(config_controller[device][CONTRPS_DPAD_MODE][1] == CONTRPDM_Hat){
            u8 mask = 0x0F;
            if(config_controller[device][CONTRPS_DPAD_MASK][0] == CONTROLLER_PATCHER_VALUE_SET){
               mask = config_controller[device][CONTRPS_DPAD_MASK][1];
            }
            if(src[config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL][0]] != config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL][1]){ // Not neutral
                u8 dir1_0 = 0,dir1_1 = 0;
                u8 dir2_0 = 0,dir2_1 = 0;
                u8 dir3_0 = 0,dir3_1 = 0;
                u8 direction = 0;
                if(VPADButton == VPAD_BUTTON_LEFT){
                    dir1_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_W][0];
                    dir2_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NW][0];
                    dir3_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SW][0];
                    dir1_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_W][1];
                    dir2_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NW][1];
                    dir3_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SW][1];
                    direction = 1;
                }else if(VPADButton == VPAD_BUTTON_RIGHT){
                    dir1_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_E][0];
                    dir2_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SE][0];
                    dir3_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NE][0];
                    dir1_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_E][1];
                    dir2_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SE][1];
                    dir3_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NE][1];
                    direction = 1;
                }else if(VPADButton == VPAD_BUTTON_DOWN){
                    dir1_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_S][0];
                    dir2_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SE][0];
                    dir3_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SW][0];
                    dir1_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_S][1];
                    dir2_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SE][1];
                    dir3_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_SW][1];
                    direction = 1;
                }else if(VPADButton == VPAD_BUTTON_UP){
                    dir1_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_N][0];
                    dir2_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NW][0];
                    dir3_0 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NE][0];
                    dir1_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_N][1];
                    dir2_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NW][1];
                    dir3_1 = config_controller[device][CONTRPS_VPAD_BUTTON_DPAD_NE][1];
                    direction = 1;
                }
                if(direction && (((src[dir1_0] & mask) == dir1_1) ||
                        ((src[dir2_0] & mask) == dir2_1) ||
                        ((src[dir3_0] & mask) == dir3_1))) return 1;
            }

        }
    }

    //! Normal DPAD treatment.
    if(VPADButton == VPAD_BUTTON_LEFT){
        cur_config = CONTRPS_VPAD_BUTTON_LEFT;
    }else if(VPADButton == VPAD_BUTTON_RIGHT){
        cur_config = CONTRPS_VPAD_BUTTON_RIGHT;
    }else if(VPADButton == VPAD_BUTTON_DOWN){
        cur_config = CONTRPS_VPAD_BUTTON_DOWN;
    }else if(VPADButton == VPAD_BUTTON_UP){
        cur_config = CONTRPS_VPAD_BUTTON_UP;
    }

    if(config_controller[device][CONTRPS_DOUBLE_USE][0] == CONTROLLER_PATCHER_VALUE_SET){
        if(config_controller[device][CONTRPS_DOUBLE_USE][1] == CONTROLLER_PATCHER_GC_DOUBLE_USE){
            if(src[config_controller[device][CONTRPS_DOUBLE_USE_BUTTON][0]] & config_controller[device][CONTRPS_DOUBLE_USE_BUTTON][1]){
                if(cur_config == CONTRPS_VPAD_BUTTON_PLUS || cur_config == CONTRPS_VPAD_BUTTON_ZL || cur_config == CONTRPS_VPAD_BUTTON_ZR || cur_config == CONTRPS_VPAD_BUTTON_A || cur_config == CONTRPS_VPAD_BUTTON_B){
                    return 0;
                }
            }else{
                if(cur_config == CONTRPS_VPAD_BUTTON_MINUS || cur_config == CONTRPS_VPAD_BUTTON_L || cur_config == CONTRPS_VPAD_BUTTON_R || cur_config == CONTRPS_VPAD_BUTTON_STICK_L || cur_config == CONTRPS_VPAD_BUTTON_STICK_R){
                    return 0;
                }
            }
        }
    }
    //log_printf("Check button %08X \n",VPADButton);
    if(config_controller[device][cur_config][0] != 0xFF){ //Invalid data
        if(hid & HID_LIST_KEYBOARD){
            if(wasInKeyboard(src,config_controller[device][cur_config][1])) return 1;
        }else{
            //log_printf("src[%02X] = %02X & %02X\n",config_controller[device][cur_config][0],src[config_controller[device][cur_config][0]],config_controller[device][cur_config][1]);
            //log_printf("%02X %02X %02X %02X %02X %02X %02X %02X | %d = %08X? \n",src[0],src[1],src[2],src[3],src[4],src[5],src[6],src[7],config_controller[device][cur_config][0],config_controller[device][cur_config][1]);
            if(src[config_controller[device][cur_config][0]] & config_controller[device][cur_config][1]){
                return 1;
            }

        }
    }
    return 0;
}

void setTouch(HID_Data_Struct data,VPADData * buffer){
    unsigned char * src = data.src;
    int hid = data.hid;
    if(hid & HID_LIST_MOUSE && gHID_Mouse_Mode == HID_MOUSE_MODE_TOUCH){
        if(getButtonPressed(data,VPAD_BUTTON_TOUCH)){
           HID_Mouse_Data *  ms_data = (HID_Mouse_Data *) src;
           int x_mouse = 80 + ((int)(((ms_data->X)*1.0f/1280.0)*3890.0f));
           int y_mouse = 3910 - ((int)(((ms_data->Y)*1.0f/720.0)*3760.0f));
           buffer->tpdata.x = x_mouse;
           buffer->tpdata.y = y_mouse;
           buffer->tpdata.touched = 1;
           buffer->tpdata.invalid = 0;
           buffer->tpdata1.x = x_mouse;
           buffer->tpdata1.y = y_mouse;
           buffer->tpdata1.touched = 1;
           buffer->tpdata1.invalid = 0;
           buffer->tpdata2.x = x_mouse;
           buffer->tpdata2.y = y_mouse;
           buffer->tpdata2.touched = 1;
           buffer->tpdata2.invalid = 0;
        }
    }
}

f32 convertAnalogValue(u8 value, u8 default_val, u8 min, u8 max, u8 invert,u8 deadzone){
    s8 new_value = (s8)(value - default_val);
    u8 range = 0;
    if((value-deadzone) > default_val){
        range = (max - default_val);
    }else if((value+deadzone) < default_val){
        range = (default_val - min);
    }else{
        return 0.0f;
    }
    if(invert != 0x01){
        return (new_value / (1.0f*range));
    }else{
        return -1.0f*(new_value / (1.0f*range));
    }
}

void convertAnalogSticks(HID_Data_Struct data, VPADData * buffer){
    unsigned char * src = data.src;
    int hid = data.hid;
    int changed = 0;
    if (hid & HID_LIST_MOUSE){
        if(gHID_Mouse_Mode == HID_MOUSE_MODE_AIM){ // TODO: tweak values
            HID_Mouse_Data *  ms_data = (HID_Mouse_Data *) src;
            buffer->rstick.x +=  ms_data->deltaX/10.0f;
            buffer->rstick.y +=  -1.0f*(ms_data->deltaY/10.0f);
            changed = 1;
        }
    }else{
        int device = getDevice(hid);
        if(device == -1) return;

        if (hid & HID_LIST_KEYBOARD){

            int w,s,a,d;
            w = wasInKeyboard(src,HID_KEYBOARD_BUTTON_W);
            s = wasInKeyboard(src,HID_KEYBOARD_BUTTON_S);
            a = wasInKeyboard(src,HID_KEYBOARD_BUTTON_A);
            d = wasInKeyboard(src,HID_KEYBOARD_BUTTON_D);

            if(w){
                if(!s){
                    buffer->lstick.y = 1.0f;
                }
                if(a || d){
                    buffer->lstick.y = 0.75f;
                    if(a) buffer->lstick.x = -0.75f;
                    if(d) buffer->lstick.x = 0.75f;
                }
                changed = 1;
            }else if(s){
                if(!w){
                    buffer->lstick.y = -1.0f;
                }
                if(a || d){
                    buffer->lstick.y = -0.75f;
                    if(a) buffer->lstick.x = -0.75f;
                    if(d) buffer->lstick.x = 0.75f;
                }
                changed = 1;
            }else{
                if(a){
                     if(!d){
                        buffer->lstick.x = -1.0f;
                        changed = 1;
                     }

                }else if(d){
                    if(!s){
                        buffer->lstick.x = 1.0f;
                        changed = 1;
                     }
                }
            }
        }else{
            int deadzone = 0;
            if(config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X_DEADZONE][0] == CONTROLLER_PATCHER_VALUE_SET){
                 deadzone = config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X_DEADZONE][1];
            }
            buffer->lstick.x += convertAnalogValue(src[config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X][0]],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX][0],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X_INVERT][1],
                                                   deadzone);
           deadzone = 0;
           if(config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y_DEADZONE][0] == CONTROLLER_PATCHER_VALUE_SET){
                 deadzone = config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y_DEADZONE][1];
           }
           buffer->lstick.y += convertAnalogValue(src[config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y][0]],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX][0],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y_INVERT][1],
                                                   deadzone);
           deadzone = 0;
           if(config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X_DEADZONE][0] == CONTROLLER_PATCHER_VALUE_SET){
                 deadzone = config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X_DEADZONE][1];
           }

           buffer->rstick.x += convertAnalogValue(src[config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X][0]],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX][0],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X_INVERT][1],
                                                   deadzone);
           deadzone = 0;
           if(config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y_DEADZONE][0] == CONTROLLER_PATCHER_VALUE_SET){
                 deadzone = config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y_DEADZONE][1];
           }

           buffer->rstick.y += convertAnalogValue(src[config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y][0]],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX][0],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX][1],
                                                   config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y_INVERT][1],
                                                   deadzone);
            changed = 1;
            /*log_printf("LX %f(%02X) LY %f(%02X) RX %f(%02X) RY %f(%02X)\n",buffer->lstick.x,src[config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_X][0]],
                                                                   buffer->lstick.y,src[config_controller[device][CONTRPS_VPAD_BUTTON_L_STICK_Y][0]],
                                                                   buffer->rstick.x,src[config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_X][0]],
                                                                   buffer->rstick.y,src[config_controller[device][CONTRPS_VPAD_BUTTON_R_STICK_Y][0]]);*/
        }
    }
    if(changed){
        if(buffer->rstick.x > 1.0f) buffer->rstick.x = 1.0f;
        if(buffer->rstick.y > 1.0f) buffer->rstick.y = 1.0f;
        if(buffer->rstick.x < -1.0f) buffer->rstick.x = -1.0f;
        if(buffer->rstick.y < -1.0f) buffer->rstick.y = -1.0f;
    }

}

int wasInKeyboard(unsigned char * src,int key){
    for(int i = 0;i<HID_KEYBOARD_DATA_LENGTH;i++){
        if(src[i] == 0 && i > 1){
             break;
        }else if (src[i] == key){
            return 1;
        }
    }
    return 0;
}


//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Mouse
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define MOUSE_XY_TYPE_A 0x01  //1 byte x, 1 byte y
#define MOUSE_XY_TYPE_B 0x02  //1 byte x, 2 byte y

void my_ms_read_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user)
{
	if(error == 0)
	{
        //log_printf("%02X %02X %02X %02X %02X %02X %02X %02X  bytes_transfered: %d\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],bytes_transfered);
        int type = MOUSE_XY_TYPE_A;
        int click_offset = 0;
        int xy_offset = 1;

	    if(bytes_transfered == 5){
            type = MOUSE_XY_TYPE_B;
            click_offset = 0;
            xy_offset = 1;
	    }else if(bytes_transfered == 6){
            type = MOUSE_XY_TYPE_A;
            xy_offset = 2;
            click_offset = 1;
	    }else if(bytes_transfered == 7){
            type = MOUSE_XY_TYPE_B;
            xy_offset = 2;
            click_offset = 1;
	    }else if(bytes_transfered == 8){
            type = MOUSE_XY_TYPE_B;
            xy_offset = 3;
            click_offset = 1;
	    }

	    my_cb_user *usr = (my_cb_user*)p_user;

	    gHID_Mouse.pad_data[0].data[0].left_click = buf[click_offset];
	    gHID_Mouse.pad_data[0].data[0].right_click = buf[click_offset]>>1;

        s8 x_value = 0;
        s8 y_value = 0;

        if(type == MOUSE_XY_TYPE_A){
            x_value = buf[xy_offset];
            y_value = buf[xy_offset+1];
        }else if(type == MOUSE_XY_TYPE_B){
            x_value = buf[xy_offset];
            y_value = ((buf[xy_offset+1] & 0xF0) >>4) | ((buf[xy_offset+2] & 0x0F) <<4);
        }

        gHID_Mouse.pad_data[0].data[0].X += x_value;
        gHID_Mouse.pad_data[0].data[0].deltaX = x_value;

        gHID_Mouse.pad_data[0].data[0].Y += y_value;
        gHID_Mouse.pad_data[0].data[0].deltaY = y_value;


        if(gHID_Mouse.pad_data[0].data[0].X < 0) gHID_Mouse.pad_data[0].data[0].X = 0;
        if(gHID_Mouse.pad_data[0].data[0].X > 1280) gHID_Mouse.pad_data[0].data[0].X = 1280;

        if(gHID_Mouse.pad_data[0].data[0].Y < 0) gHID_Mouse.pad_data[0].data[0].Y = 0;
        if(gHID_Mouse.pad_data[0].data[0].Y > 720) gHID_Mouse.pad_data[0].data[0].Y = 720;

        //log_printf("%02X %02X %02X %02X %02X %02X %02X %02X %d = X: %d Y: %d \n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],bytes_transfered,x_value,y_value);

        HIDRead(handle, usr->buf, bytes_transfered, my_ms_read_cb, usr);
	}
}


//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! GC-Adapter
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void my_read_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user)
{
	if(error == 0 && p_user != NULL && gHIDAttached)
	{
		my_cb_user *usr = (my_cb_user*)p_user;

		if(usr->hid == HID_LIST_GC){
            memcpy(&(((HID_Data *)config_controller_data_ptr[CONTRPD_GC][0])->hid_data),&buf[1],9);
            memcpy(&(((HID_Data *)config_controller_data_ptr[CONTRPD_GC][1])->hid_data),&buf[10],9);
            memcpy(&(((HID_Data *)config_controller_data_ptr[CONTRPD_GC][2])->hid_data),&buf[19],9);
            memcpy(&(((HID_Data *)config_controller_data_ptr[CONTRPD_GC][3])->hid_data),&buf[28],9);


            /*
            int i = 0;
            log_printf("GC1: %02X %02X %02X %02X %02X %02X %02X %02X %02X ",buf[i*9+1],buf[i*9+2],buf[i*9+3],buf[i*9+4],buf[i*9+5],buf[i*9+6],buf[i*9+7],buf[i*9+8],buf[i*9+9]);i++;
            log_printf("GC2: %02X %02X %02X %02X %02X %02X %02X %02X %02X ",buf[i*9+1],buf[i*9+2],buf[i*9+3],buf[i*9+4],buf[i*9+5],buf[i*9+6],buf[i*9+7],buf[i*9+8],buf[i*9+9]);i++;
            log_printf("GC3: %02X %02X %02X %02X %02X %02X %02X %02X %02X ",buf[i*9+1],buf[i*9+2],buf[i*9+3],buf[i*9+4],buf[i*9+5],buf[i*9+6],buf[i*9+7],buf[i*9+8],buf[i*9+9]);i++;
            log_printf("GC4: %02X %02X %02X %02X %02X %02X %02X %02X %02X \n",buf[i*9+1],buf[i*9+2],buf[i*9+3],buf[i*9+4],buf[i*9+5],buf[i*9+6],buf[i*9+7],buf[i*9+8],buf[i*9+9]);*/

            HIDRead(handle, usr->buf, bytes_transfered, my_read_cb, usr);
            int pad = getActivePad(HID_LIST_GC);

            //log_printf("Pad attached: %02X \n",getActivePad(HID_LIST_GC));

            if((pad >= 0) && (((HID_Data *)config_controller_data_ptr[CONTRPD_GC][pad])->rumbleActive != usr->rumblestatus[pad]))
            {
                usr->rumblestatus[pad] = ((HID_Data *)config_controller_data_ptr[CONTRPD_GC][pad])->rumbleActive;
                HIDGCRumble(handle,usr,usr->rumblestatus[pad],pad);
            }
            else
            {
                HIDRead(handle, usr->buf, bytes_transfered, my_read_cb, usr);
            }

		}else if(usr->hid != 0){ //
            int size = (HID_MAX_DATA_LENGTH_PER_PAD > bytes_transfered)? bytes_transfered : HID_MAX_DATA_LENGTH_PER_PAD;
            memcpy(&(((HID_Data *)config_controller_data_ptr[usr->deviceSlot][0])->hid_data),&buf[0],size);
            usleep(5000); //DS4 is way tooo fast (need to check the other pads)
            if(usr->hid == HID_LIST_POKKEN){
                 log_printf("HID %02X %02X %02X %02X %02X %02X %02X %02X \n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]); //<-- only debug the pokken controller
            }
            HIDRead(handle, usr->buf, bytes_transfered, my_read_cb, usr);
		}
	}
}

void HIDGCRumble(unsigned int handle,my_cb_user *usr,int rumble, u32 pad)
{
	usr->buf[0] = 0x11;
    usr->buf[1] = (pad+1) & rumble;	usr->buf[2] = ((pad+1) >> 1) & rumble;
	usr->buf[3] = ((pad+1) >> 2) & rumble;
	usr->buf[4] = ((pad+1) >> 3) & rumble;
    HIDWrite(handle, usr->buf, 5, my_gc_write_cb, usr);
}

void my_gc_write_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user)
{
	if(error == 0)
	{
		my_cb_user *usr = (my_cb_user*)p_user;
		HIDRead(handle, usr->buf, usr->transfersize, my_read_cb, usr);
	}
}
