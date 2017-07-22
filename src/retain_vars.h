#ifndef _RETAIN_VARS_H_
#define _RETAIN_VARS_H_

#include "common/types.h"
#include "utils/voice_info.h"


extern int isSplatoon;
extern int drcMode;
extern int patched;
extern int AppInBackground;

// AX
extern VoiceInfo gVoiceInfos[VOICE_INFO_MAX];

// for Splatoon
extern uint32_t* gamemode;
extern uint32_t* inkstrikeEq;
extern uint32_t* spTimer;

extern uint32_t incrementVal;
extern uint32_t* incrementPtr;

#endif // _RETAIN_VARS_H_