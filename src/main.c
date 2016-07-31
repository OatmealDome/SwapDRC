#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "main.h"
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

/* IP union */
typedef union u_serv_ip
{
	uint8_t  digit[4];
	uint32_t full;
} u_serv_ip;

static unsigned int patched = 0;

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
	
	log_printf("Geckiine started (rpx = '%s')\n", cosAppXmlInfoStruct.rpx_name);
	
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
	
	PatchMethodHooks();
	
	if (strcasecmp("men.rpx", cosAppXmlInfoStruct.rpx_name) == 0)
	{
		log_printf("Wii U menu started, exiting.\n");
		
		log_deinit();
		return EXIT_RELAUNCH_ON_LOAD;
	}
	
	if (strlen(cosAppXmlInfoStruct.rpx_name) == 0)
	{
		log_printf("Returning to the Wii U menu.\n");
		
		SYSLaunchMenu();
		
		log_deinit();
		return EXIT_RELAUNCH_ON_LOAD;
	}
	
	log_printf("Starting the TCPGecko server.\n");
	start_pygecko();
	
	if (!patched)
	{
		// Prepare screen
		int screen_buf0_size = 0;
		char msg[80];
		
		// Init screen and screen buffers
		OSScreenInit();
		screen_buf0_size = OSScreenGetBufferSizeEx(0);
		OSScreenSetBufferEx(0, (void *)0xF4000000);
		OSScreenSetBufferEx(1, (void *)0xF4000000 + screen_buf0_size);
		
		OSScreenEnableEx(0, 1);
		OSScreenEnableEx(1, 1);
		
		// Render IP selector and check for buttons
		u_serv_ip ip;
		ip.full = ( (192<<24) | (168<<16) | (2<<8) | (15<<0) );
		VPADData vpad_data;
		int error;
		int delay = 0;
		int gui_mode = 0;
		int sel_ip = 3;
		
		while (1)
		{
			// Refresh GamePad screen
			OSScreenFlipBuffersEx(1);
			OSScreenClearBufferEx(1, 0);
			
			// Read vpad
			VPADRead(0, &vpad_data, 1, &error);
			
			// Title
			PRINT_TEXT1(23, 1, "-- geckiine --");
			
			if (gui_mode == 0) // IP selector
			{
				PRINT_TEXT2(3, 4,  "   IP : %3d.%3d.%3d.%3d", ip.digit[0], ip.digit[1], ip.digit[2], ip.digit[3]);
				PRINT_TEXT1(3, 6,  "Use the D-Pad to enter in your computer's IP address,");
				PRINT_TEXT1(3, 7,  "then press A to install geckiine.");
				PRINT_TEXT1(3, 10, "Press X for credits.");
				
				PRINT_TEXT1(11 + 4 * sel_ip, 3, "vvv");

			}
			else if (gui_mode == 1) // Credits
			{
				PRINT_TEXT1(3, 4, "* Maschell for HID to VPAD");
				PRINT_TEXT1(3, 5, "* Dimok for function_hooks and the pygecko server");
				PRINT_TEXT1(3, 6, "* brienj for the IP selector and initial UI");
				PRINT_TEXT1(3, 7, "* amiibu for help with the PHP downloader script");
				PRINT_TEXT1(3, 8, "* seresaa for the banner and Geckiine Creator");
				PRINT_TEXT1(3, 9, "* 466 for web hosting");
				PRINT_TEXT1(3, 10, "* /u/MachMatic for the banner background");
				PRINT_TEXT1(3, 13, "Press X to return to the IP selector.");
			}
			
			if ((vpad_data.btn_hold & BUTTON_A) && gui_mode == 0)
			{
				// Set wait message
				OSScreenClearBufferEx(1, 0);
				PRINT_TEXT1(43, 17, "Installing geckiine...");
				OSScreenFlipBuffersEx(1);
				break;
			}
			else if (vpad_data.btn_hold & BUTTON_X)
			{
				if (--delay <= 0)
				{
					// Swap GUI mode
					gui_mode = (gui_mode == 0) ? 1 : 0;
					delay = 100;
				}
			}
			else if ((vpad_data.btn_hold & BUTTON_LEFT) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					if (sel_ip == 0)
						sel_ip = 3;
					else
						sel_ip--;
					
					delay = 12;
				}
			}
			else if ((vpad_data.btn_hold & BUTTON_RIGHT) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					sel_ip++;
					sel_ip = sel_ip % 4;
					delay = 12;
				}
			}
			else if ((vpad_data.btn_hold & BUTTON_UP) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					ip.digit[sel_ip]++;
					delay = 12;
				}
			}
			else if ((vpad_data.btn_hold & BUTTON_DOWN) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					ip.digit[sel_ip]--;
					delay = 12;
				}
			}
			else
			{
				delay = 0;
			}
		}
		
		patched = 1;
		new_addr = ip.full;
	}
	
	log_printf("Returning to application.\n");
	log_deinit();
	
	return EXIT_RELAUNCH_ON_LOAD;
}

// clear both buffers
void clearScreen()
{
	int i;
	for (i = 0; i < 2; i++)
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


