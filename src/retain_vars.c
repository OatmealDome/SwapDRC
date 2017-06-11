#include "common/types.h"

int isSplatoon __attribute__((section(".data"))) = 0;
int drcMode __attribute__((section(".data"))) = 0;
int screenPressTimeout __attribute__((section(".data"))) = 0;
uint32_t* gamemode __attribute__((section(".data"))) = (uint32_t*)0;