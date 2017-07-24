#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "proc_ui_function_patcher.h"
#include "utils/logger.h"
#include "retain_vars.h"

//PROCUI FUNCTIONS
DECL(u32, ProcUIProcessMessages, u32 u) {
	u32 res = real_ProcUIProcessMessages(u);
	if(res != gAppStatus){
        log_printf("App status changed from %d to %d \n",gAppStatus,res);
        gAppStatus = res;
    }
	return res;
}

hooks_magic_t method_hooks_proc_ui[] __attribute__((section(".data"))) = {
	MAKE_MAGIC(ProcUIProcessMessages, LIB_PROC_UI, DYNAMIC_FUNCTION),
};
u32 method_hooks_size_proc_ui __attribute__((section(".data"))) = sizeof(method_hooks_proc_ui) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_proc_ui[sizeof(method_hooks_proc_ui) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));
