#include <stdio.h>
#include <string.h>
#include "dynamic_libs/vpad_functions.h"
#include "retain_vars.h"
#include "utils/logger.h"
#include "gambit_functions.h"
#include "function_hooks.h"

uint32_t* ptr = (uint32_t*)0x106E46E8;
uint32_t* ptr2 = (uint32_t*)0x106E4DA8;
bool AButton = 0;
int touchVal = 0;

void gambitDRC()
{
	if (*inkstrikeEq == 2 && *spTimer != 0)
		swapForce = 1;
	else if (AButton)
		swapForce = 1;
	else
		swapForce = 0;
}

void gambitTouch(VPADTPData *screen)
{
	switch (touchVal)
	{
	case D_LEFT:
		screen->x = 1080;
		screen->y = 130;
		screen->touched = 1;
		screen->invalid = 0;
		break;
	case D_UP:
		screen->x = 1080;
		screen->y = 290;
		screen->touched = 1;
		screen->invalid = 0;
		break;
	case D_RIGHT:
		screen->x = 1080;
		screen->y = 450;
		screen->touched = 1;
		screen->invalid = 0;
		break;
	case D_DOWN:
		screen->x = 1170;
		screen->y = 625;
		screen->touched = 1;
		screen->invalid = 0;
		break;
	case D_NEUTRAL:
		// disable touchscreen input if the TV is on the DRC for Splatoon enhanced controls
		if ((drcMode == 0 || swapForce)) {}
		else if (AppInBackground)
		{
			screen->touched = 0;
		}
		break;
	}
}

void gambitPatches(VPADData *buffer)
{
	// prevents the game from crashing
	if (*ptr < 0x1C000000)
	{
		inkstrikeEq = (uint32_t*)0x10000000;
		spTimer = (uint32_t*)0x10000000;
	}
	else
	{
		// define pointers
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

		inkstrikeEq = (uint32_t*)(*ptr + 0x80);
		spTimer = (uint32_t*)(*ptr + 0x808);

		// check if in-game menu is up
		if (*ptr2 > 0x1C000000)
		{
			// switch if B is pressed
			if (buffer->btns_d & VPAD_BUTTON_B && AppInBackground)
			{
				drcSwap();
			}

			// force temporary default swap in a match
			if (buffer->btns_h & VPAD_BUTTON_A)
			{
				if (*gamemode != 0xFFFFFFFF)
				{
					AButton = true;

					// super jumping with D-Pad
					if (buffer->btns_h & VPAD_BUTTON_LEFT)
					{
						touchVal = D_LEFT;
					}
					else if (buffer->btns_h & VPAD_BUTTON_UP)
					{
						touchVal = D_UP;
					}
					else if (buffer->btns_h & VPAD_BUTTON_RIGHT)
					{
						touchVal = D_RIGHT;
					}
					else if (buffer->btns_h & VPAD_BUTTON_DOWN)
					{
						touchVal = D_DOWN;
					}
					else
					{
						touchVal = D_NEUTRAL;
					}
				}
			}
			else
			{
				AButton = false;
				touchVal = D_NEUTRAL;
			}
		}
		else
		{
			AButton = false;
		}
	}
}
