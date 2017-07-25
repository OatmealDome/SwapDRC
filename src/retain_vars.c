#include "common/types.h"
#include "retain_vars.h"
#include "patcher/cafiine_function_patcher.h"

int isSplatoon __attribute__((section(".data"))) = 0;
int drcMode __attribute__((section(".data"))) = 0;
uint32_t gAppStatus __attribute__((section(".data"))) = 0;
int gHomeCoolDown  __attribute__((section(".data"))) = 0;
int gLCDDelay __attribute__((section(".data"))) = 0;
s32 gLCDMode __attribute__((section(".data"))) = 0;

uint32_t cafiine_addr __attribute__((section(".data"))) = 0;
VoiceInfo gVoiceInfos[VOICE_INFO_MAX] __attribute__((section(".data")));
struct fs_patcher_utils fspatchervars __attribute__((section(".data")));

// for Splatoon
u8 gSwapForce  __attribute__((section(".data"))) = 0;
uint32_t* gamemode __attribute__((section(".data"))) = (uint32_t*)0;
uint32_t* inkstrikeEq __attribute__((section(".data"))) = (uint32_t*)0x10000000;
uint32_t* spTimer __attribute__((section(".data"))) = (uint32_t*)0x10000000;

uint32_t incrementVal __attribute__((section(".data"))) = (uint32_t)0;
uint32_t* incrementPtr __attribute__((section(".data"))) = (uint32_t*)0x10000000;
uint32_t* PCtrlPtr __attribute__((section(".data"))) = (uint32_t*)0x10000000;
