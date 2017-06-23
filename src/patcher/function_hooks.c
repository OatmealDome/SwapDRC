#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "common/common.h"
#include "common/fs_defs.h"
#include "common/loader_defs.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "kernel/kernel_functions.h"
#include "function_hooks.h"
#include "utils/logger.h"
#include "cafiine/cafiine.h"
#include "retain_vars.h"

#define LIB_CODE_RW_BASE_OFFSET                         0xC1000000
#define CODE_RW_BASE_OFFSET                             0x00000000
#define DEBUG_LOG_DYN                                   0

#define USE_EXTRA_LOG_FUNCTIONS   0

#define DECL(res, name, ...) \
        res (* real_ ## name)(__VA_ARGS__) __attribute__((section(".data"))); \
        res my_ ## name(__VA_ARGS__)

DECL(int, FSAInit, void) {
	if ((int)bss_ptr == 0x0a000000) {
		memset_bss();
	}
	return real_FSAInit();
}

DECL(int, FSAShutdown, void) {
	return real_FSAShutdown();
}

DECL(int, FSAAddClient, void *r3) {
	int res = real_FSAAddClient(r3);
	
	if ((int)bss_ptr != 0x0a000000 && res < MAX_CLIENT && res >= 0) {
		cafiine_connect((void*)0, res, 1);
	}
	
	return res;
}

DECL(int, FSADelClient, int client) {
	if ((int)bss_ptr != 0x0a000000 && client < MAX_CLIENT && client >= 0) {
		cafiine_disconnect((void*)0, client, 1);
	}
	
	return real_FSADelClient(client);
}

DECL(int, FSAOpenFile, int client, const char *path, const char *mode, int *handle) {
	if ((int)bss_ptr != 0x0a000000 && client < MAX_CLIENT && client >= 0) {
		int ret;
		if (cafiine_fopen((void*)0, client, &ret, path, mode, handle, 1) == 0)
			return ret;
	}
	
	return real_FSAOpenFile(client, path, mode, handle);
}

DECL(int, FSInit, void) {
	if ((int)bss_ptr == 0x0a000000) {
		memset_bss();
	}
	return real_FSInit();
}

DECL(int, FSShutdown, void) {
	return real_FSShutdown();
}

DECL(int, FSAddClientEx, void *r3, void *r4, void *r5) {
	int res = real_FSAddClientEx(r3, r4, r5);
	
	if ((int)bss_ptr != 0x0a000000 && res >= 0) {
		cafiine_connect(r3, 0, 0);
	}
	
	return res;
}

DECL(int, FSDelClient, void *pClient) {
	if ((int)bss_ptr != 0x0a000000) {
		cafiine_disconnect(pClient, 0, 0);
	}
	
	return real_FSDelClient(pClient);
}

DECL(int, FSOpenFile, void *pClient, void *pCmd, const char *path, const char *mode, int *handle, int error) {
	if ((int)bss_ptr != 0x0a000000) {
		int ret;
		if (cafiine_fopen(pClient, 0, &ret, path, mode, handle, 0) == 0) {
			return ret;
		}
	}
	
	return real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
}

DECL(int, FSReadFile, void *pClient, void *pCmd, void *buffer, int size, int count, int fd, int flag, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_fread(pClient, &ret, buffer, size, count, fd) == 0) {
			return ret;
		}
	}
	
	return real_FSReadFile(pClient, pCmd, buffer, size, count, fd, flag, error);
}

DECL(int, FSReadFileWithPos, void *pClient, void *pCmd, void *buffer, int size, int count, int pos, int fd, int flag, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_fsetpos(pClient, &ret, fd, pos) == 0) {
			if (cafiine_fread(pClient, &ret, buffer, size, count, fd) == 0) {
				return ret;
			}
		}
	}
	
	return real_FSReadFileWithPos(pClient, pCmd, buffer, size, count, pos, fd, flag, error);
}

DECL(int, FSCloseFile, void *pClient, void *pCmd, int fd, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_fclose(pClient, &ret, fd) == 0) {
			return ret;
		}
	}
	
	return real_FSCloseFile(pClient, pCmd, fd, error);
}

DECL(int, FSSetPosFile, void *pClient, void *pCmd, int fd, int pos, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_fsetpos(pClient, &ret, fd, pos) == 0) {
			return ret;
		}

	}
	
	return real_FSSetPosFile(pClient, pCmd, fd, pos, error);
}

DECL(int, FSGetPosFile, void *pClient, void *pCmd, int fd, int *pos, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_fgetpos(pClient, &ret, fd, pos) == 0) {
			return ret;
		}
	}
	
	return real_FSGetPosFile(pClient, pCmd, fd, pos, error);
}

DECL(int, FSGetStatFile, void *pClient, void *pCmd, int fd, void *buffer, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_fstat(pClient, &ret, fd, buffer) == 0) {
			return ret;
		}
	}
	
	return real_FSGetStatFile(pClient, pCmd, fd, buffer, error);
}

DECL(int, FSIsEof, void *pClient, void *pCmd, int fd, int error) {
	if ((int)bss_ptr != 0x0a000000 && ((fd & MASK_FD) == MASK_FD)) {
		int ret;
		if (cafiine_feof(pClient, &ret, fd) == 0) {
			return ret;
		}
	}
	
	return real_FSIsEof(pClient, pCmd, fd, error);
}

DECL(void, GX2CopyColorBufferToScanBuffer, GX2ColorBuffer *colorBuffer, s32 scan_target)
{
    // GX2 destinations:
    // 0x1 = TV
    // 0x4 = 1st GamePad

    // drcMode values:
    // 0x0 = normal
    // 0x1 = swap

	// the 1st if statement can also FORCE the drcMode to default when inkstrike is activated

	if (drcMode == 0 || (isSplatoon && *inkstrikeEq == 2 && *spTimer != 0))
	{
		real_GX2CopyColorBufferToScanBuffer(colorBuffer, scan_target);
	}
    else
    {
        if (scan_target == 0x1)
        {
            real_GX2CopyColorBufferToScanBuffer(colorBuffer, 0x4);
        }
        else if (scan_target == 0x4)
        {
            real_GX2CopyColorBufferToScanBuffer(colorBuffer, 0x1);
        }
    }
}

DECL(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error)
{
    int ret = real_VPADRead(chan, buffer, buffer_size, error);

    // check if we should use Splatoon mode controls
    if (isSplatoon)
    {
        if (gamemode == (uint32_t*)0)
        {
            uint32_t firstBase = *(uint32_t*)0x106A3BA0;
            if (firstBase > 0x10000000 && firstBase < 0x11000000)
            {
                uint32_t secondBase = *(uint32_t*)(firstBase + 0xD074);
                if (secondBase > 0x12000000 && secondBase < 0x14000000)
                {
                    gamemode = (uint32_t*)(secondBase + 0x238);
                }
            }
        }
        else if (*gamemode != 0xFFFFFFFF)
        {
            // switch if B is pressed
            if (buffer->btns_d & VPAD_BUTTON_B)
            {
                drcMode = !drcMode;
            }
        }

		// Checks special equipped pointer
		if (*(inkstrikeEq - 0xC) != 0x3F800000)
		{
			uint32_t firstBase = *(uint32_t*)0x106E46E8;
			if (firstBase > 0x1D000000 && firstBase < 0x27000000)
			{
				inkstrikeEq = (uint32_t*)(firstBase + 0x80);
				spTimer = (uint32_t*)(firstBase + 0x80C);
			}
		}
    }

	// switch on L and SELECT
	if (buffer->btns_d & VPAD_BUTTON_MINUS && buffer->btns_h & VPAD_BUTTON_L)
	{
		drcMode = !drcMode;
	}
	
    // disable touchscreen input if the TV is on the DRC
	if (drcMode == 0 || (isSplatoon && *inkstrikeEq == 2 && *spTimer != 0)) {}
	else
    {
        buffer->tpdata.touched = 0;
        buffer->tpdata1.touched = 0;
        buffer->tpdata2.touched = 0;
    }
	
    return ret;
}

DECL(void, VPADGetTPCalibratedPoint, int chan, VPADTPData *screen, VPADTPData *raw)
{
    real_VPADGetTPCalibratedPoint(chan, screen, raw);
	
	// disable touchscreen input if the TV is on the DRC
	if (drcMode == 0 || (isSplatoon && *inkstrikeEq == 2 && *spTimer != 0)) {}
	else
	{
		screen->touched = 0;
		raw->touched = 0;
	}
}

DECL(void, VPADGetTPCalibratedPointEx, int chan, int resolution, VPADTPData *screen, VPADTPData *raw)
{
    real_VPADGetTPCalibratedPointEx(chan, resolution, screen, raw);

	// disable touchscreen input if the TV is on the DRC
	if (drcMode == 0 || (isSplatoon && *inkstrikeEq == 2 && *spTimer != 0)) {}
	else
	{
		screen->touched = 0;
		raw->touched = 0;
	}

    /*
    // check for Splatoon
    if (isSplatoon)
    {
        if (screenPressTimeout != 0)
        {
            screenPressTimeout--;
        }

        // disable touchscreen input if the TV is on the DRC
        if (drcMode == 1 && screenPressTimeout == 0)
        {
            screen->touched = 0;
            raw->touched = 0;
        }
        else
        {
            // Swap once a touchscreen input is received during a match
            if (screen->touched == 1 && gamemode != 0 && *gamemode != 0xFFFFFFFF)
            {
                drcMode = !drcMode;
                screenPressTimeout = 300;
            }
        }
    }*/
}

/* *****************************************************************************
 * Creates function pointer array
 * ****************************************************************************/
#define MAKE_MAGIC(x, lib,functionType) { (unsigned int) my_ ## x, (unsigned int) &real_ ## x, lib, # x,0,0,functionType,0}

static struct hooks_magic_t {
	const unsigned int replaceAddr;
	const unsigned int replaceCall;
	const unsigned int library;
	const char functionName[50];
	unsigned int realAddr;
	unsigned int restoreInstruction;
	unsigned char functionType;
	unsigned char alreadyPatched;
} method_hooks[] = {
	MAKE_MAGIC(FSAInit, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSAShutdown, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSAAddClient, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSADelClient, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSAOpenFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSInit, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSShutdown, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSAddClientEx, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSDelClient, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSOpenFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSCloseFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSReadFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSReadFileWithPos, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSSetPosFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSGetStatFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSIsEof, LIB_FS, STATIC_FUNCTION),
    MAKE_MAGIC(GX2CopyColorBufferToScanBuffer, LIB_GX2, STATIC_FUNCTION),
    MAKE_MAGIC(VPADRead, LIB_VPAD, STATIC_FUNCTION),
    MAKE_MAGIC(VPADGetTPCalibratedPoint, LIB_VPAD, STATIC_FUNCTION)//,
    //MAKE_MAGIC(VPADGetTPCalibratedPointEx, LIB_VPAD, STATIC_FUNCTION)
};


//! buffer to store our 7 instructions needed for our replacements
//! the code will be placed in the address of that buffer - CODE_RW_BASE_OFFSET
//! avoid this buffer to be placed in BSS and reset on start up
volatile unsigned int dynamic_method_calls[sizeof(method_hooks) / sizeof(struct hooks_magic_t) * 7] __attribute__((section(".data")));

/*
*Patches a function that is loaded at the start of each application. Its not required to restore, at least when they are really dynamic.
* "normal" functions should be patch with the normal patcher.
*/
void PatchMethodHooks(void)
{
    /* Patch branches to it.  */
    volatile unsigned int *space = &dynamic_method_calls[0];

    int method_hooks_count = sizeof(method_hooks) / sizeof(struct hooks_magic_t);

    u32 skip_instr = 1;
    u32 my_instr_len = 6;
    u32 instr_len = my_instr_len + skip_instr;
    u32 flush_len = 4*instr_len;
    for(int i = 0; i < method_hooks_count; i++)
    {
        if(method_hooks[i].functionType == STATIC_FUNCTION && method_hooks[i].alreadyPatched == 1){
            if(isDynamicFunction((u32)OSEffectiveToPhysical((void*)method_hooks[i].realAddr))){
                log_printf("The function %s is a dynamic function. Please fix that <3\n", method_hooks[i].functionName);
                method_hooks[i].functionType = DYNAMIC_FUNCTION;
            }else{
                log_printf("Skipping %s, its already patched\n", method_hooks[i].functionName);
                space += instr_len;
                continue;
            }
        }

        u32 physical = 0;
        unsigned int repl_addr = (unsigned int)method_hooks[i].replaceAddr;
        unsigned int call_addr = (unsigned int)method_hooks[i].replaceCall;

        unsigned int real_addr = GetAddressOfFunction(method_hooks[i].functionName,method_hooks[i].library);

        if(!real_addr){
            log_printf("OSDynLoad_FindExport failed for %s\n", method_hooks[i].functionName);
            space += instr_len;
            continue;
        }

        if(DEBUG_LOG_DYN)log_printf("%s is located at %08X!\n", method_hooks[i].functionName,real_addr);

        physical = (u32)OSEffectiveToPhysical((void*)real_addr);
        if(!physical){
             log_printf("Something is wrong with the physical address\n");
             space += instr_len;
             continue;
        }

        if(DEBUG_LOG_DYN)log_printf("%s physical is located at %08X!\n", method_hooks[i].functionName,physical);

        bat_table_t my_dbat_table;
        if(DEBUG_LOG_DYN)log_printf("Setting up DBAT\n");
        KernelSetDBATsForDynamicFuction(&my_dbat_table,physical);

        //log_printf("Setting call_addr to %08X\n",(unsigned int)(space) - CODE_RW_BASE_OFFSET);
        *(volatile unsigned int *)(call_addr) = (unsigned int)(space) - CODE_RW_BASE_OFFSET;

        // copy instructions from real function.
        u32 offset_ptr = 0;
        for(offset_ptr = 0;offset_ptr<skip_instr*4;offset_ptr +=4){
             if(DEBUG_LOG_DYN)log_printf("(real_)%08X = %08X\n",space,*(volatile unsigned int*)(physical+offset_ptr));
            *space = *(volatile unsigned int*)(physical+offset_ptr);
            space++;
        }

        //Only works if skip_instr == 1
        if(skip_instr == 1){
            // fill the restore instruction section
            method_hooks[i].realAddr = real_addr;
            method_hooks[i].restoreInstruction = *(volatile unsigned int*)(physical);
        }else{
            log_printf("Can't save %s for restoring!\n", method_hooks[i].functionName);
        }

        //adding jump to real function
        /*
            90 61 ff e0     stw     r3,-32(r1)
            3c 60 12 34     lis     r3,4660
            60 63 56 78     ori     r3,r3,22136
            7c 69 03 a6     mtctr   r3
            80 61 ff e0     lwz     r3,-32(r1)
            4e 80 04 20     bctr*/
        *space = 0x9061FFE0;
        space++;
        *space = 0x3C600000 | (((real_addr + (skip_instr * 4)) >> 16) & 0x0000FFFF); // lis r3, real_addr@h
        space++;
        *space = 0x60630000 |  ((real_addr + (skip_instr * 4)) & 0x0000ffff); // ori r3, r3, real_addr@l
        space++;
        *space = 0x7C6903A6; // mtctr   r3
        space++;
        *space = 0x8061FFE0; // lwz     r3,-32(r1)
        space++;
        *space = 0x4E800420; // bctr
        space++;
        DCFlushRange((void*)(space - instr_len), flush_len);
        ICInvalidateRange((unsigned char*)(space - instr_len), flush_len);

        //setting jump back
        unsigned int replace_instr = 0x48000002 | (repl_addr & 0x03fffffc);
        *(volatile unsigned int *)(physical) = replace_instr;
        ICInvalidateRange((void*)(real_addr), 4);

        //restore my dbat stuff
        KernelRestoreDBATs(&my_dbat_table);

        method_hooks[i].alreadyPatched = 1;
    }
    log_print("Done with patching all functions!\n");
}

/* ****************************************************************** */
/*                  RESTORE ORIGINAL INSTRUCTIONS                     */
/* ****************************************************************** */
void RestoreInstructions(void)
{
    bat_table_t table;
    log_printf("Restore functions!\n");
    int method_hooks_count = sizeof(method_hooks) / sizeof(struct hooks_magic_t);
    for(int i = 0; i < method_hooks_count; i++)
    {
        if(method_hooks[i].restoreInstruction == 0 || method_hooks[i].realAddr == 0){
            log_printf("I dont have the information for the restore =( skip\n");
            continue;
        }

        unsigned int real_addr = GetAddressOfFunction(method_hooks[i].functionName,method_hooks[i].library);

        if(!real_addr){
            log_printf("OSDynLoad_FindExport failed for %s\n", method_hooks[i].functionName);
            continue;
        }

        u32 physical = (u32)OSEffectiveToPhysical((void*)real_addr);
        if(!physical){
            log_printf("Something is wrong with the physical address\n");
            continue;
        }

        if(isDynamicFunction(physical)){
             log_printf("Its a dynamic function. We don't need to restore it! %s\n",method_hooks[i].functionName);
        }else{
            KernelSetDBATs(&table);

            *(volatile unsigned int *)(LIB_CODE_RW_BASE_OFFSET + method_hooks[i].realAddr) = method_hooks[i].restoreInstruction;
            DCFlushRange((void*)(LIB_CODE_RW_BASE_OFFSET + method_hooks[i].realAddr), 4);
            ICInvalidateRange((void*)method_hooks[i].realAddr, 4);
            log_printf("Restored %s\n",method_hooks[i].functionName);
            KernelRestoreDBATs(&table);
        }
        method_hooks[i].alreadyPatched = 0; // In case a
    }
    KernelRestoreInstructions();
    log_print("Done with restoring all functions!\n");
}

int isDynamicFunction(unsigned int physicalAddress){
    if((physicalAddress & 0x80000000) == 0x80000000){
        return 1;
    }
    return 0;
}

unsigned int GetAddressOfFunction(const char * functionName,unsigned int library){
    unsigned int real_addr = 0;

    unsigned int rpl_handle = 0;
    if(library == LIB_FS){
        log_printf("FindExport of %s! From LIB_FS\n", functionName);
        if(coreinit_handle == 0){log_print("LIB_FS not aquired\n"); return 0;}
        rpl_handle = coreinit_handle;
    }else if (library == LIB_GX2){
        log_printf("FindExport of %s! From LIB_GX2\n", functionName);
        if(gx2_handle == 0){log_print("LIB_GX2 not aquired\n"); return 0;}
        rpl_handle = gx2_handle;
    }else if (library == LIB_VPAD){
        log_printf("FindExport of %s! From LIB_VPAD\n", functionName);
        if(vpad_handle == 0){log_print("LIB_VPAD not aquired\n"); return 0;}
        rpl_handle = vpad_handle;
    }

    if(!rpl_handle){
        log_printf("Failed to find the RPL handle for %s\n", functionName);
        return 0;
    }

    OSDynLoad_FindExport(rpl_handle, 0, functionName, &real_addr);

    if(!real_addr){
        log_printf("OSDynLoad_FindExport failed for %s\n", functionName);
        return 0;
    }

    if((u32)(*(volatile unsigned int*)(real_addr) & 0xFF000000) == 0x48000000){
        real_addr += (u32)(*(volatile unsigned int*)(real_addr) & 0x0000FFFF);
        if((u32)(*(volatile unsigned int*)(real_addr) & 0xFF000000) == 0x48000000){
            return 0;
        }
    }

    return real_addr;
}
