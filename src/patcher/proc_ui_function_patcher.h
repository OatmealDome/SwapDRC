#ifndef _PROC_UI_FUNCTION_PATCHER_H
#define _PROC_UI_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/function_patcher.h"

extern hooks_magic_t method_hooks_proc_ui[];
extern u32 method_hooks_size_proc_ui;
extern volatile unsigned int method_calls_proc_ui[];

#ifdef __cplusplus
}
#endif

#endif /* _PROC_UI_FUNCTION_PATCHER_H */
