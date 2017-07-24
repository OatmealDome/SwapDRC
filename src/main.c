#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "main.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/ax_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/proc_ui_functions.h"
#include "utils/logger.h"
#include "system/memory.h"
#include "common/common.h"
#include "patcher/cafiine_function_patcher.h"
#include "patcher/voice_swapping_function_patcher.h"
#include "patcher/video_swapping_function_patcher.h"
#include "patcher/vpad_function_patcher.h"
#include "patcher/proc_ui_function_patcher.h"
#include "kernel/kernel_functions.h"
#include "gecko/pygecko.h"
#include "cafiine/cafiine.h"
#include "retain_vars.h"

#define PRINT_TEXT1(x, y, str) { OSScreenPutFontEx(1, x, y, str); }
#define PRINT_TEXT2(x, y, ...) { snprintf(msg, 80, __VA_ARGS__); OSScreenPutFontEx(0, x, y, msg); OSScreenPutFontEx(1, x, y, msg); }
#define PRINT_TEXT3(x, y, _fmt, ...) { __os_snprintf(msg, 80, _fmt, __VA_ARGS__); OSScreenPutFontEx(1, x, y, msg); }

#define BUILD 1.2

u8 isFirstBoot __attribute__((section(".data"))) = 1;

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

	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);

	os_sleep(1);
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
	InitAXFunctionPointers();
	InitProcUIFunctionPointers();

    log_init("192.168.0.181");

	SetupKernelCallback();

	log_printf("SWAP DRC started\n");

	memset(gVoiceInfos, 0, sizeof(gVoiceInfos));
	memset(&fspatchervars,0,sizeof(fspatchervars));

	 //Reset everything when were going back to the Mii Maker
    if(!isFirstBoot && isInMiiMakerHBL()){
        log_printf("Returing to the Homebrew Launcher!\n");
        isFirstBoot = 0;
        deInit();
        return EXIT_SUCCESS;
    }

	ApplyPatches();
	log_printf("Starting the TCPGecko server.\n");
	start_pygecko();

	if(!isInMiiMakerHBL()){ //Starting the application
        // Check for Splatoon (Gambit)
        if (isInSplatoon())
        {
            log_printf("Splatoon enhanced swapping enabled.\n");
            gambitBootScreen();
            isSplatoon = 1;
        }
		else {
			log_printf("Splatoon enhanced swapping disabled.\n");
			isSplatoon = 0;
		}

        return EXIT_RELAUNCH_ON_LOAD;
    }

    log_printf("Not in Mii Maker\n");

    if(isFirstBoot){ // First boot back to SysMenu
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
                PRINT_TEXT2(2, 9, "* Maschell for development contribution and the audio swap");
                PRINT_TEXT2(2, 10, "* Dimok, Chadderz, etc for function_hooks and pygecko");
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


                PRINT_TEXT2(0, 13, "Hold L then press Minus to swap displays.");
                PRINT_TEXT2(0, 14, "You can also press the TV button.");
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
            RestorePatches();
            return EXIT_SUCCESS;
        }

        cafiine_addr = ip.full;

        log_printf("Returning to application.\n");
        log_printf("De-initializing logging.\n");

        isFirstBoot = 0;
        SYSLaunchMenu();
    }
	return EXIT_RELAUNCH_ON_LOAD;
}

/*
    Patching all the functions!!!
*/
void ApplyPatches(){
    PatchInvidualMethodHooks(method_hooks_cafiine,          method_hooks_size_cafiine,          method_calls_cafiine);
    PatchInvidualMethodHooks(method_hooks_voice_swapping,   method_hooks_size_voice_swapping,   method_calls_voice_swapping);
    PatchInvidualMethodHooks(method_hooks_video_swapping,   method_hooks_size_video_swapping,   method_calls_video_swapping);
    PatchInvidualMethodHooks(method_hooks_vpad,             method_hooks_size_vpad,             method_calls_vpad);
    PatchInvidualMethodHooks(method_hooks_proc_ui,          method_hooks_size_proc_ui,          method_calls_proc_ui);
}

/*
    Restoring everything!!
*/

void RestorePatches(){
    RestoreInvidualInstructions(method_hooks_cafiine,           method_hooks_size_cafiine);
    RestoreInvidualInstructions(method_hooks_voice_swapping,    method_hooks_size_voice_swapping);
    RestoreInvidualInstructions(method_hooks_video_swapping,    method_hooks_size_video_swapping);
    RestoreInvidualInstructions(method_hooks_vpad,              method_hooks_size_vpad);
    RestoreInvidualInstructions(method_hooks_proc_ui,           method_hooks_size_proc_ui);
    KernelRestoreInstructions();
}

void deInit(){
    RestorePatches();
    log_deinit();
}

int isInSplatoon(){
    if (OSGetTitleID != 0 && (
            OSGetTitleID() == 0x0005000010176a00 || // splatoon eur
            OSGetTitleID() == 0x0005000010176900 || // splatoon usa
            OSGetTitleID() == 0x0005000010162b00)){   // splatoon jpn
            return 1;
    }
    return 0;
}

int isInMiiMakerHBL(){
    if (OSGetTitleID != 0 && (
            OSGetTitleID() == 0x000500101004A200 || // mii maker eur
            OSGetTitleID() == 0x000500101004A100 || // mii maker usa
            OSGetTitleID() == 0x000500101004A000 || // mii maker jpn
            OSGetTitleID() == 0x0005000013374842))
        {
            return 1;
    }
    return 0;
}
