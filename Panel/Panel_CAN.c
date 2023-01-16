#include "OS_Defs.h"

void Panel_Tx_Speed(void){
    uint8 u8_rpm;
    uint8 u8_Speed_kmh;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x250;
    t_Msg.t_Id.u8_Xtd = FALSE;
    t_Msg.u8_Dlc = 8;

    u8_rpm = st_Panel.u16_Rpm * 0.0238f;
    u8_Speed_kmh = st_Panel.u8_Speed_kmh * 0.7812f;

    t_Msg.au8_Data[0] = 0x09;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x01;
    t_Msg.au8_Data[3] = 0xFF;
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = u8_Speed_kmh;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = u8_rpm;

    x_can_obj_send_msg(mpv_MsgObjTx_Panel, &t_Msg);
}

void Panel_Tx_Indicators(void){

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x552;
    t_Msg.t_Id.u8_Xtd = FALSE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0x00;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0xFF;
    t_Msg.au8_Data[3] = 0xFF;
    t_Msg.au8_Data[4] = 0xFF;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0xFF;

    x_can_obj_send_msg(mpv_MsgObjTx_Panel, &t_Msg);
}

void Panel_Tx_Buzzer(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x254;
    t_Msg.t_Id.u8_Xtd = FALSE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (Fusi_get_Buzzer() & 0x3) << 4;
    t_Msg.au8_Data[0] |= (Fusi_get_Request_Stop() & 0x1) << 7;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Panel, &t_Msg);
}

void Panel_Tx_Check_Engine(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x65F;
    t_Msg.t_Id.u8_Xtd = FALSE;
    t_Msg.u8_Dlc = 8;

    switch(Fusi_get_Check_Engine()){
        case FUSI_CHECK_ENGINE_OFF:
            t_Msg.au8_Data[0] = 0x3;
            break;
        case FUSI_CHECK_ENGINE_ON:
            t_Msg.au8_Data[0] = 0x7;
            break;
        case FUSI_CHECK_ENGINE_BLINKING:
            t_Msg.au8_Data[0] = 0xB;
            break;
        default:
            t_Msg.au8_Data[0] = 0xF;
            break;
    }

    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Panel, &t_Msg);
}

void Send_Panel(void){
    if(Inverter_Get_Potocol_Flag())
    {
        st_Panel.u8_msg_Counter_Fast++;
        if(st_Panel.u8_msg_Counter_Fast >= PANEL_FAST_COUNTER_LIMIT){
            Panel_Tx_Speed();
            Panel_Tx_Buzzer();
            st_Panel.u8_msg_Counter_Fast = 0;
            return;
        }

        st_Panel.u8_msg_Counter_Slow++;
        if(st_Panel.u8_msg_Counter_Slow >= PANEL_SLOW_COUNTER_LIMIT){
            //Panel_Tx_Indicators();
            Panel_Tx_Check_Engine();
            st_Panel.u8_msg_Counter_Slow = 0;
            return;
        }
    }
}

