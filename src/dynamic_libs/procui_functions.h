#ifndef __PROCUI_FUNCTIONS_H_
#define __PROCUI_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

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
extern void (*ProcUIRegisterCallback)(int message, ProcUICallbackFunction function, void* data, int priority);

#ifdef __cplusplus
}
#endif

#endif // __PROCUI_FUNCTIONS_H_
