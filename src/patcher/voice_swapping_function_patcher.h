#ifndef _VOICE_SWAPPING_FUNCTION_PATCHER_H
#define _VOICE_SWAPPING_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/function_patcher.h"

void swapVoices();

extern hooks_magic_t method_hooks_voice_swapping[];
extern u32 method_hooks_size_voice_swapping;
extern volatile unsigned int method_calls_voice_swapping[];

#ifdef __cplusplus
}
#endif

#endif /* _VOICE_SWAPPING_FUNCTION_PATCHER_H */
