#include "OS_Defs.h"

St_Isolation_Monitor_Vars st_Isometer_Vars;
T_x_os_mutex t_Mutex_Isometer;

void *mpv_MsgObjTx_Isometer; // module global defined TX object for Isometer
void *mpv_MsgObjRx_Isometer; // module global defined RX object for Isometer

T_x_can_msg t_Isometer_Msg;          //Message Holder RX object
T_x_os_mutex t_Mutex_Isometer_Msg;   //Message Isometer
