#include "OS_Defs.h"

void Battery_TX_Command(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1802F3EF;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8) (st_Battery.un_Command.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((st_Battery.un_Command.all>>8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Battery.un_Command.all>>16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Battery.un_Command.all>>24) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Battery.un_Command.all>>32) & 0xFF);
    t_Msg.au8_Data[5] = (uint8) ((st_Battery.un_Command.all>>40) & 0xFF);
    t_Msg.au8_Data[6] = (uint8) ((st_Battery.un_Command.all>>48) & 0xFF);
    t_Msg.au8_Data[7] = (uint8) ((st_Battery.un_Command.all>>56) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Battery, &t_Msg);
}

void  Battery_TX_VIN(void){

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18E1F3EF;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Battery, &t_Msg);


}

void  Battery_TX_Isometer(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1803F3EF;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = st_Battery.st_Isometer.un_Flags.all;
    //t_Msg.au8_Data[0] = 1;
    t_Msg.au8_Data[1] = (uint8) (st_Battery.st_Isometer.u16_Positive_Resistance & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Battery.st_Isometer.u16_Positive_Resistance>>8) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) (st_Battery.st_Isometer.u16_Negitive_Resistance & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Battery.st_Isometer.u16_Negitive_Resistance>>8) & 0xFF);
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Battery, &t_Msg);
}

void Battery_RX_Current_Limit(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x18C4EFF3){
        return;
    }

    uint16 u16_Inpulse_Discharge_A;
    uint16 u16_Inpulse_Charge_A;
    uint16 u16_Flow_Charge_A;

    u16_Inpulse_Discharge_A = t_Battery_Msg.au8_Data[1];
    u16_Inpulse_Discharge_A = u16_Inpulse_Discharge_A <<8;
    u16_Inpulse_Discharge_A = u16_Inpulse_Discharge_A + t_Battery_Msg.au8_Data[0];
    u16_Inpulse_Discharge_A = u16_Inpulse_Discharge_A * 0.1f;

    u16_Inpulse_Charge_A = t_Battery_Msg.au8_Data[3];
    u16_Inpulse_Charge_A = u16_Inpulse_Charge_A <<8;
    u16_Inpulse_Charge_A = u16_Inpulse_Charge_A + t_Battery_Msg.au8_Data[2];
    u16_Inpulse_Charge_A = u16_Inpulse_Charge_A * 0.1f;

    u16_Flow_Charge_A = t_Battery_Msg.au8_Data[5];
    u16_Flow_Charge_A = u16_Flow_Charge_A <<8;
    u16_Flow_Charge_A = u16_Flow_Charge_A + t_Battery_Msg.au8_Data[4];
    u16_Flow_Charge_A = u16_Flow_Charge_A * 0.1f;

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

    st_Battery.st_Current_Limits.u16_Avail_Inpulse_Discharge_Current_A = u16_Inpulse_Discharge_A;
    st_Battery.st_Current_Limits.u16_Avail_Inpulse_Charge_Current_A = u16_Inpulse_Charge_A;
    st_Battery.st_Current_Limits.u16_Avail_Sustain_Charge_Current_A = u16_Flow_Charge_A;

    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_RX_Electric_Energy(void);

void Battery_RX_Fault_1(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1881EFF3){
        return;
    }

    uint32 u32_Flag;

    u32_Flag = t_Battery_Msg.au8_Data[3];
    u32_Flag = u32_Flag <<8;
    u32_Flag += t_Battery_Msg.au8_Data[2];
    u32_Flag = u32_Flag <<8;
    u32_Flag += t_Battery_Msg.au8_Data[1];
    u32_Flag = u32_Flag <<8;
    u32_Flag += t_Battery_Msg.au8_Data[0];

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

    st_Battery.un_Faults_1.all = u32_Flag;

    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_RX_Fault_2(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1882EFF3){
        return;
    }

    uint64 u64_Flag;

    u64_Flag = t_Battery_Msg.au8_Data[7];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[6];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[5];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[4];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[3];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[2];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[1];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[0];

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

    st_Battery.un_Faults_2.all = u64_Flag;

    x_os_mutex_release(&t_Mutex_Battery);
}
/*void Battery_RX_Temperature(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1886EFF3){
        return;
    }
    uint16 u16_Frame_Num;
    sint8 s8_Cell_Temperature_N1_C;
    sint8 s8_Cell_Temperature_N2_C;
    sint8 s8_Cell_Temperature_N3_C;
    sint8 s8_Cell_Temperature_N4_C;
    sint8 s8_Cell_Temperature_N5_C;
    sint8 s8_Cell_Temperature_N6_C;

    u16_Frame_Num = t_Battery_Msg.au8_Data[1];
    u16_Frame_Num = u16_Frame_Num <<8;
    u16_Frame_Num = u16_Frame_Num + t_Battery_Msg.au8_Data[0];

    s8_Cell_Temperature_N1_C = t_Battery_Msg.au8_Data[2] - 40;
    s8_Cell_Temperature_N2_C = t_Battery_Msg.au8_Data[3] - 40;
    s8_Cell_Temperature_N3_C = t_Battery_Msg.au8_Data[4] - 40;
    s8_Cell_Temperature_N4_C = t_Battery_Msg.au8_Data[5] - 40;
    s8_Cell_Temperature_N5_C = t_Battery_Msg.au8_Data[6] - 40;
    s8_Cell_Temperature_N6_C = t_Battery_Msg.au8_Data[7] - 40;

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

    st_Battery. = s8_Max_Temp;
    st_Battery.st_State_4.s8_Min_Temperature_C = s8_Min_Temp;
    st_Battery.st_State_4.s8_Avg_Temperature_C = s8_Avg_Temp;
    st_Battery.st_State_4.u8_Max_Temperature_Cell_Number = u8_Max_Temp_Num;
    st_Battery.st_State_4.u8_Max_Temperature_Position = u8_Max_Temp_Pos;
    st_Battery.st_State_4.u8_Min_Temperature_Cell_Number = u8_Min_Temp_Num;
    st_Battery.st_State_4.u8_Min_Temperature_Position = u8_Min_Temp_Pos;

    x_os_mutex_release(&t_Mutex_Battery);
}*/

void Battery_RX_Cell_Voltage(void);

void Battery_RX_Sate_1(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1883EFF3){
        return;
    }

    uint64 u64_Flag;

    u64_Flag = t_Battery_Msg.au8_Data[7];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[6];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[5];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[4];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[3];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[2];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[1];
    u64_Flag = u64_Flag <<8;
    u64_Flag += t_Battery_Msg.au8_Data[0];

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

    st_Battery.un_State_1.all = u64_Flag;

    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_RX_Sate_2(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1884EFF3){
        return;
    }
    uint8 u8_SOC;
    uint8 u8_SOH;
    uint16 u16_Current_A;
    sint16 s16_Current_A;
    uint16 u16_Voltage_V;
    uint8 u8_Fault_Code;
    uint8 u8_Flag;

    u8_SOC = t_Battery_Msg.au8_Data[0] * 0.4f;
    u8_SOH = t_Battery_Msg.au8_Data[1] * 0.4f;

    u16_Current_A = t_Battery_Msg.au8_Data[3];
    u16_Current_A = u16_Current_A << 8;
    u16_Current_A = u16_Current_A + t_Battery_Msg.au8_Data[2];
    s16_Current_A = u16_Current_A * 0.1f;
    s16_Current_A = s16_Current_A - 1000;

    u16_Voltage_V = t_Battery_Msg.au8_Data[5];
    u16_Voltage_V = u16_Voltage_V << 8;
    u16_Voltage_V = u16_Voltage_V + t_Battery_Msg.au8_Data[4];
    u16_Voltage_V = u16_Voltage_V * 0.1f;

    u8_Fault_Code = t_Battery_Msg.au8_Data[6];
    u8_Flag = t_Battery_Msg.au8_Data[7];

    if(st_Battery.st_State_2.un_Flags.bits.b4_Life != ((u8_Flag & 0xF0)>>4)){
        Battery_Reset_CAN_Counter();
    }

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

        st_Battery.st_State_2.u8_SOC = u8_SOC;
        st_Battery.st_State_2.u8_SOH = u8_SOH;
        st_Battery.st_State_2.s16_Pack_Current_A = s16_Current_A;
        st_Battery.st_State_2.u16_Pack_Inside_Voltage_V = u16_Voltage_V;
        st_Battery.st_State_2.u8_Fault_Code = u8_Fault_Code;
        st_Battery.st_State_2.un_Flags.all = u8_Flag;

    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_RX_Sate_3(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1885EFF3){
        return;
    }

    uint16 u16_Positive_Resistance;
    uint16 u16_Negative_Resistance;
    uint16 u16_System_Resistance;
    uint8 u8_Inuslation_Status;


    u16_Positive_Resistance = t_Battery_Msg.au8_Data[1];
    u16_Positive_Resistance = u16_Positive_Resistance << 8;
    u16_Positive_Resistance = u16_Positive_Resistance + t_Battery_Msg.au8_Data[0];

    u16_Negative_Resistance = t_Battery_Msg.au8_Data[3];
    u16_Negative_Resistance = u16_Negative_Resistance << 8;
    u16_Negative_Resistance = u16_Negative_Resistance + t_Battery_Msg.au8_Data[2];

    u16_System_Resistance = t_Battery_Msg.au8_Data[5];
    u16_System_Resistance = u16_System_Resistance << 8;
    u16_System_Resistance = u16_System_Resistance + t_Battery_Msg.au8_Data[4];

    u8_Inuslation_Status = t_Battery_Msg.au8_Data[6];


    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

        st_Battery.st_State_3.u16_Positive_Insulation_Resistance_kohm = u16_Positive_Resistance;
        st_Battery.st_State_3.u16_Negative_Insulation_Resistance_kohm = u16_Negative_Resistance;
        st_Battery.st_State_3.u16_System_Insulation_Resistance_kohm = u16_System_Resistance;
        st_Battery.st_State_3.u8_Insolation_Monitor_Status = u8_Inuslation_Status;

    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_RX_Sate_4(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1886EFF3){
        return;
    }

    sint8 s8_Max_Temp;
    sint8 s8_Min_Temp;
    sint8 s8_Avg_Temp;
    uint8 u8_Max_Temp_Num;
    uint8 u8_Max_Temp_Pos;
    uint8 u8_Min_Temp_Num;
    uint8 u8_Min_Temp_Pos;

    s8_Max_Temp = t_Battery_Msg.au8_Data[0] - 40;
    s8_Min_Temp = t_Battery_Msg.au8_Data[1] - 40;
    s8_Avg_Temp = t_Battery_Msg.au8_Data[2] - 40;
    u8_Max_Temp_Num = t_Battery_Msg.au8_Data[3];
    u8_Max_Temp_Pos = t_Battery_Msg.au8_Data[4];
    u8_Min_Temp_Num = t_Battery_Msg.au8_Data[5];
    u8_Min_Temp_Pos = t_Battery_Msg.au8_Data[6];

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

        st_Battery.st_State_4.s8_Max_Temperature_C = s8_Max_Temp;
        st_Battery.st_State_4.s8_Min_Temperature_C = s8_Min_Temp;
        st_Battery.st_State_4.s8_Avg_Temperature_C = s8_Avg_Temp;
        st_Battery.st_State_4.u8_Max_Temperature_Cell_Number = u8_Max_Temp_Num;
        st_Battery.st_State_4.u8_Max_Temperature_Position = u8_Max_Temp_Pos;
        st_Battery.st_State_4.u8_Min_Temperature_Cell_Number = u8_Min_Temp_Num;
        st_Battery.st_State_4.u8_Min_Temperature_Position = u8_Min_Temp_Pos;

    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_RX_Sate_5(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1887EFF3){
        return;
    }

    uint16 u16_Max_Ucel;
    uint16 u16_Min_Ucel;
    uint16 u16_Avg_Ucel;

    float32 f32_Max_Ucel;
    float32 f32_Min_Ucel;
    float32 f32_Avg_Ucel;

    u16_Max_Ucel = t_Battery_Msg.au8_Data[1];
    u16_Max_Ucel = u16_Max_Ucel << 8;
    f32_Max_Ucel = u16_Max_Ucel + t_Battery_Msg.au8_Data[0];
    f32_Max_Ucel = f32_Max_Ucel * 0.001f;

    u16_Min_Ucel = t_Battery_Msg.au8_Data[3];
    u16_Min_Ucel = u16_Min_Ucel << 8;
    f32_Min_Ucel = u16_Min_Ucel + t_Battery_Msg.au8_Data[2];
    f32_Min_Ucel = f32_Min_Ucel * 0.001f;

    u16_Avg_Ucel = t_Battery_Msg.au8_Data[5];
    u16_Avg_Ucel = u16_Avg_Ucel << 8;
    f32_Avg_Ucel = u16_Avg_Ucel + t_Battery_Msg.au8_Data[4];
    f32_Avg_Ucel = f32_Avg_Ucel * 0.001f;


    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

        st_Battery.st_State_5.f32_Max_Cell_Voltage_V = f32_Max_Ucel;
        st_Battery.st_State_5.f32_Min_Cell_Voltage_V = f32_Min_Ucel;
        st_Battery.st_State_5.f32_Avg_Cell_Voltage_V = f32_Avg_Ucel;

    x_os_mutex_release(&t_Mutex_Battery);

}

void Battery_RX_Sate_6(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1888EFF3){
        return;
    }

    uint8 u8_Max_Ucel_Num;
    uint16 u16_Max_Ucel_Pos;
    uint8 u8_Min_Ucel_Num;
    uint16 u16_Min_Ucel_Pos;

    u8_Max_Ucel_Num = t_Battery_Msg.au8_Data[0];

    u16_Max_Ucel_Pos = t_Battery_Msg.au8_Data[2];
    u16_Max_Ucel_Pos = u16_Max_Ucel_Pos << 8;
    u16_Max_Ucel_Pos = u16_Max_Ucel_Pos + t_Battery_Msg.au8_Data[1];

    u8_Min_Ucel_Num = t_Battery_Msg.au8_Data[3];

    u16_Min_Ucel_Pos = t_Battery_Msg.au8_Data[5];
    u16_Min_Ucel_Pos = u16_Min_Ucel_Pos << 8;
    u16_Min_Ucel_Pos = u16_Min_Ucel_Pos + t_Battery_Msg.au8_Data[4];

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }
        st_Battery.st_State_6.u8_Max_Voltage_Cell_Number = u8_Max_Ucel_Num;
        st_Battery.st_State_6.u16_Max_Voltage_Cell_Position = u16_Max_Ucel_Pos;
        st_Battery.st_State_6.u8_Min_Voltage_Cell_Number = u8_Min_Ucel_Num;
        st_Battery.st_State_6.u16_Min_Voltage_Cell_Position = u16_Min_Ucel_Pos;

    x_os_mutex_release(&t_Mutex_Battery);
}
void Battery_RX_Sate_7(void){
    if(t_Battery_Msg.t_Id.u32_Id != 0x1889EFF3){
        return;
    }

    sint8 s8_Gun_1DC_Pos_Temp;
    sint8 s8_Gun_1DC_Neg_Temp;
    sint8 s8_Gun_2DC_Pos_Temp;
    sint8 s8_Gun_2DC_Neg_Temp;
    sint8 s8_Gun_AC_Pos_Temp;
    sint8 s8_Gun_AC_Neg_Temp;

    s8_Gun_1DC_Pos_Temp = t_Battery_Msg.au8_Data[0] - 40;
    s8_Gun_1DC_Neg_Temp = t_Battery_Msg.au8_Data[1] - 40;
    s8_Gun_2DC_Pos_Temp = t_Battery_Msg.au8_Data[2] - 40;
    s8_Gun_2DC_Neg_Temp = t_Battery_Msg.au8_Data[3] - 40;
    s8_Gun_AC_Pos_Temp = t_Battery_Msg.au8_Data[4] - 40;
    s8_Gun_AC_Neg_Temp = t_Battery_Msg.au8_Data[5] - 40;

    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
        return;
    }

        st_Battery.st_State_7.s8_Gun_1_DC_Positive_Temperature_C = s8_Gun_1DC_Pos_Temp;
        st_Battery.st_State_7.s8_Gun_1_DC_Negitive_Temperature_C = s8_Gun_1DC_Neg_Temp;
        st_Battery.st_State_7.s8_Gun_2_DC_Positive_Temperature_C = s8_Gun_2DC_Pos_Temp;
        st_Battery.st_State_7.s8_Gun_2_DC_Negitive_Temperature_C = s8_Gun_2DC_Neg_Temp;
        st_Battery.st_State_7.s8_Gun_AC_Positive_Temperature_C = s8_Gun_AC_Pos_Temp;
        st_Battery.st_State_7.s8_Gun_AC_Negitive_Temperature_C = s8_Gun_AC_Neg_Temp;


    x_os_mutex_release(&t_Mutex_Battery);
}

uint8 Battery_Recived(void){
    sint16 s16_Error;
    sint16 s16_Error_Can;
    uint8 u8_Recived;
    u8_Recived = FALSE;

    Battery_Check_CAN();

    s16_Error = x_os_mutex_get(&t_Mutex_Battery_Msg,10);
    if(s16_Error != C_NO_ERR){
        Error_Beep();
        return u8_Recived;
    }
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Battery, &t_Battery_Msg);
    while(s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR){
        u8_Recived = TRUE;
        switch(t_Battery_Msg.t_Id.u32_Id){
            case 0x18C4EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Current_Limit();
                break;
            case 0x1883EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_1();
                break;
            case 0x1884EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_2();
                break;
            case 0x1885EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_3();
                break;
            case 0x1886EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_4();
                break;
            case 0x1887EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_5();
                break;
            case 0x1888EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_6();
                break;
            case 0x1889EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Sate_7();
                break;
            case 0x1881EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Fault_1();
                break;
            case 0x1882EFF3:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
                Battery_RX_Fault_2();
                break;
            default:
                x_os_mutex_release(&t_Mutex_Battery_Msg);
        }
        s16_Error = x_os_mutex_get(&t_Mutex_Battery_Msg,10);    //Trato de obtener el mutex denuevo
        if(s16_Error != C_NO_ERR){
            break;                                              //No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Battery, &t_Battery_Msg);
    }
    x_os_mutex_release(&t_Mutex_Battery_Msg);

    return u8_Recived;

}

void Battery_Send_Msg(void){
    st_Battery_Counters.u8_Fast_Counter++;
    if(st_Battery_Counters.u8_Fast_Counter >= BATTERY_FAST_COUNTER_LIMIT){
        st_Battery_Counters.u8_Fast_Counter = 0;
        Battery_Update_Life_Signal();
        Battery_TX_Command();
    }

    st_Battery_Counters.u8_Normal_Counter++;
    if(st_Battery_Counters.u8_Normal_Counter >= BATTERY_NORMAL_COUNTER_LIMIT){
        st_Battery_Counters.u8_Normal_Counter = 0;
        Battery_TX_Isometer();
    }

    st_Battery_Counters.u8_Slow_Counter++;
    if(st_Battery_Counters.u8_Slow_Counter >= BATTERY_SLOW_COUNTER_LIMIT){
        st_Battery_Counters.u8_Slow_Counter = 0;
        Battery_TX_VIN();
    }
}




