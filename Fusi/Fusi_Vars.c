#include "OS_Defs.h"

St_Fusi_Vars st_Fusi;

St_Counters_Fusi st_Fusi_Counters;

void *mpv_MsgObjTx_Fusi; // module global defined TX object for Fusi CAN 1
void *mpv_MsgObjTx_Fusi4; // module global defined TX object for Fusi CAN 4
void *mpv_MsgObjRx_Fusi; // module global defined TX object for Fusi

T_x_can_msg t_Fusi_Msg;  // Message Holder RX object
T_x_os_mutex t_Mutex_Fusi_Msg;
