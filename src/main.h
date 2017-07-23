#ifndef _MAIN_H_
#define _MAIN_H_

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

//! C wrapper for our C++ functions
int Menu_Main(void);
int isInMiiMakerHBL();
int isInSplatoon();
void ApplyPatches();
void RestorePatches();
void deInit();

#ifdef __cplusplus
}
#endif

#endif
