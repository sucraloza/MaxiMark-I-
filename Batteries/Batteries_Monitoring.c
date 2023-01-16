#include "OS_Defs.h"

uint8 Battery_Monitoring_Turn;

void Battery_Monitoring_Current_Limit(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18C4EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Variable_Raw;
    uint8 u8_Low_Byte;
    uint8 u8_High_Byte;

    u16_Variable_Raw = st_Battery.st_Current_Limits.u16_Avail_Inpulse_Discharge_Current_A *10;
    u8_Low_Byte = (uint8)(u16_Variable_Raw & 0xFF);
    u8_High_Byte = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[0] = u8_Low_Byte;
    t_Msg.au8_Data[1] = u8_High_Byte;

    u16_Variable_Raw = st_Battery.st_Current_Limits.u16_Avail_Inpulse_Charge_Current_A *10;
    u8_Low_Byte = (uint8)(u16_Variable_Raw & 0xFF);
    u8_High_Byte = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[2] = u8_Low_Byte;
    t_Msg.au8_Data[3] = u8_High_Byte;

    u16_Variable_Raw = st_Battery.st_Current_Limits.u16_Avail_Sustain_Charge_Current_A *10;
    u8_Low_Byte = (uint8)(u16_Variable_Raw & 0xFF);
    u8_High_Byte = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[4] = u8_Low_Byte;
    t_Msg.au8_Data[5] = u8_High_Byte;

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);

}

void Battery_Monitoring_Electric_Energy(void);
void Battery_Monitoring_Fault_1(void);
void Battery_Monitoring_Fault_2(void);
void Battery_Monitoring_Temperature(void);
void Battery_Monitoring_Cell_Voltage(void);

void Batteries_Monitoring_State_1(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1883EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8)((st_Battery.un_State_1.all >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Battery.un_State_1.all >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((st_Battery.un_State_1.all >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((st_Battery.un_State_1.all >> 24) & 0xFF);
    t_Msg.au8_Data[4] = (uint8)((st_Battery.un_State_1.all >> 32) & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((st_Battery.un_State_1.all >> 40) & 0xFF);
    t_Msg.au8_Data[6] = (uint8)((st_Battery.un_State_1.all >> 48) & 0xFF);
    t_Msg.au8_Data[7] = (uint8)((st_Battery.un_State_1.all >> 56) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Batteries_Monitoring_State_2(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1884EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    sint32 s32_Variable_Raw;
    uint16 u16_Variable_Raw;

    t_Msg.au8_Data[0] = ((uint8)(st_Battery.st_State_2.u8_SOC * 2.5f)) & 0xFF;
    t_Msg.au8_Data[1] = ((uint8)(st_Battery.st_State_2.u8_SOH * 2.5f)) & 0xFF;

    s32_Variable_Raw = (st_Battery.st_State_2.s16_Pack_Current_A + 1000) * 10;

    t_Msg.au8_Data[2] = (uint8)(s32_Variable_Raw & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((s32_Variable_Raw >> 8) & 0xFF);

    u16_Variable_Raw = st_Battery.st_State_2.u16_Pack_Inside_Voltage_V * 10;

    t_Msg.au8_Data[4] = (uint8)(u16_Variable_Raw & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[6] = st_Battery.st_State_2.u8_Fault_Code;
    t_Msg.au8_Data[7] = st_Battery.st_State_2.un_Flags.all;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Batteries_Monitoring_State_3(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1885EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8)(st_Battery.st_State_3.u16_Positive_Insulation_Resistance_kohm & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((st_Battery.st_State_3.u16_Positive_Insulation_Resistance_kohm >> 8) & 0xFF);

    t_Msg.au8_Data[2] = (uint8)(st_Battery.st_State_3.u16_Negative_Insulation_Resistance_kohm & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((st_Battery.st_State_3.u16_Negative_Insulation_Resistance_kohm >> 8) & 0xFF);

    t_Msg.au8_Data[4] = (uint8)(st_Battery.st_State_3.u16_System_Insulation_Resistance_kohm & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((st_Battery.st_State_3.u16_System_Insulation_Resistance_kohm >> 8) & 0xFF);

    t_Msg.au8_Data[6] = st_Battery.st_State_3.u8_Insolation_Monitor_Status;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Batteries_Monitoring_State_4(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1886EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    sint16 s16_Variable_Raw;

    s16_Variable_Raw = st_Battery.st_State_4.s8_Max_Temperature_C + 40;
    t_Msg.au8_Data[0] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_4.s8_Min_Temperature_C + 40;
    t_Msg.au8_Data[1] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_4.s8_Avg_Temperature_C + 40;
    t_Msg.au8_Data[2] = (uint8)(s16_Variable_Raw & 0xFF);

    t_Msg.au8_Data[3] = st_Battery.st_State_4.u8_Max_Temperature_Cell_Number;
    t_Msg.au8_Data[4] = st_Battery.st_State_4.u8_Max_Temperature_Position;
    t_Msg.au8_Data[5] = st_Battery.st_State_4.u8_Min_Temperature_Cell_Number;
    t_Msg.au8_Data[6] = st_Battery.st_State_4.u8_Min_Temperature_Position;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Batteries_Monitoring_State_5(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1887EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Variable_Raw;

    u16_Variable_Raw =  st_Battery.st_State_5.f32_Max_Cell_Voltage_V * 1000;
    t_Msg.au8_Data[0] = (uint8)(u16_Variable_Raw & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    u16_Variable_Raw =  st_Battery.st_State_5.f32_Min_Cell_Voltage_V * 1000;
    t_Msg.au8_Data[2] = (uint8)(u16_Variable_Raw & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    u16_Variable_Raw =  st_Battery.st_State_5.f32_Avg_Cell_Voltage_V * 1000;
    t_Msg.au8_Data[4] = (uint8)(u16_Variable_Raw & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void Batteries_Monitoring_State_6(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1888EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Variable_Raw;

    t_Msg.au8_Data[0] = st_Battery.st_State_6.u8_Max_Voltage_Cell_Number;

    u16_Variable_Raw =  st_Battery.st_State_6.u16_Max_Voltage_Cell_Position;
    t_Msg.au8_Data[1] = (uint8)(u16_Variable_Raw & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[3] = st_Battery.st_State_6.u8_Min_Voltage_Cell_Number;

    u16_Variable_Raw =  st_Battery.st_State_6.u16_Min_Voltage_Cell_Position;
    t_Msg.au8_Data[4] = (uint8)(u16_Variable_Raw & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u16_Variable_Raw >> 8) & 0xFF);

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void Batteries_Monitoring_State_7(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1889EFF3;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    sint16 s16_Variable_Raw;

    s16_Variable_Raw = st_Battery.st_State_7.s8_Gun_1_DC_Positive_Temperature_C + 40;
    t_Msg.au8_Data[0] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_7.s8_Gun_1_DC_Negitive_Temperature_C + 40;
    t_Msg.au8_Data[1] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_7.s8_Gun_2_DC_Positive_Temperature_C + 40;
    t_Msg.au8_Data[2] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_7.s8_Gun_2_DC_Negitive_Temperature_C + 40;
    t_Msg.au8_Data[3] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_7.s8_Gun_AC_Positive_Temperature_C + 40;
    t_Msg.au8_Data[4] = (uint8)(s16_Variable_Raw & 0xFF);

    s16_Variable_Raw = st_Battery.st_State_7.s8_Gun_AC_Negitive_Temperature_C + 40;
    t_Msg.au8_Data[5] = (uint8)(s16_Variable_Raw & 0xFF);

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void Batteries_Send_Monitoring(void){
    switch(Battery_Monitoring_Turn){
        case 0:
            Battery_TX_Command();
            break;
        case 1:
            Batteries_Monitoring_State_1();
            break;
        case 2:
            Batteries_Monitoring_State_2();
            break;
        case 3:
            Batteries_Monitoring_State_3();
            break;
        case 4:
            Batteries_Monitoring_State_4();
            break;
        case 5:
            Batteries_Monitoring_State_5();
            break;
        case 6:
            Batteries_Monitoring_State_6();
            break;
        case 7:
            Batteries_Monitoring_State_7();
            break;
        case 8:
            Battery_Monitoring_Current_Limit();
            break;
        default:
            Battery_Monitoring_Turn = 0;

    }
    Battery_Monitoring_Turn++;
}
