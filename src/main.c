#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "utils/logger.h"
#include "common/common.h"
#include "patcher/function_hooks.h"
#include "kernel/kernel_functions.h"
#include "gecko/pygecko.h"
#include "cafiine/cafiine.h"

#define PRINT_TEXT2(x, y, ...) { snprintf(msg, 80, __VA_ARGS__); OSScreenPutFontEx(0, x, y, msg); OSScreenPutFontEx(1, x, y, msg); }

/* Entry point */
int Menu_Main(void)
{
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitOSFunctionPointers();
    InitSocketFunctionPointers();
    InitGX2FunctionPointers();
    InitSysFunctionPointers();
	InitFSFunctionPointers();

    log_init("192.168.2.15");

	//memoryInitAreaTable();
    SetupKernelCallback();
	//memoryInitialize();
	
    log_printf("Started %s\n", cosAppXmlInfoStruct.rpx_name);
	
	//!*******************************************************************
	//!                    Initialize BSS sections                       *
	//!*******************************************************************
	asm volatile(
				 "lis 3, __bss_start@h\n"
				 "ori 3, 3,__bss_start@l\n"
				 "lis 5, __bss_end@h\n"
				 "ori 5, 5, __bss_end@l\n"
				 "subf 5, 3, 5\n"
				 "li 4, 0\n"
				 "bl memset\n"
				 );
	
	log_printf("starting tcpgecko\n");
	start_pygecko();
	
	if(strcasecmp("men.rpx", cosAppXmlInfoStruct.rpx_name) == 0)
	{
		return EXIT_RELAUNCH_ON_LOAD;
	}
	else if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0)
	{
		return EXIT_RELAUNCH_ON_LOAD;
	}
	
	PatchMethodHooks();
	
	// UI stuff begins here
	
	/*VPADInit();
	
	// Prepare screen
	int screen_buf0_size = 0;
	int screen_buf1_size = 0;
	
	// Init screen and screen buffers
	OSScreenInit();
	screen_buf0_size = OSScreenGetBufferSizeEx(0);
	screen_buf1_size = OSScreenGetBufferSizeEx(1);
	
	unsigned char *screenBuffer = MEM1_alloc(screen_buf0_size + screen_buf1_size, 0x40);
	
	OSScreenSetBufferEx(0, screenBuffer);
	OSScreenSetBufferEx(1, (screenBuffer + screen_buf0_size));
	
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	
	char msg[80];
	int update_screen = 1;
	int ip[4] = { 192, 168, 2, 15 };
	uint8_t sel_ip = 3;
	int vpadError = -1;
	VPADData vpadData;
	int delay = 0;
	
	while (1)
	{
		// Read vpad
		VPADRead(0, &vpadData, 1, &vpadError);
		
		if(update_screen)
		{
			OSScreenClearBufferEx(0, 0);
			OSScreenClearBufferEx(1, 0);
			
			// Print message
			PRINT_TEXT2(14, 1, "-- cafiine + tcpgecko --");
			PRINT_TEXT2(0, 4, "Using the D-Pad, enter in the IP of the computer running a");
			PRINT_TEXT2(0, 5, "cafiine server.");
			
			// Print ip digit selector
			uint8_t x_shift = 17 + 4 * sel_ip;
			PRINT_TEXT2(x_shift, 7, "vvv");
			
			PRINT_TEXT2(0, 8, "      Server IP: %03i.%03i.%03i.%03i", ip[0], ip[1], ip[2], ip[3]);
			
			PRINT_TEXT2(0, 11, "Press A to install cafiine and TCPGecko.");
			PRINT_TEXT2(0, 12, "You will be returned to the Wii U menu.");
			
			
			OSScreenFlipBuffersEx(0);
			OSScreenFlipBuffersEx(1);
		}
		
		u32 pressedBtns = vpadData.btns_d | vpadData.btns_h;
		
		// Check for buttons
		// A Button
		if (pressedBtns & VPAD_BUTTON_A) {
			break;
		}
		
		// Left/Right Buttons
		if (vpadData.btns_d & VPAD_BUTTON_LEFT) {
			if(sel_ip == 0)
				sel_ip = 3;
			else
				--sel_ip;
		} else if (vpadData.btns_d & VPAD_BUTTON_RIGHT) {
			sel_ip = ((sel_ip + 1) % 4);
		} else if (pressedBtns & VPAD_BUTTON_UP) {
			if(--delay <= 0) {
				ip[sel_ip]++;
				delay = (vpadData.btns_d & VPAD_BUTTON_UP) ? 6 : 0;
			}
		} else if (pressedBtns & VPAD_BUTTON_DOWN) {
			if(--delay <= 0) {
				ip[sel_ip]--;
				delay = (vpadData.btns_d & VPAD_BUTTON_DOWN) ? 6 : 0;
			}
		} else {
			delay = 0;
		}
		
		// Button pressed ?
		update_screen = (pressedBtns & (VPAD_BUTTON_LEFT | VPAD_BUTTON_RIGHT | VPAD_BUTTON_UP | VPAD_BUTTON_DOWN)) ? 1 : 0;
		usleep(20000);
	}
	
	log_printf("freeing screenBuffer\n");
	
	MEM1_free(screenBuffer);
	screenBuffer = NULL;
	
	log_printf("releasing memory\n");
	memoryRelease();*/
	
	//log_printf("setting cafiine ip\n");
	
	log_printf("starting system menu\n");
	SYSLaunchMenu();

    log_deinit();

    return EXIT_RELAUNCH_ON_LOAD;
}

