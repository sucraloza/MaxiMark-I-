#include "OS_Defs.h"

#define CAPL_ACTIVE 1
#if(CAPL_ACTIVE)

void CAPL_Send_Batt_On(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18001114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Isometer_On(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18011114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Votage_On(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18021114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Votage_Off(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18031114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_Votage_On(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18041114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_Votage_Off(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18051114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_Operational(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18061114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_Standby(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18071114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_Power_Off(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18081114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_EV(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18091114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Inv_Neutral(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180A1114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Batt_Off(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180B1114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;

     x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}


void CAPL_Send_Battery_Timer(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180C1114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

void CAPL_Send_Battery_Ign_Off(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180D1114;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 0;
    x_can_obj_send_msg(mpv_MsgObjTx_CAPL, &t_Msg);
}

#else
     void CAPL_Send_Batt_On(void){}
     void CAPL_Send_Batt_Off(void){}
     void CAPL_Send_Isometer_On(void){}
     void CAPL_Send_Votage_On(void){}
     void CAPL_Send_Votage_Off(void){}

     void CAPL_Send_Inv_Votage_On(void){}
     void CAPL_Send_Inv_Votage_Off(void){}
     void CAPL_Send_Inv_Operational(void){}
     void CAPL_Send_Inv_Standby(void){}
     void CAPL_Send_Inv_Power_Off(void){}
     void CAPL_Send_Inv_EV(void){}
     void CAPL_Send_Inv_Neutral(void){}
     void CAPL_Send_Battery_Timer(void){}
     void CAPL_Send_Battery_Ign_Off(void){}
#endif

void *mpv_MsgObjTx_CAPL; // module global defined TX object for CAPL

