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
#include "dynamic_libs/vpad_functions.h"
#include "utils/logger.h"
#include "common/common.h"
#include "patcher/function_hooks.h"
#include "kernel/kernel_functions.h"
#include "gecko/pygecko.h"
#include "cafiine/cafiine.h"

#define PRINT_TEXT1(x, y, str) { OSScreenPutFontEx(1, x, y, str); }
#define PRINT_TEXT2(x, y, _fmt, ...) { __os_snprintf(msg, 80, _fmt, __VA_ARGS__); OSScreenPutFontEx(1, x, y, msg); }
#define BTN_PRESSED (BUTTON_LEFT | BUTTON_RIGHT | BUTTON_UP | BUTTON_DOWN | BUTTON_A | BUTTON_B)

/* IP union */
typedef union u_serv_ip
{
	uint8_t  digit[4];
	uint32_t full;
} u_serv_ip;

static unsigned int patched = 0;
static uint32_t persistent_ip = 0xC0A80140;

/* Entry point */
int Menu_Main()
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
	InitVPadFunctionPointers();
	
	log_init("192.168.2.15");
	
	SetupKernelCallback();
	
	log_printf("Geckiine started (rpx =  %s)\n", cosAppXmlInfoStruct.rpx_name);
	
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
	
	if (patched)
	{
		log_printf("Starting the TCPGecko server.\n");
		start_pygecko();
	}
	
	if (patched == 0)
	{
		// Prepare screen
		int screen_buf0_size = 0;
		int screen_buf1_size = 0;
		char msg[80];
		
		// Init screen and screen buffers
		OSScreenInit();
		screen_buf0_size = OSScreenGetBufferSizeEx(0);
		screen_buf1_size = OSScreenGetBufferSizeEx(1);
		OSScreenSetBufferEx(0, (void *)0xF4000000);
		OSScreenSetBufferEx(1, (void *)0xF4000000 + screen_buf0_size);
		
		OSScreenEnableEx(0, 1);
		OSScreenEnableEx(1, 1);
		
		void clearScreen();
		
		// Set server ip with buttons
		int error;
		u_serv_ip ip;
		ip.full = persistent_ip;
		uint8_t sel_ip = 3;
		uint8_t button_pressed = 1;
		uint8_t first_pass = 1;
		int use_fs_logs = 0;
		VPADData vpad_data;
		VPADRead(0, &vpad_data, 1, &error); //Read initial vpad status
		while (1)
		{
			// Refresh screen if needed
			if (button_pressed) { OSScreenFlipBuffersEx(1); OSScreenClearBufferEx(1, 0); }
			
			// Read vpad
			VPADRead(0, &vpad_data, 1, &error);
			
			// Title
			PRINT_TEXT1(23, 1, "-- geckiine --");
			PRINT_TEXT1(23, 2, "by OatmealDome");
			
			// Displays the current page
			PRINT_TEXT2(3, 5,  "1. IP : %3d.%3d.%3d.%3d  (Use D-PAD to change IP selection)", ip.digit[0], ip.digit[1], ip.digit[2], ip.digit[3]);
			PRINT_TEXT1(3, 6,  "2. Press A with Cafiine server running on above IP");
			PRINT_TEXT1(3, 7,  "3. To take out of memory, reload HBL in browser");
			PRINT_TEXT1(3, 8,  "4. After reloading HBL, geckiine will be gone");
			PRINT_TEXT1(3, 9,  "5. If loading HBL fails, just reload kernel");
			
			PRINT_TEXT1(11 + 4 * sel_ip, 4, "vvv");
			
			// Check buttons
			if (!button_pressed)
			{
				// A Button
				if (vpad_data.btn_hold & BUTTON_A)
				{
					// Set wait message
					OSScreenClearBufferEx(1, 0);
					PRINT_TEXT1(27, 8, "Wait ...");
					OSScreenFlipBuffersEx(1);
					break;
				}
				
				// Left/Right Buttons
				if (vpad_data.btn_hold & BUTTON_LEFT ) sel_ip = !sel_ip ? sel_ip = 3 : --sel_ip;
				if (vpad_data.btn_hold & BUTTON_RIGHT) sel_ip = ++sel_ip % 4;
				
				// Up/Down Buttons
				if (vpad_data.btn_hold & BUTTON_UP  ) ip.digit[sel_ip] = ++ip.digit[sel_ip];
				if (vpad_data.btn_hold & BUTTON_DOWN) ip.digit[sel_ip] = --ip.digit[sel_ip];
			}
			
			// Print coffee and exit msg
			PRINT_TEXT1(0, 12, "    )))");
			PRINT_TEXT1(0, 13, "    (((");
			PRINT_TEXT1(0, 14, "  +-----+");
			PRINT_TEXT1(0, 15, "  | XHP |]");
			PRINT_TEXT1(0, 16, "  `-----\'");
			PRINT_TEXT1(25, 16, "geckiine IP selection added by brienj");
			
			// Update screen
			if (first_pass) { OSScreenFlipBuffersEx(1); OSScreenClearBufferEx(1, 0); first_pass = 0; }
			
			// Button pressed ?
			button_pressed = (vpad_data.btn_hold & BTN_PRESSED) ? 1 : 0;
		}
		
		persistent_ip = ip.full;
		new_addr = ip.full;
		
		log_printf("Starting the TCPGecko server.\n");
		start_pygecko();
		patched = 1;
	}
	
	if(strcasecmp("men.rpx", cosAppXmlInfoStruct.rpx_name) == 0)
	{
		log_deinit();
		return EXIT_RELAUNCH_ON_LOAD;
	}
	else if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0)
	{
		log_deinit();
		return EXIT_RELAUNCH_ON_LOAD;
	}
	
	PatchMethodHooks();
	
	log_printf("Returning to the Wii U menu.\n");
	SYSLaunchMenu();
	
	log_deinit();
	
	return EXIT_RELAUNCH_ON_LOAD;
}

// clear both buffers
void clearScreen()
{
	int i;
	for( i = 0; i < 2; i++ )
	{
		int screen_buf0_size = OSScreenGetBufferSizeEx(0);
		int screen_buf1_size = OSScreenGetBufferSizeEx(1);
		
		// Clear screens
		OSScreenClearBufferEx(0, 0);
		OSScreenClearBufferEx(1, 0);
		
		// Flush the cache
		DCFlushRange((void *)0xF4000000, screen_buf0_size);
		DCFlushRange((void *)0xF4000000 + screen_buf0_size, screen_buf1_size);
		
		// Flip buffers
		OSScreenFlipBuffersEx(0);
		OSScreenFlipBuffersEx(1);
	}
}


