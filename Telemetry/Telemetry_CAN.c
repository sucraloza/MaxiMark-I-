#include "OS_Defs.h"

void Telemetry_Errors(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x10008819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = SCU_get_DCU_Error() & 0x3;
    t_Msg.au8_Data[0] |= ( SCU_get_PSU_Error() & 0x3 ) << 2;

    t_Msg.au8_Data[1] = Inverter_Get_Error_Flags();
    t_Msg.au8_Data[2] = (uint8)(Inverter_Get_Error_Code() & 0x00FF);
    t_Msg.au8_Data[3] = (uint8)((Inverter_Get_Error_Code() & 0xFF00) >> 8) ;

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}

void Telemetry_Battery_Errors_1(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x10018819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    uint32 u32_Error_Flags_1;
    u32_Error_Flags_1 = Battery_get_Error_Flags_1();

    t_Msg.au8_Data[0] = (uint8)(u32_Error_Flags_1 & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((u32_Error_Flags_1 >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((u32_Error_Flags_1 >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((u32_Error_Flags_1 >> 24) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}

void Telemetry_Battery_Errors_2(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x10028819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint64 u32_Error_Flags_2;
    u32_Error_Flags_2 = Battery_get_Error_Flags_2();

    t_Msg.au8_Data[0] = (uint8)(u32_Error_Flags_2 & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((u32_Error_Flags_2 >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((u32_Error_Flags_2 >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((u32_Error_Flags_2 >> 24) & 0xFF);
    t_Msg.au8_Data[4] = (uint8)((u32_Error_Flags_2 >> 32) & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u32_Error_Flags_2 >> 40) & 0xFF);
    t_Msg.au8_Data[6] = (uint8)((u32_Error_Flags_2 >> 48) & 0xFF);
    t_Msg.au8_Data[7] = (uint8)((u32_Error_Flags_2 >> 56) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}


void Telemetry_Equipment_Status(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C038819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = st_Telemetry.st_Tracking.un_Flags.all;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    if(x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg) == C_NO_ERR)
        st_Telemetry.u8_Tracking_Triggered = FALSE;

}

void Telemetry_System_Status(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C048819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint8 u8_Auxiliary_Voltage_V;
    uint8 u8_Charging_Current_A;
    uint8 u8_AInv_Temperature_C;
    u8_Auxiliary_Voltage_V = Inverter_Get_Auxiliary_Voltage_V() * 5;
    u8_Charging_Current_A = SCU_get_PSU_Filtered_Current_A();
    u8_AInv_Temperature_C =  (SCU_get_PSU_Temperature_C() + 40);

    t_Msg.au8_Data[0] = st_Telemetry.st_Tracking.u8_ECU_State;
    t_Msg.au8_Data[1] = st_Telemetry.st_Tracking.u8_Key_State;
    t_Msg.au8_Data[2] = u8_Auxiliary_Voltage_V;
    t_Msg.au8_Data[3] = u8_Charging_Current_A;
    t_Msg.au8_Data[4] = u8_AInv_Temperature_C;

    t_Msg.au8_Data[5] = Jarvis_get_Button_Hand_Break();
    t_Msg.au8_Data[5] |= (Jarvis_get_Button_Pedal_Break() << 1);
    t_Msg.au8_Data[5] |= (Jarvis_get_Button_Regen() << 2);
    t_Msg.au8_Data[5] |= (Gear_get_State() & 0x7) << 5;

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}

void Telemetry_Power_OnOff(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C058819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = System_get_Power_On_State();
    t_Msg.au8_Data[1] = System_get_Power_Off_State();
    t_Msg.au8_Data[2] = PDU_get_State();

    t_Msg.au8_Data[3] = GPIO_get_PDU_Precharge();
    t_Msg.au8_Data[3] |= GPIO_get_PDU_Contactor_Main() << 2;
    t_Msg.au8_Data[3] |= GPIO_get_PDU_Discharge() << 4;

    t_Msg.au8_Data[4] = (uint8) (Inverter_Get_Power_Voltage_V() & 0x00FF);
    t_Msg.au8_Data[5] = (uint8) ((Inverter_Get_Power_Voltage_V() & 0xFF00) >> 8);

    t_Msg.au8_Data[6] = (uint8) (Battery_get_Pack_Voltage_V() & 0x00FF);
    t_Msg.au8_Data[7] = (uint8) ((Battery_get_Pack_Voltage_V() & 0xFF00) >> 8);

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}

void Telemetry_Isolation(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C068819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8) (Battery_get_Isolation_kohm() & 0x00FF);
    t_Msg.au8_Data[1] = (uint8) ((Battery_get_Isolation_kohm() & 0xFF00) >> 8);

    t_Msg.au8_Data[2] = (uint8) (Battery_get_Isolation_Pos_kohm() & 0x00FF);
    t_Msg.au8_Data[3] = (uint8) ((Battery_get_Isolation_Pos_kohm() & 0xFF00) >> 8);

    t_Msg.au8_Data[4] = (uint8) (Battery_get_Isolation_Neg_kohm() & 0x00FF);
    t_Msg.au8_Data[5] = (uint8) ((Battery_get_Isolation_Neg_kohm() & 0xFF00) >> 8);

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;


    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}

void Telemetry_Electrical(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C078819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Pack_Voltage;
    uint8 u8_Aux_Charging_Current_A;
    u16_Pack_Voltage = Battery_get_Pack_Voltage_V();
    u8_Aux_Charging_Current_A = SCU_get_PSU_Filtered_Current_A();

    t_Msg.au8_Data[0] = Battery_get_SOC();
    t_Msg.au8_Data[1] = 0;

    t_Msg.au8_Data[2] = u8_Aux_Charging_Current_A;
    t_Msg.au8_Data[3] = 0;

    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;

    t_Msg.au8_Data[6] = (uint8) (u16_Pack_Voltage & 0x00FF);
    t_Msg.au8_Data[7] = (uint8) ((u16_Pack_Voltage & 0xFF00) >> 8);

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);

}

void Telemetry_Drive_Fast(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C088819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Pressure_mbar;
    uint16 u16_Avg_Torque_nm;
    uint16 u16_Avg_Current_A;
    uint8 u8_Avg_Speed_kmh;

    u16_Pressure_mbar = Jarvis_get_Pressure_mBar();
    u16_Avg_Torque_nm = Inverter_Get_Avg_Torque_nm() + 3000;
    u16_Avg_Current_A = Inverter_Get_Avg_Current_A() + 1000;
    u8_Avg_Speed_kmh =(uint8) (Inverter_Get_Avg_Speed_kmh() + 120);

    t_Msg.au8_Data[0] = (uint8)(u16_Pressure_mbar & 0x00FF);
    t_Msg.au8_Data[1] = (uint8)((u16_Pressure_mbar >> 8) & 0x00FF);

    t_Msg.au8_Data[2] = (uint8)(u16_Avg_Torque_nm & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((u16_Avg_Torque_nm >> 8) & 0xFF);

    t_Msg.au8_Data[4] = (uint8)(u16_Avg_Current_A & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u16_Avg_Current_A >> 8) & 0xFF);

    t_Msg.au8_Data[6] = u8_Avg_Speed_kmh;
    t_Msg.au8_Data[7] = Inverter_Get_Derrating();


    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);
}


void Telemetry_Drive_Slow(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C098819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Max_Pos_Torque_nm;
    uint16 u16_Max_Neg_Torque_nm;
    uint8 u8_Max_Speed_kmh;

    u16_Max_Pos_Torque_nm = Inverter_Get_Max_Torque_nm() + 3000;
    u16_Max_Neg_Torque_nm = Inverter_Get_Min_Torque_nm() + 3000;
    u8_Max_Speed_kmh = Inverter_Get_Max_Speed_kmh() + 120;

    t_Msg.au8_Data[0] = (uint8)(u16_Max_Pos_Torque_nm & 0x00FF);
    t_Msg.au8_Data[1] = (uint8)((u16_Max_Pos_Torque_nm >> 8) & 0x00FF);

    t_Msg.au8_Data[2] = (uint8)(u16_Max_Neg_Torque_nm & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((u16_Max_Neg_Torque_nm >> 8) & 0xFF);

    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;

    t_Msg.au8_Data[6] = u8_Max_Speed_kmh;
    t_Msg.au8_Data[7] = 0;

    Inverter_Reset_Max_Torque_nm();
    Inverter_Reset_Min_Torque_nm();
    Inverter_Reset_Max_Speed_kmh();

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);
}

void Telemetry_Odometer(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C0A8819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    float32 f32_Odometer;
    sint16 s16_error = Odometer_Get_Km(&f32_Odometer);
    if (s16_error != C_NO_ERR) return;

    uint32 u32_Odometer_100m = (uint32) (10 * f32_Odometer);


    t_Msg.au8_Data[0] = (uint8)(u32_Odometer_100m & 0x00FF);
    t_Msg.au8_Data[1] = (uint8)((u32_Odometer_100m >> 8) & 0x00FF);

    t_Msg.au8_Data[2] = (uint8)((u32_Odometer_100m >> 16) & 0x00FF);
    t_Msg.au8_Data[3] = (uint8)((u32_Odometer_100m >> 24) & 0xFF);

    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;

    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);
}

void Telemetry_HVIL(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C0B8819;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = HVIL_get_State();
    t_Msg.au8_Data[1] = HVIL_get_ref_GPIO();
    t_Msg.au8_Data[2] = HVIL_get_Feedback();
    t_Msg.au8_Data[3] = GPIO_get_HVIL_Feedback();

    x_can_obj_send_msg(mpv_MsgObjTx_Telemetry, &t_Msg);
}

void Telemetry_Fusi(void){
    uint16 u16_Current_DTC;
    u16_Current_DTC = Fusi_get_DTC();
    if(st_Telemetry.u16_Preveous_DTC != u16_Current_DTC){
        st_Telemetry.u16_Preveous_DTC = u16_Current_DTC;
        Fusi_TX_Flags_Acctions(4);
        return;
    }

    if(u16_Current_DTC){
        if(st_Telemetry_Counters.u8_Fusi_Counter == 0){
            switch(st_Telemetry.u8_Fusi_Message_Turn){
                case 0:
                    Telemetry_Errors();
                    break;
                case 1:
                    Telemetry_Battery_Errors_1();
                    break;
                case 2:
                    Telemetry_Battery_Errors_2();
                    break;
                case 3:
                    Fusi_TX_Flags_System(4);
                    break;
                case 4:
                    Fusi_TX_Flags_Battery(4);
                    break;
                case 5:
                    Fusi_TX_Flags_PDU(4);
                    break;
                case 6:
                    Fusi_TX_Flags_Motor(4);
                    break;
                case 7:
                    Fusi_TX_Flags_Steering(4);
                    break;
                case 8:
                    Fusi_TX_Flags_AuxInverter(4);
                    break;
                case 9:
                    Fusi_TX_Flags_Air(4);
                    break;
                case 10:
                    Fusi_TX_Flags_Heating(4);
                    break;
                case 11:
                    Fusi_TX_Flags_Acctions(4);
                    break;

            }
           st_Telemetry.u8_Fusi_Message_Turn++;
            if(st_Telemetry.u8_Fusi_Message_Turn > 11){
                st_Telemetry.u8_Fusi_Message_Turn = 0;
            }
        }
        st_Telemetry_Counters.u8_Fusi_Counter++;
        if(st_Telemetry_Counters.u8_Fusi_Counter >= TELEMETRY_FUSI_CYCLE){
            st_Telemetry_Counters.u8_Fusi_Counter = 0;
        }
    }
    else{
        st_Telemetry.u8_Fusi_Message_Turn = 0;
    }
}

void Telemetry_5min(void){
    uint8 u8_Number_Off_Messages;
    u8_Number_Off_Messages = 5;

    if(st_Telemetry_Counters.u16_5min_Counter < u8_Number_Off_Messages){
        switch(st_Telemetry_Counters.u16_5min_Counter){
            case 0:
                if(System_get_State() != ECU_STATE_SLEEP){
                    Telemetry_Isolation();
                }
                break;
            case 1:
                if(System_get_State() != ECU_STATE_SLEEP){
                    Telemetry_Electrical();
                }
                break;
            case 2:
                if(System_get_State() == ECU_STATE_DRIVE){
                    Telemetry_Drive_Slow();
                }
                break;
            case 3:
                    Telemetry_System_Status();
                break;
            case 4:
                if(System_get_State() != ECU_STATE_SLEEP){
                    Telemetry_HVIL();
                }
                break;

        }
    }
    st_Telemetry_Counters.u16_5min_Counter++;
    if(st_Telemetry_Counters.u16_5min_Counter >= TELEMETRY_5MIN_COUNTER_LIMIT){
        st_Telemetry_Counters.u16_5min_Counter = 0;
    }

}

void Telemetry_30sec(void){
    uint8 u8_Number_Off_Messages;
    u8_Number_Off_Messages = 2;

    if(st_Telemetry_Counters.u16_30sec_Counter < u8_Number_Off_Messages){
        switch(st_Telemetry_Counters.u16_30sec_Counter){
            case 0:
                if(System_get_State() == ECU_STATE_DRIVE){
                    Telemetry_Drive_Fast();
                }
                break;
            case 1:
                Fusi_TX_DTC_History(4);
                break;
        }
    }

    st_Telemetry_Counters.u16_30sec_Counter++;
    if(st_Telemetry_Counters.u16_30sec_Counter > TELEMETRY_30SEC_COUNTER_LIMIT){
        st_Telemetry_Counters.u16_30sec_Counter = 0;
    }
}


void Telemetry_Send(void){
    if(st_Telemetry.u8_Tracking_Triggered){
        Telemetry_Equipment_Status();
        Telemetry_HVIL();
    }

    uint8 u8_ECU_State;
    u8_ECU_State = System_get_State();

    if((u8_ECU_State ==  ECU_STATE_POWER_ON) || (u8_ECU_State == ECU_STATE_POWER_OFF)){
        Telemetry_Power_OnOff();
    }
    Telemetry_Fusi();
    Telemetry_5min();
    Telemetry_30sec();

}
