#ifndef _CAFIINE_FUNCTION_PATCHER_H
#define _CAFIINE_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/function_patcher.h"
#include "cafiine/cafiine.h"

struct fs_patcher_utils {
	int socket_fsa[MAX_CLIENT];
	void *pClient_fs[MAX_CLIENT];
	int socket_fs[MAX_CLIENT];
	volatile int lock;
};

extern hooks_magic_t method_hooks_cafiine[];
extern u32 method_hooks_size_cafiine;
extern volatile unsigned int method_calls_cafiine[];

#ifdef __cplusplus
}
#endif

#endif /* _CAFIINE_FUNCTION_PATCHER_H */
