#include "main.h"
#include "retain_vars.h"

int __entry_menu(int argc, char **argv){
    if(gAppStatus == 2){
        return EXIT_RELAUNCH_ON_LOAD;
    }
    gAppStatus = 0;
    //! *******************************************************************
    //! *                 Jump to our application                    *
    //! *******************************************************************
    return Menu_Main();
}
