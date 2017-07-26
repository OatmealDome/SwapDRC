#ifndef _RETAIN_VARS_H_
#define _RETAIN_VARS_H_

//for pre-compiling the app
#define LITE     0    // 0 = default, 1 = lite version (no tcpgecko and cafiine)


#include "common/types.h"
#include "utils/voice_info.hpp"
#include "patcher/cafiine_function_patcher.h"

extern struct fs_patcher_utils fspatchervars __attribute__((section(".data")));

extern int isSplatoon;
extern int drcMode;
extern uint32_t gAppStatus;
extern int gHomeCoolDown;
extern int gLCDDelay;
extern s32 gLCDMode;

extern uint32_t cafiine_addr;

// AX
extern VoiceInfo gVoiceInfos[VOICE_INFO_MAX];

// for Splatoon
extern u8 gSwapForce;

extern uint32_t* gamemode;
extern uint32_t* inkstrikeEq;
extern uint32_t* spTimer;

extern uint32_t incrementVal;
extern uint32_t* incrementPtr;
extern uint32_t* PCtrlPtr;

#endif // _RETAIN_VARS_H_
