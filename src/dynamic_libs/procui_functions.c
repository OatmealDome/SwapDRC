#include "os_functions.h"
#include "procui_functions.h"

unsigned int procui_handle __attribute__((section(".data"))) = 0;

EXPORT_DECL(void, ProcUIInit, ProcUISaveCallbackFunction function);
EXPORT_DECL(void, ProcUIRegisterCallback, int message, ProcUICallbackFunction function, void* data, int priority);

void InitAcquireProcUI(void)
{
    OSDynLoad_Acquire("proc_ui.rpl", &procui_handle);
}

void InitProcUIFunctionPointers(void)
{
    unsigned int *funcPointer = 0;
    InitAcquireProcUI();
    
    OS_FIND_EXPORT(procui_handle, ProcUIInit);
    OS_FIND_EXPORT(procui_handle, ProcUIRegisterCallback);
}
