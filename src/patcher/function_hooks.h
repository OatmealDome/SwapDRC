#ifndef _FUNCTION_HOOKS_H_
#define _FUNCTION_HOOKS_H_

#ifdef __cplusplus
extern "C" {
#endif
	
/* Forward declarations */
#define MASK_FD 0x0fff00ff

struct fs_async_t {
	void (*callback)(int status, int command, void *request, void *response, void *context);
	void *context;
	void *queue;
};

extern void swapVoices();
extern void drcSwap();

int swapForce;
unsigned int homeCoolDown;
void PatchMethodHooks(void);
void RestoreInstructions(void);
unsigned int GetAddressOfFunction(const char * functionName,unsigned int library);
int isDynamicFunction(unsigned int physicalAddress);

#ifdef __cplusplus
}
#endif

#endif /* _FS_H */
