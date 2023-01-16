#include "OS_Defs.h"

void Pump_TX_Control(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18EF2000;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    if(u8_Enable_Cooling_Pump)
        t_Msg.au8_Data[0] = 70;
    else
        t_Msg.au8_Data[0] = 0;

    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = u8_Enable_Cooling_Pump;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Pump, &t_Msg);
}
