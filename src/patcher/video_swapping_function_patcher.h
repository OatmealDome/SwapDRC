#ifndef _VIDEO_SWAPPING_FUNCTION_PATCHER_H
#define _VIDEO_SWAPPING_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/function_patcher.h"

extern hooks_magic_t method_hooks_video_swapping[];
extern u32 method_hooks_size_video_swapping;
extern volatile unsigned int method_calls_video_swapping[];

#ifdef __cplusplus
}
#endif

#endif /* _VIDEO_SWAPPING_FUNCTION_PATCHER_H */
