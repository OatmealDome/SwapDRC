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
#include "system/memory.h"
#include "common/common.h"
#include "patcher/function_hooks.h"
#include "kernel/kernel_functions.h"
#include "gecko/pygecko.h"
#include "cafiine/cafiine.h"
#include "retain_vars.h"

#define PRINT_TEXT1(x, y, str) { OSScreenPutFontEx(1, x, y, str); }
#define PRINT_TEXT2(x, y, ...) { snprintf(msg, 80, __VA_ARGS__); OSScreenPutFontEx(0, x, y, msg); OSScreenPutFontEx(1, x, y, msg); }
#define PRINT_TEXT3(x, y, _fmt, ...) { __os_snprintf(msg, 80, _fmt, __VA_ARGS__); OSScreenPutFontEx(1, x, y, msg); }

#define BUILD 1.0


/* IP union */
typedef union u_serv_ip
{
	uint8_t  digit[4];
	uint32_t full;
} u_serv_ip;

void gambitBootScreen()
{
	memoryInitialize();
	// Init screen and screen buffers
	OSScreenInit();
	int screen_buf0_size = OSScreenGetBufferSizeEx(0);
	int screen_buf1_size = OSScreenGetBufferSizeEx(1);
	unsigned char *screenBuffer = MEM1_alloc(screen_buf0_size + screen_buf1_size, 0x100);
	char msg[80];

	OSScreenSetBufferEx(0, screenBuffer);
	OSScreenSetBufferEx(1, (screenBuffer + screen_buf0_size));

	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);

	PRINT_TEXT2(0, 1, "Splatoon detected! Enhancing swap controls...");
	PRINT_TEXT2(2, 4, "Tips:");
	PRINT_TEXT2(2, 6, "Press B to swap displays except in-game menus.");
	PRINT_TEXT2(2, 7, "Hold A + D-PAD to super jump to a teammate in a match.");

	PRINT_TEXT2(10, 10, "           Teammate 2");
	PRINT_TEXT2(10, 11, "               _  ");
	PRINT_TEXT2(10, 12, "             _| |_");
	PRINT_TEXT2(10, 13, "Teammate 1  |_   _|  Teammate 3");
	PRINT_TEXT2(10, 14, "              |_|");
	PRINT_TEXT2(10, 16, "           Spawn Point");

	PRINT_TEXT2(50, 9, " ____________");
	PRINT_TEXT2(50, 10, "| Teammate 1 |");
	PRINT_TEXT2(50, 10, " ____________");
	PRINT_TEXT2(50, 11, " ____________");
	PRINT_TEXT2(50, 12, "| Teammate 2 |");
	PRINT_TEXT2(50, 12, " ____________");
	PRINT_TEXT2(50, 13, " ____________");
	PRINT_TEXT2(50, 14, "| Teammate 3 |");
	PRINT_TEXT2(50, 14, " ____________");
	PRINT_TEXT2(55, 15, " _______");
	PRINT_TEXT2(55, 16, "| Spawn |");
	PRINT_TEXT2(55, 17, "| Point |");
	PRINT_TEXT2(55, 17, " _______");

	OSScreenFlipBuffersEx(1);

	sleep(1);
	MEM1_free(screenBuffer);
	screenBuffer = NULL;

	memoryRelease();
}

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
	
	log_init("192.168.2.18");
	
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

	// Check for Splatoon (Gambit)
	if (strcasecmp("Gambit.rpx", cosAppXmlInfoStruct.rpx_name) == 0)
	{
		log_printf("Splatoon enhanced swapping enabled.\n");
		gambitBootScreen();
		isSplatoon = 1;
	}
	else if (isSplatoon == 1 && strcasecmp("miiverse_post.rpx", cosAppXmlInfoStruct.rpx_name) != 0)
	{
		log_printf("Splatoon enhanced swapping disabled.\n");
		isSplatoon = 0;
	}

	log_printf("Starting the TCPGecko server.\n");
	start_pygecko();

	if (patched == 0)
	{
		memoryInitialize();

		VPADInit();

		// Init screen and screen buffers
		OSScreenInit();
		int screen_buf0_size = OSScreenGetBufferSizeEx(0);
		int screen_buf1_size = OSScreenGetBufferSizeEx(1);
		unsigned char *screenBuffer = MEM1_alloc(screen_buf0_size + screen_buf1_size, 0x100);
		char msg[80];

		OSScreenSetBufferEx(0, screenBuffer);
		OSScreenSetBufferEx(1, (screenBuffer + screen_buf0_size));

		OSScreenEnableEx(0, 1);
		OSScreenEnableEx(1, 1);

		// Render IP selector and check for buttons
		u_serv_ip ip;
		ip.full = ((192 << 24) | (168 << 16) | (2 << 8) | (18 << 0));
		VPADData vpad_data;
		s32 error;
		int delay = 0;
		int gui_mode = 0;
		int success = 0; // for exiting the menu with the home button
		int sel_ip = 3;

		while (1)
		{
			// Refresh TV and GamePad screen
			OSScreenFlipBuffersEx(0);
			OSScreenFlipBuffersEx(1);
			OSScreenClearBufferEx(0, 0);
			OSScreenClearBufferEx(1, 0);

			// Read vpad
			VPADRead(0, &vpad_data, 1, &error);

			// Title
			PRINT_TEXT2(20, 1, "-- Swap DRC v%.1f --", BUILD);

			if (gui_mode == 0) // IP selector
			{
				PRINT_TEXT2(0, 4, "   IP : %3d.%3d.%3d.%3d", ip.digit[0], ip.digit[1], ip.digit[2], ip.digit[3]);
				PRINT_TEXT2(0, 6, "Use the D-Pad to enter in your computer's IP address for Cafiine.");
				PRINT_TEXT2(0, 8, "Press A to install with TCPGecko.");
				PRINT_TEXT2(0, 9, "Press Y to install with TCPGecko + Cafiine. (needs server running)");
				PRINT_TEXT2(0, 11, "Press B to view guide.");
				PRINT_TEXT2(0, 12, "Press X for credits.");
				PRINT_TEXT2(0, 15, "Press Home to exit.");


				PRINT_TEXT2(8 + 4 * sel_ip, 3, "vvv");

			}
			else if (gui_mode == 1) // Credits
			{
				PRINT_TEXT2(0, 3, "Creators:");
				PRINT_TEXT2(2, 5, "* Oatmealdome and Yahya14");
				PRINT_TEXT2(0, 7, "Special Thanks:");
				PRINT_TEXT2(2, 9, "* Maschell for HID to VPAD");
				PRINT_TEXT2(2, 10, "* Dimok for function_hooks and the pygecko server");
				PRINT_TEXT2(2, 11, "* brienj for the IP selector and initial UI");
				PRINT_TEXT2(2, 12, "* /u/MachMatic for the banner background");
				PRINT_TEXT2(2, 13, "* BKOOL999 for testing the Swap DRC app");
				PRINT_TEXT2(0, 15, "Press B to return to the menu.");
			}

			else if (gui_mode == 2) // Guide
			{
				PRINT_TEXT2(0, 2, " ___                               ___ ");
				PRINT_TEXT2(0, 3, " ------------------------------------- ");
				PRINT_TEXT2(0, 3, "/                                     \\");
				PRINT_TEXT2(0, 4, "|          _________________          |");
				PRINT_TEXT2(0, 5, "|    _    |                 |         |");
				PRINT_TEXT2(0, 6, "|  _| |_  |                 |    O    |");
				PRINT_TEXT2(0, 7, "| |_   _| |                 |  O   O  |");
				PRINT_TEXT2(0, 8, "|   |_|   |                 |    O    |");
				PRINT_TEXT2(0, 9, "|         |                 | +       |");
				PRINT_TEXT2(0, 9, "|          _________________          |");
				PRINT_TEXT2(0,10, "|                  ~          -       |");
				PRINT_TEXT2(0,11, "|_____________________________________|");

				PRINT_TEXT2(0, 1, " |||");
				PRINT_TEXT2(0, 2, " vvv");
				PRINT_TEXT2(0,10, "                                <---");


				PRINT_TEXT2(0,13, "Hold L then press Minus to swap displays.");
				PRINT_TEXT2(0, 15, "Press B to return to the menu.");

				
			}

			if ((vpad_data.btns_h & VPAD_BUTTON_A) && gui_mode == 0)
			{
				// Set wait message
				OSScreenClearBufferEx(1, 0);
				OSScreenClearBufferEx(0, 0);
				PRINT_TEXT2(42, 17, "Installing TCPGecko...");
				ip.full = 0;
				OSScreenFlipBuffersEx(1);
				break;
			}
			else if ((vpad_data.btns_h & VPAD_BUTTON_Y) && gui_mode == 0)
			{
				// Set wait message
				OSScreenClearBufferEx(1, 0);
				OSScreenClearBufferEx(0, 0);
				PRINT_TEXT2(42, 17, "Installing geckiine...");
				OSScreenFlipBuffersEx(1);
				break;
			}
			else if (vpad_data.btns_h & VPAD_BUTTON_X)
			{
				if (--delay <= 0)
				{
					// Swap GUI mode
					gui_mode = (gui_mode == 0) ? 1 : 0;
					delay = 100;
				}
			}
			else if (vpad_data.btns_h & VPAD_BUTTON_B)
			{
				if (--delay <= 0)
				{
					// Swap GUI mode
					gui_mode = (gui_mode == 0) ? 2 : 0;
					delay = 100;
				}
			}
			else if ((vpad_data.btns_h & VPAD_BUTTON_LEFT) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					if (sel_ip == 0)
						sel_ip = 3;
					else
						sel_ip--;

					delay = 8;
				}
			}
			else if ((vpad_data.btns_h & VPAD_BUTTON_RIGHT) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					sel_ip++;
					sel_ip = sel_ip % 4;
					delay = 8;
				}
			}
			else if ((vpad_data.btns_d & VPAD_BUTTON_UP) && gui_mode == 0) // add large delay at first 1/2
			{
				if (--delay <= 0)
				{
					ip.digit[sel_ip]++;
					delay = 10;
				}
			}
			else if ((vpad_data.btns_h & VPAD_BUTTON_UP) && gui_mode == 0) // then it gets shorter when holding it 2/2
			{
				if (--delay <= 0)
				{
					ip.digit[sel_ip]++;
					delay = 2;
				}
			}
			else if ((vpad_data.btns_d & VPAD_BUTTON_DOWN) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					ip.digit[sel_ip]--;
					delay = 10;
				}
			}
			else if ((vpad_data.btns_h & VPAD_BUTTON_DOWN) && gui_mode == 0)
			{
				if (--delay <= 0)
				{
					ip.digit[sel_ip]--;
					delay = 2;
				}
			}
			else if ((vpad_data.btns_h & VPAD_BUTTON_HOME) && gui_mode == 0)
			{
				success = -1;
				break;
			}
			else
			{
				delay = 0;
			}
		}

		MEM1_free(screenBuffer);
		screenBuffer = NULL;

		memoryRelease();

		//return to HBV
		if (success == -1)
		{
			RestoreInstructions();
			return EXIT_SUCCESS;
		}

		SYSLaunchMenu();

		patched = 1;
		new_addr = ip.full;

		log_printf("Returning to application.\n");
		log_printf("De-initializing logging.\n");
		log_deinit();
	}

	return EXIT_RELAUNCH_ON_LOAD;
}
