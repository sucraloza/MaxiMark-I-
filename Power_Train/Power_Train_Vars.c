#include "OS_Defs.h"

St_Inverter_Variables st_Inverter;
T_x_os_mutex t_Mutex_Inverter;

void *mpv_MsgObjTx_Inverter; // module global defined TX object
void *mpv_MsgObjRx_Inverter; // module global defined RX object

T_x_can_msg t_Inverter_Msg;
T_x_os_mutex t_Mutex_Inverter_Msg;
