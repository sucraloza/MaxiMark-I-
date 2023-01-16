#include "OS_Defs.h"
#include "Batteries_Vars.h"

St_Battery_Vars st_Battery;
T_x_os_mutex t_Mutex_Battery;

St_Battery_Counters st_Battery_Counters;

void *mpv_MsgObjTx_Battery; // module global defined TX object for Batteries
void *mpv_MsgObjRx_Battery; // module global defined RX object for Batteries

T_x_can_msg t_Battery_Msg;          //Message Holder RX object
T_x_os_mutex t_Mutex_Battery_Msg;
