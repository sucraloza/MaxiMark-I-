#include "OS_Defs.h"

void Battery_Machine_State(void){
}

void Battery_Update_Life_Signal(void){
    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
       return;
    }
    uint8 u8_Life_Signal = st_Battery.un_Command.bits.b8_Life_Signal;
    u8_Life_Signal++;
    st_Battery.un_Command.bits.b8_Life_Signal = u8_Life_Signal;
    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_Set_Power_On(void){
    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
       return;
    }
    st_Battery.un_Command.bits.b2_HV_On_Off = 1;
    x_os_mutex_release(&t_Mutex_Battery);
}

void Battery_Set_Power_Off(void){
    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
       return;
    }
    st_Battery.un_Command.bits.b2_HV_On_Off = 2;
    x_os_mutex_release(&t_Mutex_Battery);
}

void Baterry_Set_Pos_Contator(uint8 u8_status){
    /*
    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
       return;
    }
    if(u8_status < 4)
    {
        st_Battery.un_Command.bits.b2_Main_Positive_Status = u8_status;
    }
    else{
        st_Battery.un_Command.bits.b2_Main_Positive_Status = BATTERY_RELAY_STATE_INVALID;
    }
    x_os_mutex_release(&t_Mutex_Battery);
    */
}

void Baterry_Set_Aux_Contator(uint8 u8_status){
    /*
    if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
       return;
    }
    if(u8_status < 4)
    {
        st_Battery.un_Command.bits.b2_Aux_1_Status = u8_status;
    }
    else{
        st_Battery.un_Command.bits.b2_Aux_1_Status = BATTERY_RELAY_STATE_INVALID;
    }
    x_os_mutex_release(&t_Mutex_Battery);
    */
}

void Baterry_Set_Speed_RPM(sint16 s16_speed_RPM){

if(x_os_mutex_get(&t_Mutex_Battery,10) != C_NO_ERR){
       return;
    }
    uint8 u8_speed_kmh = 0;
    if(s16_speed_RPM >= 0){
        u8_speed_kmh = SPEED_FACTOR * s16_speed_RPM;
    }else{
        u8_speed_kmh = - SPEED_FACTOR * s16_speed_RPM;
    }
    st_Battery.un_Command.bits.b8_VehicleSpeed_kmh = u8_speed_kmh;
    x_os_mutex_release(&t_Mutex_Battery);
}

uint8 Battery_Is_Ready(void){
    return (st_Battery.un_State_1.bits.b2_BMS_HV_Status == 2);
}

uint8 Battery_Is_Open(void){
    return (st_Battery.un_State_1.bits.b2_BMS_HV_Status == 0);
}

uint8 Battery_get_Emergency_Off(void);

//Positive values are discharge and negative charge
sint16 Battery_get_Pack_Current(void){
    return st_Battery.st_State_2.s16_Pack_Current_A;
}

uint16 Battery_get_Pack_Voltage_V(void){
    return st_Battery.st_State_2.u16_Pack_Inside_Voltage_V;
}

uint16 Battery_get_ImpulseDischarge_Current(void){
    return st_Battery.st_Current_Limits.u16_Avail_Inpulse_Discharge_Current_A;
}

uint16 Battery_get_ImpulseCharge_Current(void){
    return st_Battery.st_Current_Limits.u16_Avail_Inpulse_Charge_Current_A;
}

uint16 Battery_get_Charge_Current(void){
    return st_Battery.st_Current_Limits.u16_Avail_Sustain_Charge_Current_A;
}


uint8 Battery_get_SOC(void){
    return st_Battery.st_State_2.u8_SOC;
}

sint8 Battery_get_Temperature(void){
    return st_Battery.st_State_4.s8_Avg_Temperature_C;
}

uint8 Battery_get_Gun_Connection(void){
    return (st_Battery.un_State_1.bits.b2_DC_Charge_Connected_Status == 1); //Single Gun Connected
}

uint8 Battery_get_Not_Charging(void){
    return (st_Battery.un_State_1.bits.b2_Charge_Status == BATTERY_CHR_NOT_CHARGING);
}
uint8 Battery_get_Charging(void){
    return (st_Battery.un_State_1.bits.b2_Charge_Status == BATTERY_CHR_CHARGING);
}
uint8 Battery_get_Charge_Finished(void){
    return (st_Battery.un_State_1.bits.b2_Charge_Status == BATTERY_CHR_CHARGE_FINISH);
}
uint8 Battery_get_Charging_Error(void){
    return (st_Battery.un_State_1.bits.b2_Charge_Status == BATTERY_CHR_CHARGE_ERROR);
}

uint8 Battery_get_Heating(void){
    return st_Battery.un_State_1.bits.b1_Heating_Status;
}

uint8 Battery_get_Cooling(void){
    return (st_Battery.un_State_1.bits.b2_Cooling_Status == TRUE);
}

uint8 Battery_get_Failure(void){
    uint8 res;
    res = FALSE;
    if(st_Battery.un_Faults_1.bits.b1_Baterry_System_Not_Mach)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b1_HVIL_Alarm)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Cell_Over_Voltage == 2)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Over_Charge == 2)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Over_SOC == 2)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Over_Temperature == 2)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Pack_Over_Voltage == 2)
        res = TRUE;

    if(st_Battery.un_Faults_2.bits.b1_Charge_NTC_Error)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Current_Sensor_Error)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Heating_Film_Error)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Over_Discharge_Current_While_Charging)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b2_Over_Discharge_Current)
        res = TRUE;

    return res;
}

uint8 Battery_get_Catastrofic_Failure(void){
    uint8 res;
    res = FALSE;
    if(st_Battery.un_Faults_1.bits.b2_Cell_Over_Voltage == 3)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Over_Charge == 3)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Over_SOC == 3)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Over_Temperature == 3)
        res = TRUE;
    if(st_Battery.un_Faults_1.bits.b2_Pack_Over_Voltage == 3)
        res = TRUE;

    if(st_Battery.un_Faults_2.bits.b1_Cell_Over_Temperature)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Fire_Detection)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Heat_Relay_Weld)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Inter_Comunication_Error)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Over_Charge_Current_While_Charging)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Pack_Fire_Warning)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Pack_Self_Protect)
        res = TRUE;
    if(st_Battery.un_Faults_2.bits.b1_Power_Supply_Error)
        res = TRUE;

    return res;
}

uint16 Battery_get_Isolation_kohm(void){
    uint16 u16_Isolation_kohm;
    u16_Isolation_kohm = st_Battery.st_State_3.u16_System_Insulation_Resistance_kohm;
    if(u16_Isolation_kohm > st_Battery.st_State_3.u16_Positive_Insulation_Resistance_kohm)
        u16_Isolation_kohm = st_Battery.st_State_3.u16_Positive_Insulation_Resistance_kohm;
    if(u16_Isolation_kohm > st_Battery.st_State_3.u16_Negative_Insulation_Resistance_kohm)
        u16_Isolation_kohm = st_Battery.st_State_3.u16_Negative_Insulation_Resistance_kohm;
    return u16_Isolation_kohm;
    //return st_Battery.st_State_3.u16_System_Insulation_Resistance_kohm;
}

uint16 Battery_get_Isolation_Pos_kohm(void){
    return st_Battery.st_State_3.u16_Positive_Insulation_Resistance_kohm;
}
uint16 Battery_get_Isolation_Neg_kohm(void){
    return st_Battery.st_State_3.u16_Negative_Insulation_Resistance_kohm;
}

uint8 Battery_get_BTMS_Error(void){
    return st_Battery.un_Faults_2.bits.b1_BTMS_Error;
}
uint32 Battery_get_Error_Flags_1(void){
    return st_Battery.un_Faults_1.all;
}

uint64 Battery_get_Error_Flags_2(void){
    return st_Battery.un_Faults_2.all;
}

uint8 Battery_get_BMS_HV_State(void){
    return st_Battery.un_State_1.bits.b2_BMS_HV_Status;
}

void Battery_Check_CAN(void){
    if(x_os_mutex_get(&t_Mutex_Battery,10) == C_NO_ERR){
        st_Battery.st_CAN.u8_CAN_Timeout_Counter++;
        if(st_Battery.st_CAN.u8_CAN_Timeout_Counter > BATTERY_CAN_TIMEOUT){
            st_Battery.st_CAN.u8_CAN_Timeout_Counter = BATTERY_CAN_TIMEOUT + 1;
            if(GPIO_get_T15_BCU() == DS_ON){
                Fusi_Set_Battery_CAN(FUSI_ON_FLAG);
            }
            st_Battery.st_CAN.u8_CAN_Timeout_Flag = FUSI_ON_FLAG;
        }
        x_os_mutex_release(&t_Mutex_Battery);
    }
}

void Battery_Reset_CAN_Counter(void){
    if(x_os_mutex_get(&t_Mutex_Battery,10) == C_NO_ERR){
        Fusi_Set_Battery_CAN(FUSI_OFF_FLAG);
        st_Battery.st_CAN.u8_CAN_Timeout_Counter = 0;
        st_Battery.st_CAN.u8_CAN_Timeout_Flag = FUSI_OFF_FLAG;
        x_os_mutex_release(&t_Mutex_Battery);
    }
}

uint8 Battery_Get_CAN_OK(void){
    if(st_Battery.st_CAN.u8_CAN_Timeout_Flag)
        return FALSE;
    else
        return TRUE;
}
