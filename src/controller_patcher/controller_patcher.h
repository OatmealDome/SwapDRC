#ifndef _CONTROLLER_PATCHER_H_
#define _CONTROLLER_PATCHER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common/types.h"
#include "common/common.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/syshid_functions.h"
#include "dynamic_libs/vpad_functions.h"


//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Don't forget to change this data!
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

enum Controller_Patcher_Devices
{
    CONTRPD_GC,
    CONTRPD_DS3,
    CONTRPD_DS4,
    CONTRPD_SP2600,
    CONTRPD_KEYBOARD,
    CONTRPD_PS2_ADAPTER,
    CONTRPD_POKKEN,
    CONTRPD_PS2,
    CONTRPD_MAX_VALUE
};

#define HID_LIST_GC                     0x001
#define HID_LIST_KEYBOARD               0x002
#define HID_LIST_MOUSE                  0x004
#define HID_LIST_DS3                    0x008
#define HID_LIST_DS4                    0x010
#define HID_LIST_SP2600                 0x020
#define HID_LIST_PS2_ADAPTER            0x040
#define HID_LIST_POKKEN                 0x080
#define HID_LIST_PS2                    0x100

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! End
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_MAX_DATA_LENGTH_PER_PAD             10
#define HID_MAX_PADS_COUNT                      4

typedef struct _HID_Data {
    u8 rumbleActive;
    u8 hid_data[HID_MAX_DATA_LENGTH_PER_PAD*2];
} HID_Data;

//! most data has the format: byte,value (byte starting at 0)
enum Controller_Patcher_Settings
{
    CONTRPS_VID,                          //! pid: 0x451d would be 0x45,0x1d
    CONTRPS_PID,                          //! vid: 0x488d would be 0x48,0x8d
    CONTRPS_BUF_SIZE,                     //! To invert: CONTROLLER_PATCHER_VALUE_SET, BUF_SIZE (default is 64)
    CONTRPS_VPAD_BUTTON_A,
    CONTRPS_VPAD_BUTTON_B,
    CONTRPS_VPAD_BUTTON_X,
    CONTRPS_VPAD_BUTTON_Y,
    CONTRPS_DPAD_MODE,                     //! To set mode: CONTROLLER_PATCHER_VALUE_SET, Controller_Patcher_DPAD_MODE (default is normal mode)
    CONTRPS_DPAD_MASK,                     //! Mask needed for hat mode: CONTROLLER_PATCHER_VALUE_SET, mask
    /* Normal DPAD */
    CONTRPS_VPAD_BUTTON_LEFT,
    CONTRPS_VPAD_BUTTON_RIGHT,
    CONTRPS_VPAD_BUTTON_UP,
    CONTRPS_VPAD_BUTTON_DOWN,
    /* DPAD hat mode */
    CONTRPS_VPAD_BUTTON_DPAD_N,
    CONTRPS_VPAD_BUTTON_DPAD_NE,
    CONTRPS_VPAD_BUTTON_DPAD_E,
    CONTRPS_VPAD_BUTTON_DPAD_SE,
    CONTRPS_VPAD_BUTTON_DPAD_S,
    CONTRPS_VPAD_BUTTON_DPAD_SW,
    CONTRPS_VPAD_BUTTON_DPAD_W,
    CONTRPS_VPAD_BUTTON_DPAD_NW,
    CONTRPS_VPAD_BUTTON_DPAD_NEUTRAL,
    /* */
    CONTRPS_VPAD_BUTTON_ZL,
    CONTRPS_VPAD_BUTTON_ZR,
    CONTRPS_VPAD_BUTTON_L,
    CONTRPS_VPAD_BUTTON_R,
    CONTRPS_VPAD_BUTTON_PLUS,
    CONTRPS_VPAD_BUTTON_MINUS,
    CONTRPS_VPAD_BUTTON_HOME,
    CONTRPS_VPAD_BUTTON_SYNC,
    CONTRPS_VPAD_BUTTON_STICK_R,
    CONTRPS_VPAD_BUTTON_STICK_L,
    CONTRPS_VPAD_BUTTON_L_STICK_X,          //! byte, default value
    CONTRPS_VPAD_BUTTON_L_STICK_X_INVERT,   //! To invert: CONTROLLER_PATCHER_VALUE_SET, 0x01
    CONTRPS_VPAD_BUTTON_L_STICK_X_DEADZONE, //! Deadzone
    CONTRPS_VPAD_BUTTON_L_STICK_X_MINMAX,   //! min,max
    CONTRPS_VPAD_BUTTON_L_STICK_Y,          //! byte, default value
    CONTRPS_VPAD_BUTTON_L_STICK_Y_INVERT,   //! To invert: CONTROLLER_PATCHER_VALUE_SET, 0x01
    CONTRPS_VPAD_BUTTON_L_STICK_Y_DEADZONE, //! Deadzone
    CONTRPS_VPAD_BUTTON_L_STICK_Y_MINMAX,   //! min,max
    CONTRPS_VPAD_BUTTON_R_STICK_X,          //! byte, default value
    CONTRPS_VPAD_BUTTON_R_STICK_X_INVERT,   //! To invert: CONTROLLER_PATCHER_VALUE_SET, 0x01
    CONTRPS_VPAD_BUTTON_R_STICK_X_DEADZONE, //! Deadzone
    CONTRPS_VPAD_BUTTON_R_STICK_X_MINMAX,   //! min,max
    CONTRPS_VPAD_BUTTON_R_STICK_Y,          //! byte, default value
    CONTRPS_VPAD_BUTTON_R_STICK_Y_INVERT,   //! To invert: CONTROLLER_PATCHER_VALUE_SET, 0x01
    CONTRPS_VPAD_BUTTON_R_STICK_Y_DEADZONE, //! Deadzone
    CONTRPS_VPAD_BUTTON_R_STICK_Y_MINMAX,   //! min,max
    CONTRPS_VPAD_BUTTON_TV,
    CONTRPS_DOUBLE_USE,                     //!When used: e.g. CONTROLLER_PATCHER_VALUE_SET, CONTROLLER_PATCHER_GC_DOUBLE_USE
    CONTRPS_DOUBLE_USE_BUTTON,
    CONTRPS_PAD_COUNT,                      //!
    CONTRPS_MAX_VALUE
};

enum Controller_Patcher_DPAD_MODE
{
    CONTRPDM_Normal,
    CONTRPDM_Hat
};

#define CONTROLLER_PATCHER_VALUE_SET            0x01
#define CONTROLLER_PATCHER_GC_DOUBLE_USE        0x01
#define CONTROLLER_PATCHER_INVALIDVALUE         0xFF

typedef struct _my_cb_user{
	unsigned char *buf;
	unsigned int transfersize;
	unsigned int rumblestatus[4];
	unsigned int handle;
	unsigned int hid;
	unsigned int deviceSlot;
}my_cb_user;

typedef struct _HID_DEVICE_DATA {
    my_cb_user * user_data;
    HID_Data pad_data[HID_MAX_PADS_COUNT];
} HID_DEVICE_DATA;



typedef struct _HID_Data_Struct{
	unsigned char *src;
	int hid;
}HID_Data_Struct;

void setConfigValue(u8 * dest , u8 first, u8 second);
void init_config_controller();
void deinit_config_controller();

void setControllerReleasePressData(HID_Data_Struct data_cur, HID_Data_Struct data_last,VPADData * buffer,int VPADButton);
void setControllerDataFromHID(VPADData * buffer,int hid);
void checkMouseMode(HID_Data_Struct data, HID_Data_Struct data_last);

void my_read_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user);

int wasInKeyboard(unsigned char * src,int key);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! GC-Adapter
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//A config would be cleaner, but no motivation atm :P
#define HID_GC_BUTTON_A                 0x01
#define HID_GC_BUTTON_B                 0x02
#define HID_GC_BUTTON_X                 0x04
#define HID_GC_BUTTON_Y                 0x08
#define HID_GC_BUTTON_LEFT              0x10
#define HID_GC_BUTTON_RIGHT             0x20
#define HID_GC_BUTTON_DOWN              0x40
#define HID_GC_BUTTON_UP                0x80
#define HID_GC_BUTTON_START             0x01
#define HID_GC_BUTTON_L                 0x80
#define HID_GC_BUTTON_R                 0x80
#define HID_GC_BUTTON_Z                 0x02

#define HID_GC_PAD_COUNT             4

void HIDGCRumble(unsigned int handle,my_cb_user *usr,int rumble, u32 pad);
void my_gc_write_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! DS3
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define PS3_F4_REPORT_LEN               4
#define PS3_F5_REPORT_LEN               8
#define PS3_01_REPORT_LEN               48
#define HID_REPORT_FEATURE              3
#define HID_REPORT_OUTPUT               2
#define PS3_F4_REPORT_ID                0xF4
#define PS3_01_REPORT_ID                0x01
#define PS3_F5_REPORT_ID                0xF5

#define HID_DS3_BUTTON_CROSS            0x40 // 3
#define HID_DS3_BUTTON_CIRCLE           0x20 // 3
#define HID_DS3_BUTTON_SQUARE           0x80 // 3
#define HID_DS3_BUTTON_TRIANGLE         0x10 // 3
#define HID_DS3_BUTTON_L1               0x04 // 3
#define HID_DS3_BUTTON_L2               0x01 // 3
#define HID_DS3_BUTTON_R1               0x08 // 3
#define HID_DS3_BUTTON_R2               0x02 // 3

#define HID_DS3_BUTTON_L3               0x02 // 2
#define HID_DS3_BUTTON_R3               0x04 // 2
#define HID_DS3_BUTTON_SELECT           0x01 // 2
#define HID_DS3_BUTTON_START            0x08 // 2
#define HID_DS3_BUTTON_LEFT             0x80 // 2
#define HID_DS3_BUTTON_RIGHT            0x20 // 2
#define HID_DS3_BUTTON_UP               0x10 // 2
#define HID_DS3_BUTTON_DOWN             0x40 // 2
#define HID_DS3_BUTTON_GUIDE            0x01 // 4

#define HID_DS3_PAD_COUNT             1

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! DS4
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_DS4_BUTTON_CROSS            0x20 // 5
#define HID_DS4_BUTTON_SQUARE           0x10 // 5
#define HID_DS4_BUTTON_CIRCLE           0x40 // 5
#define HID_DS4_BUTTON_TRIANGLE         0x80 // 5
#define HID_DS4_BUTTON_L1               0x01 // 6
#define HID_DS4_BUTTON_L2               0x04 // 6
#define HID_DS4_BUTTON_L3               0x40 // 6
#define HID_DS4_BUTTON_R1               0x02 // 6
#define HID_DS4_BUTTON_R2               0x08 // 6
#define HID_DS4_BUTTON_R3               0x80 // 6
#define HID_DS4_BUTTON_SHARE            0x10 // 6
#define HID_DS4_BUTTON_OPTIONS          0x20 // 6
#define HID_DS4_BUTTON_DPAD_N           0x00 // 5
#define HID_DS4_BUTTON_DPAD_NE          0x01 // 5
#define HID_DS4_BUTTON_DPAD_E           0x02 // 5
#define HID_DS4_BUTTON_DPAD_SE          0x03 // 5
#define HID_DS4_BUTTON_DPAD_S           0x04 // 5
#define HID_DS4_BUTTON_DPAD_SW          0x05 // 5
#define HID_DS4_BUTTON_DPAD_W           0x06 // 5
#define HID_DS4_BUTTON_DPAD_NW          0x07 // 5
#define HID_DS4_BUTTON_DPAD_NEUTRAL     0x08 // 5
#define HID_DS4_BUTTON_GUIDE            0x01 // 7
#define HID_DS4_BUTTON_T_PAD_CLICK      0x02 // 7

#define HID_DS4_PAD_COUNT               1

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! PS2 Adapter
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_PS2_ADAPTER_BUTTON_CROSS            0x40 // 5
#define HID_PS2_ADAPTER_BUTTON_CIRCLE           0x20 // 5
#define HID_PS2_ADAPTER_BUTTON_SQUARE           0x80 // 5
#define HID_PS2_ADAPTER_BUTTON_TRIANGLE         0x10 // 5

#define HID_PS2_ADAPTER_BUTTON_DPAD_N           0x00 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_NE          0x01 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_E           0x02 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_SE          0x03 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_S           0x04 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_SW          0x05 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_W           0x06 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_NW          0x07 // 5
#define HID_PS2_ADAPTER_BUTTON_DPAD_NEUTRAL     0x0F // 5

#define HID_PS2_ADAPTER_BUTTON_L1               0x04 // 6
#define HID_PS2_ADAPTER_BUTTON_L2               0x01 // 6
#define HID_PS2_ADAPTER_BUTTON_R1               0x08 // 6
#define HID_PS2_ADAPTER_BUTTON_R2               0x02 // 6
#define HID_PS2_ADAPTER_BUTTON_START            0x20 // 6
#define HID_PS2_ADAPTER_BUTTON_SELECT           0x10 // 6

#define HID_PS2_ADAPTER_PAD_COUNT                1


//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! PS2
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_PS2_BUTTON_CROSS            0x20 // 5
#define HID_PS2_BUTTON_SQUARE           0x40 // 5
#define HID_PS2_BUTTON_CIRCLE           0x10 // 5
#define HID_PS2_BUTTON_TRIANGLE         0x80 // 5

#define HID_PS2_BUTTON_DPAD_N           0x00 // 5
#define HID_PS2_BUTTON_DPAD_NE          0x01 // 5
#define HID_PS2_BUTTON_DPAD_E           0x02 // 5
#define HID_PS2_BUTTON_DPAD_SE          0x03 // 5
#define HID_PS2_BUTTON_DPAD_S           0x04 // 5
#define HID_PS2_BUTTON_DPAD_SW          0x05 // 5
#define HID_PS2_BUTTON_DPAD_W           0x06 // 5
#define HID_PS2_BUTTON_DPAD_NW          0x07 // 5
#define HID_PS2_BUTTON_DPAD_NEUTRAL     0x0F // 5

#define HID_PS2_BUTTON_L1               0x04 // 6
#define HID_PS2_BUTTON_L2               0x01 // 6
#define HID_PS2_BUTTON_R1               0x08 // 6
#define HID_PS2_BUTTON_R2               0x02 // 6
#define HID_PS2_BUTTON_START            0x20 // 6
#define HID_PS2_BUTTON_SELECT           0x10 // 6

#define HID_PS2_PAD_COUNT                1

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Saitek P2600
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_SP2600_BUTTON_1              0x01 // 5
#define HID_SP2600_BUTTON_2              0x02 // 5
#define HID_SP2600_BUTTON_3              0x04 // 5
#define HID_SP2600_BUTTON_4              0x08 // 5

#define HID_SP2600_BUTTON_DPAD_N         0x00 // 5
#define HID_SP2600_BUTTON_DPAD_NE        0x10 // 5
#define HID_SP2600_BUTTON_DPAD_E         0x20 // 5
#define HID_SP2600_BUTTON_DPAD_SE        0x30 // 5
#define HID_SP2600_BUTTON_DPAD_S         0x40 // 5
#define HID_SP2600_BUTTON_DPAD_SW        0x50 // 5
#define HID_SP2600_BUTTON_DPAD_W         0x60 // 5
#define HID_SP2600_BUTTON_DPAD_NW        0x70 // 5
#define HID_SP2600_BUTTON_DPAD_NEUTRAL   0xF0 // 5

#define HID_SP2600_BUTTON_5              0x10 // 5   //L upper
#define HID_SP2600_BUTTON_6              0x20 // 5   //R upper
#define HID_SP2600_BUTTON_7              0x40 // 5   //L lower
#define HID_SP2600_BUTTON_8              0x80 // 5   //R lower
#define HID_SP2600_BUTTON_BLACK          0x01 // 6
#define HID_SP2600_BUTTON_WHITE          0x02 // 6
#define HID_SP2600_BUTTON_FPS            0x04 // 6
#define HID_SP2600_BUTTON_Analog         0x08 // 6

#define HID_SP2600_PAD_COUNT             1

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! POKKEN
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_POKKEN_BUTTON_1              0x01
#define HID_POKKEN_BUTTON_2              0x02
#define HID_POKKEN_BUTTON_3              0x04
#define HID_POKKEN_BUTTON_4              0x08
#define HID_POKKEN_BUTTON_5              0x10
#define HID_POKKEN_BUTTON_6              0x20
#define HID_POKKEN_BUTTON_7              0x40
#define HID_POKKEN_BUTTON_8              0x80
#define HID_POKKEN_BUTTON_9              0x80
#define HID_POKKEN_BUTTON_10             0x80

#define HID_POKKEN_BUTTON_DPAD_N         0x00
#define HID_POKKEN_BUTTON_DPAD_NE        0x10
#define HID_POKKEN_BUTTON_DPAD_E         0x20
#define HID_POKKEN_BUTTON_DPAD_SE        0x30
#define HID_POKKEN_BUTTON_DPAD_S         0x40
#define HID_POKKEN_BUTTON_DPAD_SW        0x50
#define HID_POKKEN_BUTTON_DPAD_W         0x60
#define HID_POKKEN_BUTTON_DPAD_NW        0x70
#define HID_POKKEN_BUTTON_DPAD_NEUTRAL   0xF0

#define HID_POKKEN_PAD_COUNT             1

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Keyboard
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_KEYBOARD_BUTTON_A                 0x04
#define HID_KEYBOARD_BUTTON_B                 0x05
#define HID_KEYBOARD_BUTTON_C                 0x06
#define HID_KEYBOARD_BUTTON_D                 0x07
#define HID_KEYBOARD_BUTTON_E                 0x08
#define HID_KEYBOARD_BUTTON_F                 0x09
#define HID_KEYBOARD_BUTTON_G                 0x0A
#define HID_KEYBOARD_BUTTON_H                 0x0B
#define HID_KEYBOARD_BUTTON_I                 0x0C
#define HID_KEYBOARD_BUTTON_J                 0x0D
#define HID_KEYBOARD_BUTTON_K                 0x0E
#define HID_KEYBOARD_BUTTON_L                 0x0F
#define HID_KEYBOARD_BUTTON_M                 0x10
#define HID_KEYBOARD_BUTTON_N                 0x11
#define HID_KEYBOARD_BUTTON_O                 0x12
#define HID_KEYBOARD_BUTTON_P                 0x13
#define HID_KEYBOARD_BUTTON_Q                 0x14
#define HID_KEYBOARD_BUTTON_R                 0x15
#define HID_KEYBOARD_BUTTON_S                 0x16
#define HID_KEYBOARD_BUTTON_T                 0x17
#define HID_KEYBOARD_BUTTON_U                 0x18
#define HID_KEYBOARD_BUTTON_V                 0x19
#define HID_KEYBOARD_BUTTON_W                 0x1A
#define HID_KEYBOARD_BUTTON_X                 0x1B
#define HID_KEYBOARD_BUTTON_Y                 0x1C
#define HID_KEYBOARD_BUTTON_Z                 0x1D
#define HID_KEYBOARD_BUTTON_F1                0x3A
#define HID_KEYBOARD_BUTTON_F2                0x3B
#define HID_KEYBOARD_BUTTON_F3                0x3C
#define HID_KEYBOARD_BUTTON_F4                0x3D
#define HID_KEYBOARD_BUTTON_F5                0x3E
#define HID_KEYBOARD_BUTTON_F6                0x3F
#define HID_KEYBOARD_BUTTON_F7                0x40
#define HID_KEYBOARD_BUTTON_F8                0x41
#define HID_KEYBOARD_BUTTON_F9                0x42
#define HID_KEYBOARD_BUTTON_F10               0x43
#define HID_KEYBOARD_BUTTON_F11               0x44
#define HID_KEYBOARD_BUTTON_F12               0x45
#define HID_KEYBOARD_BUTTON_LEFT              0x50
#define HID_KEYBOARD_BUTTON_RIGHT             0x4f
#define HID_KEYBOARD_BUTTON_DOWN              0x51
#define HID_KEYBOARD_BUTTON_UP                0x52
#define HID_KEYBOARD_BUTTON_ENTER             0x28
#define HID_KEYBOARD_BUTTON_MINUS             0x56
#define HID_KEYBOARD_BUTTON_PLUS              0x57
#define HID_KEYBOARD_BUTTON_SPACE             0x2C
#define HID_KEYBOARD_BUTTON_SHIFT             0x02
#define HID_KEYBOARD_BUTTON_TAB               0x2B

#define HID_KEYBOARD_PAD_COUNT             1
#define HID_KEYBOARD_DATA_LENGTH           8

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Mouse
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define HID_MOUSE_BUTTON_LEFTCLICK                 0x04
#define HID_MOUSE_BUTTON_RIGHTCLICK                0x05


#define HID_MOUSE_PAD_COUNT             1

#define HID_MOUSE_MODE_AIM                0x01
#define HID_MOUSE_MODE_TOUCH              0x02

struct _HID_Mouse_Data {
    u8 left_click;
    u8 right_click;
    s16 X;
    s16 Y;
    s16 deltaX;
    s16 deltaY;
} __attribute__ ((packed));

typedef struct _HID_Mouse_Data HID_Mouse_Data;

typedef struct _HID_Mouse_Data_c {
    HID_Mouse_Data data[2];
} HID_Mouse_Data_c;

typedef struct _HID_Mouse {
    my_cb_user * user_data;
    HID_Mouse_Data_c pad_data[HID_MOUSE_PAD_COUNT];
} HID_Mouse;

void my_ms_read_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user);
void my_ms_write_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user);
void my_ms_report_cb(unsigned int handle, int error, unsigned char *buf, unsigned int bytes_transfered, void *p_user);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Shared
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------


int my_attach_cb(HIDClient *p_client, HIDDevice *p_device, unsigned int attach);

int getPadCount(int hid);
HID_Data_Struct * getLastData(HID_Data_Struct data);
int getButtonPressed(HID_Data_Struct data, int VPADButton);
void setLastData(HID_Data_Struct data,HID_Data_Struct data_last);
int getActivePad(int hid);
void setRumble(int hid,int rumble);
void convertAnalogSticks(HID_Data_Struct data,VPADData * buffer);
void setTouch(HID_Data_Struct data,VPADData * buffer);

HID_Data_Struct * getHIDDataAll(int hid,int * size);

#ifdef __cplusplus
}
#endif

#endif /* _CONTROLLER_PATCHER_H_ */
