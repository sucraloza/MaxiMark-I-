#include "OS_Defs.h"



void Isometer_RX_Status(void){
    if(t_Isometer_Msg.t_Id.u32_Id != 0x1819A1A4u){
        return;
    }

    uint8 u8_Flag;
    uint8 u8_Alarm;
    uint16 u16_Isolation_Recistance_kOhm;
    uint16 u16_Voltage_V;
    uint8 u8_Life_Signal;

    u16_Isolation_Recistance_kOhm = t_Isometer_Msg.au8_Data[0];
    u16_Isolation_Recistance_kOhm = u16_Isolation_Recistance_kOhm <<8;
    u16_Isolation_Recistance_kOhm += t_Isometer_Msg.au8_Data[1];

    u16_Voltage_V = t_Isometer_Msg.au8_Data[2];
    u16_Voltage_V = u16_Voltage_V <<8;
    u16_Voltage_V += t_Isometer_Msg.au8_Data[3];
    u16_Voltage_V = u16_Voltage_V * 0.1f;

    u8_Alarm = t_Isometer_Msg.au8_Data[4];
    u8_Flag = t_Isometer_Msg.au8_Data[6];

    u8_Life_Signal = t_Isometer_Msg.au8_Data[7];

    if(x_os_mutex_get(&t_Mutex_Isometer,10) != C_NO_ERR){
        return;
    }

    st_Isometer_Vars.u8_Alarm = u8_Alarm;
    st_Isometer_Vars.u8_State = u8_Flag;
    st_Isometer_Vars.u16_Isulation_Resistance_kOhm = u16_Isolation_Recistance_kOhm;
    st_Isometer_Vars.u16_Voltage_V = u16_Voltage_V;
    st_Isometer_Vars.u8_Life_Signal = u8_Life_Signal;
    x_os_mutex_release(&t_Mutex_Isometer);
}

void Recived_Isometer(void){
    sint16 s16_Error;
    sint16 s16_Error_Can;

    s16_Error = x_os_mutex_get(&t_Mutex_Isometer_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Isometer, &t_Isometer_Msg);
    while(s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR){

        switch(t_Isometer_Msg.t_Id.u32_Id){
            case 0x1819A1A4u:
                x_os_mutex_release(&t_Mutex_Isometer_Msg);
                Isometer_RX_Status();
                break;
            default:
                x_os_mutex_release(&t_Mutex_Isometer_Msg);
        }
        s16_Error = x_os_mutex_get(&t_Mutex_Isometer_Msg,10);    //Trato de obtener el mutex denuevo
        if(s16_Error != C_NO_ERR){
            break;                                              //No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Isometer, &t_Isometer_Msg);
    }
    x_os_mutex_release(&t_Mutex_Isometer_Msg);
}


