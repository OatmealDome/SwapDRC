/****************************************************************************
 * Copyright (C) 2015
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include "os_functions.h"
#include "procui_functions.h"

unsigned int procui_handle __attribute__((section(".data"))) = 0;

EXPORT_DECL(void, ProcUIInit, ProcUISaveCallbackFunction function);
EXPORT_DECL(u32, ProcUIInForeground, void);
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
	OS_FIND_EXPORT(procui_handle, ProcUIInForeground);
    OS_FIND_EXPORT(procui_handle, ProcUIRegisterCallback);
}
