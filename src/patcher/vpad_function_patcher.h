#ifndef _VPAD_FUNCTION_PATCHER_H
#define _VPAD_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/function_patcher.h"

void drcSwap();

extern hooks_magic_t method_hooks_vpad[];
extern u32 method_hooks_size_vpad;
extern volatile unsigned int method_calls_vpad[];

#ifdef __cplusplus
}
#endif

#endif /* _VPAD_FUNCTION_PATCHER_H */
