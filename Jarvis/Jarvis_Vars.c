#include "OS_Defs.h"

St_Jarvis_Vars st_Jarvis;
T_x_os_mutex t_Mutex_Jarvis;

void *mpv_MsgObjTx_Jarvis; // module global defined TX object for Jarvis
void *mpv_MsgObjRx_Jarvis; // module global defined RX object for Jarvis

T_x_can_msg t_Jarvis_Msg;          //Message Holder RX object
T_x_os_mutex t_Mutex_Jarvis_Msg;
