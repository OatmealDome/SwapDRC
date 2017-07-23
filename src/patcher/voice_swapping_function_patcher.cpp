#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "voice_swapping_function_patcher.h"
#include "utils/voice_swapper.hpp"
#include "utils/logger.h"
#include "retain_vars.h"

DECL(s32, AXSetVoiceDeviceMixOld, void *v, s32 device, u32 id, void *mix){
   if (drcMode) device = (device == 1) ? 0 : 1;
    if(VOICE_SWAP_LOG == 1){log_printf("AXSetVoiceDeviceMixOld voice: %08X device: %d, mix: %08X\n",v,device,mix);}
    VoiceSwapper::setMix(v,device,mix);
    return real_AXSetVoiceDeviceMixOld(v,device,id,mix);
}

DECL(s32, AXSetVoiceDeviceMix, void *v, s32 device, u32 id, void *mix){
    if (drcMode) device = (device == 1) ? 0 : 1;
    if(VOICE_SWAP_LOG == 1){log_printf("AXSetVoiceDeviceMix voice: %08X device: %d, mix: %08X\n",v,device,mix);}
    VoiceSwapper::setMix(v,device,mix);
    return real_AXSetVoiceDeviceMix(v,device,id,mix);
}

DECL(void *, AXAcquireVoiceExOld, u32 prio, void * callback, u32 arg){
    void * result = real_AXAcquireVoiceExOld(prio,callback,arg);
    if(VOICE_SWAP_LOG == 1){log_printf("AXAcquireVoiceExOld result: %08X \n",result);}
    VoiceSwapper::acquireVoice(result);
    return result;
}

DECL(void *, AXAcquireVoiceEx, u32 prio, void * callback, u32 arg){
    void * result = real_AXAcquireVoiceEx(prio,callback,arg);
    if(VOICE_SWAP_LOG == 1){log_printf("AXAcquireVoiceEx result: %08X \n",result);}
    VoiceSwapper::acquireVoice(result);
    return result;
}

DECL(void, AXFreeVoiceOld, void *v){
    if(VOICE_SWAP_LOG == 1){log_printf("AXFreeVoiceOld v: %08X \n",v);}
    VoiceSwapper::freeVoice(v);
    real_AXFreeVoiceOld(v);
}

DECL(void, AXFreeVoice, void *v){
    if(VOICE_SWAP_LOG == 1){log_printf("AXFreeVoice v: %08X \n",v);}
    VoiceSwapper::freeVoice(v);
    real_AXFreeVoice(v);
}

/**
    We do this here because we need the real_ addresses. Not clean code, but the easiest way to do it :>
**/
void swapVoices(){
    VoiceSwapper::swapAll();
    for(int i = 0;i<VOICE_INFO_MAX;i++){
        if(gVoiceInfos[i].voice == NULL) continue;

        real_AXSetVoiceDeviceMix(gVoiceInfos[i].voice,0,0,gVoiceInfos[i].mixTV);
        real_AXSetVoiceDeviceMix(gVoiceInfos[i].voice,1,0,gVoiceInfos[i].mixDRC);
        real_AXSetVoiceDeviceMixOld(gVoiceInfos[i].voice,0,0,gVoiceInfos[i].mixTV);
        real_AXSetVoiceDeviceMixOld(gVoiceInfos[i].voice,1,0,gVoiceInfos[i].mixDRC);
    }
}

hooks_magic_t method_hooks_voice_swapping[] __attribute__((section(".data"))) = {
    MAKE_MAGIC_NAME(AXAcquireVoiceExOld,    AXAcquireVoiceEx,       LIB_AX_OLD,     STATIC_FUNCTION),
    MAKE_MAGIC_NAME(AXFreeVoiceOld,         AXFreeVoice,            LIB_AX_OLD,     STATIC_FUNCTION),
    MAKE_MAGIC_NAME(AXSetVoiceDeviceMixOld, AXSetVoiceDeviceMix,    LIB_AX_OLD,     STATIC_FUNCTION),
    MAKE_MAGIC_NAME(AXAcquireVoiceEx,       AXAcquireVoiceEx,       LIB_AX,         DYNAMIC_FUNCTION),
    MAKE_MAGIC_NAME(AXFreeVoice,            AXFreeVoice,            LIB_AX,         DYNAMIC_FUNCTION),
    MAKE_MAGIC_NAME(AXSetVoiceDeviceMix,    AXSetVoiceDeviceMix,    LIB_AX,         DYNAMIC_FUNCTION),
};

u32 method_hooks_size_voice_swapping __attribute__((section(".data"))) = sizeof(method_hooks_voice_swapping) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_voice_swapping[sizeof(method_hooks_voice_swapping) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

