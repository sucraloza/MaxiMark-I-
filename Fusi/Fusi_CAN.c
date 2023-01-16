#include "OS_Defs.h"

void Fusi_TX_Flags_System(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C008817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_System.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_System.all >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((st_Fusi.un_System.all >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((st_Fusi.un_System.all >> 24) & 0xFF);
    t_Msg.au8_Data[4] = (uint8)((st_Fusi.un_System.all >> 32) & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((st_Fusi.un_System.all >> 40) & 0xFF);
    t_Msg.au8_Data[6] = (uint8)((st_Fusi.un_System.all >> 48) & 0xFF);
    t_Msg.au8_Data[7] = (uint8)((st_Fusi.un_System.all >> 56) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }

}

void Fusi_TX_Flags_Battery(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C018817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_Battery.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_Battery.all >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((st_Fusi.un_Battery.all >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((st_Fusi.un_Battery.all >> 24) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_PDU(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C028817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_PDU.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_PDU.all >> 8) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_Motor(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C038817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_Motor.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_Motor.all >> 8) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_Steering(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C048817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 1;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_Steering.all & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_AuxInverter(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C058817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_AuxInv.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_AuxInv.all >> 8) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_Air(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C068817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_Air.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_Air.all >> 8) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_Heating(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C078817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_Heating.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_Heating.all >> 8) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_Flags_Acctions(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18108817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 5;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.un_Actions.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.un_Actions.all >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)(st_Fusi.u16_DTC & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((st_Fusi.u16_DTC >> 8) & 0xFF);
    t_Msg.au8_Data[4] = st_Fusi.u8_DTC_Level;

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_TX_DTC_History(uint8 u8_CAN){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18118817;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8)(st_Fusi.u16_DTCs[0] & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Fusi.u16_DTCs[0] >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)(st_Fusi.u16_DTCs[1] & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((st_Fusi.u16_DTCs[1] >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8)(st_Fusi.u16_DTCs[2] & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((st_Fusi.u16_DTCs[2] >> 8) & 0xFF);
    t_Msg.au8_Data[6] = (uint8)(st_Fusi.u16_DTCs[3] & 0xFF);
    t_Msg.au8_Data[7] = (uint8)((st_Fusi.u16_DTCs[4] >> 8) & 0xFF);

    if(u8_CAN == 4){
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi4, &t_Msg);
    }
    else{
        x_can_obj_send_msg(mpv_MsgObjTx_Fusi, &t_Msg);
    }
}

void Fusi_Activate_DCU_Fault(void){
    if(t_Fusi_Msg.t_Id.u32_Id != 0x18001788u){
        return;
    }
    if(t_Fusi_Msg.au8_Data[0]){
        st_Fusi.un_AuxInv.bits.b2_DCU_Fault = FUSI_ON_FLAG;
    }
    else{
        st_Fusi.un_AuxInv.bits.b2_DCU_Fault = FUSI_OFF_FLAG;
    }
}

void Fusi_RX_Clear(void){

}

void Fusi_Send(void){
    st_Fusi_Counters.u8_Fast_Counter++;
    if(st_Fusi_Counters.u8_Fast_Counter >= FUSI_FAST_COUNTER_LIMIT){
        st_Fusi_Counters.u8_Fast_Counter = 0;
        Fusi_TX_Flags_Acctions(1);
    }
    if(st_Fusi.u16_DTC == 0){
        return;
    }
    st_Fusi_Counters.u8_Normal_Counter++;
    if(st_Fusi_Counters.u8_Normal_Counter >= FUSI_NORMAL_COUNTER_LIMIT){
        st_Fusi_Counters.u8_Normal_Counter = 0;
        Fusi_TX_Flags_System(1);
        Fusi_TX_Flags_Battery(1);
        Fusi_TX_Flags_PDU(1);
        Fusi_TX_Flags_Motor(1);
    }
    st_Fusi_Counters.u8_Slow_Counter++;
    if(st_Fusi_Counters.u8_Slow_Counter >= FUSI_SLOW_COUNTER_LIMIT){
        st_Fusi_Counters.u8_Slow_Counter = 0;
        Fusi_TX_Flags_Air(1);
        Fusi_TX_Flags_AuxInverter(1);
        Fusi_TX_Flags_Heating(1);
        Fusi_TX_Flags_Steering(1);
        Fusi_TX_DTC_History(1);
    }
}

void Fusi_Recived(void){
    sint16 s16_Error;
    sint16 s16_Error_Can;

    s16_Error = x_os_mutex_get(&t_Mutex_Fusi_Msg,10);
    if(s16_Error != C_NO_ERR){
        Error_Beep();
        return;
    }
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Fusi, &t_Fusi_Msg);
    while(s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR){
        switch(t_Fusi_Msg.t_Id.u32_Id){
            default:
                x_os_mutex_release(&t_Mutex_Fusi_Msg);
        }
        s16_Error = x_os_mutex_get(&t_Mutex_Fusi_Msg,10);    //Trato de obtener el mutex denuevo
        if(s16_Error != C_NO_ERR){
            break;                                              //No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Fusi, &t_Fusi_Msg);
    }
    x_os_mutex_release(&t_Mutex_Fusi_Msg);

}
