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
#ifndef __PROCUI_FUNCTIONS_H_
#define __PROCUI_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <gctypes.h>

typedef enum ProcUICallbackType
{
	PROCUI_CALLBACK_ACQUIRE,
	PROCUI_CALLBACK_RELEASE,
	PROCUI_CALLBACK_EXIT,
	PROCUI_CALLBACK_NET_IO_START,
	PROCUI_CALLBACK_NET_IO_STOP,
	PROCUI_CALLBACK_HOME_BUTTON_DENIED,
} ProcUICallbackType;

typedef enum ProcUIStatus
{
	PROCUI_STATUS_IN_FOREGROUND,
	PROCUI_STATUS_IN_BACKGROUND,
	PROCUI_STATUS_RELEASE_FOREGROUND,
	PROCUI_STATUS_EXITING,
} ProcUIStatus;

extern unsigned int procui_handle;

void InitProcUIFunctionPointers(void);
void InitAcquireProcUI(void);

typedef void (*ProcUISaveCallbackFunction)(void);
typedef int (*ProcUICallbackFunction)(void*);

extern void (*ProcUIInit)(ProcUISaveCallbackFunction function);
extern u32(*ProcUIInForeground)(void);
extern void (*ProcUIRegisterCallback)(int message, ProcUICallbackFunction function, void* data, int priority);

#ifdef __cplusplus
}
#endif

#endif // __PROCUI_FUNCTIONS_H_
