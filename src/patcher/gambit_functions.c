#include "dynamic_libs/vpad_functions.h"
#include "retain_vars.h"
#include "utils/logger.h"
#include "gambit_functions.h"

unsigned int PCoord[3][3];

uint32_t* ptr = (uint32_t*)0x106E46E8;
uint32_t* ptr2 = (uint32_t*)0x106E4DA8;


int gambitDRC()
{
	if (isSplatoon && *inkstrikeEq == 2 && *spTimer != 0)
		return 1;
	else
		return 0;
}

void gambitPatcher(VPADData *buffer)
{
	// prevents the game from crashing
	if (*ptr2 < 0x1D000000)
	{
		inkstrikeEq = (uint32_t*)0x10000000;
		spTimer = (uint32_t*)0x10000000;
	}
	else
	{
		// define pointers
		inkstrikeEq = (uint32_t*)(*ptr + 0x80);
		spTimer = (uint32_t*)(*ptr + 0x808);

		// switch if B is pressed in non-menu areas
		if (buffer->btns_d & VPAD_BUTTON_B)
		{
			drcMode = !drcMode;
		}
		
		////DEV
		int j = 0;
		// find team side for each player
		// you
		uint32_t* teamYou = (uint32_t*)(*ptr + 0x2C);

		// other
		for (int i = 0; i < 8; i++)
		{
			uint32_t* pbase = (uint32_t*)(*ptr2 + 0x56C + (4 * i));
			uint32_t* team = (uint32_t*)(*pbase + 0x2C);

			// add allies' coords into an array
			if (*team == teamYou && *ptr != *pbase)
			{
				for (int k = 0; k < 3; k++)
				{
					PCoord[j][k] = *pbase + 0x248 + (4 * k);
				}

				j++;

			}
		}

		////
	}
}
