#include "OS_Defs.h"

void *mpv_MsgObjTx_PTC1;
void *mpv_MsgObjRx_PTC1;

void *mpv_MsgObjTx_PTC4;
void *mpv_MsgObjRx_PTC4;

void PTC1_Recived(void){
    T_x_can_msg t_Msg;
    x_can_obj_get_msg(mpv_MsgObjRx_PTC1, &t_Msg);
    if(t_Msg.t_Id.u32_Id == 0x502){
            t_Msg.t_Id.u32_Id = 0x501;
        x_can_obj_send_msg(mpv_MsgObjTx_PTC4, &t_Msg);
    }
}

void PTC4_Recived(void){
    T_x_can_msg t_Msg;
    x_can_obj_get_msg(mpv_MsgObjRx_PTC4, &t_Msg);
    if(t_Msg.t_Id.u32_Id == 0x511){
            t_Msg.t_Id.u32_Id = 0x522;
        x_can_obj_send_msg(mpv_MsgObjTx_PTC1, &t_Msg);
    }
}

