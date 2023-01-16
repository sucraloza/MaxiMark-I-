#include "OS_Defs.h"


void Fusi_Eep_Load(void){
    /*
    sint16 s16_Error;
    uint32 u32_Temp;
    // !!!!RANGO DE MEMORIO FUE OCUPADO PARA EL ODOMETRO!!!!
    while(x_eep_read(1, 4, &u32_Temp) != C_NO_ERR){
        x_sys_delay_us(10u);
    }
    st_Fusi.un_Eeprom.all = u32_Temp;
    Fusi_from_Eeprom_to_Fusi();
    */
}

void Fusi_Eep_Save(void){
    /*
    sint16 s16_Error;
    uint32 u32_Temp;

    Fusi_from_Fusi_to_Eeprom();
    u32_Temp = st_Fusi.un_Eeprom.all;
    // !!!!RANGO DE MEMORIO FUE OCUPADO PARA EL ODOMETRO!!!!
    if(st_Fusi.un_Eeprom.all != 0){
        s16_Error = x_eep_write(1, 4, &u32_Temp);
        s16_Error = x_eep_flush();
    }
    */
    //todo ERROR WRITE
}

void Fusi_from_Eeprom_to_Fusi(void){
    st_Fusi.un_System.bits.b2_CAN_3 = st_Fusi.un_Eeprom.bits.b2_Sys_CAN_3;
    st_Fusi.un_Battery.bits.b2_CAN = st_Fusi.un_Eeprom.bits.b2_Batt_CAN;
    st_Fusi.un_Battery.bits.b2_Failure = st_Fusi.un_Eeprom.bits.b2_Batt_Failure;
    st_Fusi.un_Battery.bits.b2_Catastrofic_Failure = st_Fusi.un_Eeprom.bits.b2_Batt_Catastrofic_Failure;
    st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault = st_Fusi.un_Eeprom.bits.b2_Batt_Isolation_Neg_Fault;
    st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault = st_Fusi.un_Eeprom.bits.b2_Batt_Isolation_Pos_Fault;
    st_Fusi.un_PDU.bits.b2_Low_Voltage = st_Fusi.un_Eeprom.bits.b2_PDU_Low_Voltage;
    st_Fusi.un_PDU.bits.b2_HVIL_Open = st_Fusi.un_Eeprom.bits.b2_PDU_HVIL_Open;
    st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close = st_Fusi.un_Eeprom.bits.b2_PDU_Contactor_Cant_Close;
    st_Fusi.un_PDU.bits.b2_Contactor_Welded = st_Fusi.un_Eeprom.bits.b2_PDU_Contactor_Welded;
}

void Fusi_from_Fusi_to_Eeprom(void){
    st_Fusi.un_Eeprom.bits.b2_Sys_CAN_3 = st_Fusi.un_System.bits.b2_CAN_3;
    st_Fusi.un_Eeprom.bits.b2_Batt_CAN = st_Fusi.un_Battery.bits.b2_CAN;
    st_Fusi.un_Eeprom.bits.b2_Batt_Failure = st_Fusi.un_Battery.bits.b2_Failure;
    st_Fusi.un_Eeprom.bits.b2_Batt_Catastrofic_Failure = st_Fusi.un_Battery.bits.b2_Catastrofic_Failure;
    st_Fusi.un_Eeprom.bits.b2_Batt_Isolation_Neg_Fault = st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault;
    st_Fusi.un_Eeprom.bits.b2_Batt_Isolation_Pos_Fault = st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault;
    st_Fusi.un_Eeprom.bits.b2_PDU_Low_Voltage = st_Fusi.un_PDU.bits.b2_Low_Voltage;
    st_Fusi.un_Eeprom.bits.b2_PDU_HVIL_Open = st_Fusi.un_PDU.bits.b2_HVIL_Open;
    st_Fusi.un_Eeprom.bits.b2_PDU_Contactor_Cant_Close = st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close;
    st_Fusi.un_Eeprom.bits.b2_PDU_Contactor_Welded = st_Fusi.un_PDU.bits.b2_Contactor_Welded;
}

void Fusi_Check_Start(void){

}

void Fusi_Check_DCLink(void){
    if(System_get_DCLink_State() && (Inverter_Get_Power_Voltage_V() < FUSI_DCLINK_MIN_VOLTAGE)){
        st_Fusi.un_PDU.bits.b2_Low_Voltage = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 13){
            Fusi_set_DTC(0x5001u);
            st_Fusi.u8_DTC_Level = 13;
        }
    }
    else{
        st_Fusi.un_PDU.bits.b2_Low_Voltage = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Emergency_Stop(void){
    if(GPIO_get_Emergency()){
        st_Fusi.un_System.bits.b2_Emergency_Stop = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 12){
            Fusi_set_DTC(0x5308u);
            st_Fusi.u8_DTC_Level = 12;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Emergency_Stop = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Isolation(void){
    uint16 u16_Iso_kohm;

    if(GPIO_get_T15_BCU() == DS_OFF){
        st_Fusi_Counters.u16_Battery_Isolation_Start_Counter = 0;
        return;
    }

    if(st_Fusi_Counters.u16_Battery_Isolation_Start_Counter < FUSI_BATTERY_ISO_START_LIMIT_10S){
        st_Fusi_Counters.u16_Battery_Isolation_Start_Counter++;
        return;
    }

    u16_Iso_kohm = Battery_get_Isolation_Pos_kohm();
    if(u16_Iso_kohm < FUSI_ISOLATION_WARNING_KOHM){
        st_Fusi.un_Battery.bits.b2_Isolation_Pos_Warning = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x420Bu);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
    else{
        st_Fusi.un_Battery.bits.b2_Isolation_Pos_Warning = FUSI_OFF_FLAG;
    }
    if(u16_Iso_kohm < FUSI_ISOLATION_FAULT_KOHM){
        st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 3){
            Fusi_set_DTC(0x420Du);
            st_Fusi.u8_DTC_Level = 3;
        }
    }
    else{
        st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault = FUSI_OFF_FLAG;
    }
    u16_Iso_kohm = Battery_get_Isolation_Neg_kohm();
    if(u16_Iso_kohm < FUSI_ISOLATION_WARNING_KOHM){
        st_Fusi.un_Battery.bits.b2_Isolation_Neg_Warning = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x420Au);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
    else{
        st_Fusi.un_Battery.bits.b2_Isolation_Neg_Warning = FUSI_OFF_FLAG;
    }
    if(u16_Iso_kohm < FUSI_ISOLATION_FAULT_KOHM){
        st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 3){
            Fusi_set_DTC(0x420Cu);
            st_Fusi.u8_DTC_Level = 3;
        }
    }
    else{
        st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Battery_Failure(void){
    if(Battery_get_Failure()){
        st_Fusi.un_Battery.bits.b2_Failure = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 11){
            Fusi_set_DTC(0x4201u);
            st_Fusi.u8_DTC_Level = 11;
        }
    }
    else{
       st_Fusi.un_Battery.bits.b2_Failure = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Battery_Catastrofic_Failure(void){
    if(Battery_get_Catastrofic_Failure()){
       st_Fusi.un_Battery.bits.b2_Catastrofic_Failure = FUSI_ON_FLAG;
       if(st_Fusi.u8_DTC_Level <= 13){
            Fusi_set_DTC(0x4202u);
            st_Fusi.u8_DTC_Level = 13;
        }
    }
    else{
       st_Fusi.un_Battery.bits.b2_Catastrofic_Failure = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Battery_High_SOC(void){
    if(Battery_get_SOC() > 94){
       st_Fusi.un_Battery.bits.b2_Over_SOC_Warning = FUSI_ON_FLAG;
       if(st_Fusi.u8_DTC_Level <= 4){
            Fusi_set_DTC(0x4207u);
            st_Fusi.u8_DTC_Level = 4;
        }
    }
    else{
       st_Fusi.un_Battery.bits.b2_Over_SOC_Warning = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_CAN_3(void);
void Fusi_Check_CAN_1(void);

void Fusi_Check_Motor_Failure(void){
    if(Inverter_Get_EmergencyStop() || Inverter_Get_Operational_State() == INV_STATE_FAILURE){
        st_Fusi.un_Motor.bits.b2_Failure = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 10){
            Fusi_set_DTC(0x4D01u);
            st_Fusi.u8_DTC_Level = 10;
        }
    }
    else{
       st_Fusi.un_Motor.bits.b2_Failure = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Motor_CAN(void);
void Fusi_Check_CAN_2(void);

void Fusi_HVIL(void){
    sint16 s16_Speed_rpm;
    if(HVIL_get_State() == HVIL_OPERATION_ON){
        if(!HVIL_get_Feedback()){
            s16_Speed_rpm = Inverter_Get_Speed_rpm();
            if((s16_Speed_rpm >= FUSI_HVIL_SPEED_LIMIT_RPM) || (s16_Speed_rpm <= (-FUSI_HVIL_SPEED_LIMIT_RPM))){
                st_Fusi.un_System.bits.b2_HVIL_Fault_With_Speed = FUSI_ON_FLAG;
                if(st_Fusi.u8_DTC_Level <= 8){
                    Fusi_set_DTC(0x5316u);
                    st_Fusi.u8_DTC_Level = 8;
                }
            }else{
                st_Fusi.un_System.bits.b2_HVIL_Fault_No_Speed = FUSI_ON_FLAG;
                if(st_Fusi.u8_DTC_Level <= 8){
                    Fusi_set_DTC(0x5317u);
                    st_Fusi.u8_DTC_Level = 8;
                }
            }
        }
        else{
            st_Fusi.un_System.bits.b2_HVIL_Fault_No_Speed = FUSI_OFF_FLAG;
            st_Fusi.un_System.bits.b2_HVIL_Fault_With_Speed = FUSI_OFF_FLAG;
        }
    }
}

void Fusi_Check_Contactor_cant_Close(void){
    if(PDU_get_Fault_Bypass()){
        st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 9){
            Fusi_set_DTC(0x5002u);
            st_Fusi.u8_DTC_Level = 9;
        }
    }
    else{
        st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Contactor_is_Welded(void){
    if(PDU_get_Fault_Welded()){
        st_Fusi.un_PDU.bits.b2_Contactor_Welded = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 9){
            Fusi_set_DTC(0x5003u);
            st_Fusi.u8_DTC_Level = 9;
        }
    }
    else{
        st_Fusi.un_PDU.bits.b2_Contactor_Welded = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Precharge_Failure(void){
    if(PDU_get_Fault_Precharge()){
        st_Fusi.un_PDU.bits.b2_Precharge_Failure = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 8){
            Fusi_set_DTC(0x5004u);
            st_Fusi.u8_DTC_Level = 8;
        }
    }
    else{
        st_Fusi.un_PDU.bits.b2_Precharge_Failure = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Discharge_Failure(void){
    if(PDU_get_Fault_Discharge()){
        st_Fusi.un_PDU.bits.b2_Discharge_Failure = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 8){
            Fusi_set_DTC(0x5005u);
            st_Fusi.u8_DTC_Level = 8;
        }
    }
    else{
        st_Fusi.un_PDU.bits.b2_Discharge_Failure = FUSI_OFF_FLAG;
    }
}

void Fusi_Check_Precharge_Cant_Close_Failure(void){
    if(PDU_get_Fault_Precharge()){
        st_Fusi.un_PDU.bits.b2_Precharge_Cant_Close = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 8){
            Fusi_set_DTC(0x5006u);
            st_Fusi.u8_DTC_Level = 8;
        }
    }
    else{
        st_Fusi.un_PDU.bits.b2_Precharge_Cant_Close = FUSI_OFF_FLAG;
    }
}

void Fusi_Charge_Failure(void);
void Fusi_ACU_CAN(void);

void Fusi_Check_BTMS(void){
    if(Battery_get_BTMS_Error()){
        st_Fusi.un_Battery.bits.b2_BTMS_Failure = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 4){
            Fusi_set_DTC(0x4206u);
            st_Fusi.u8_DTC_Level = 4;
        }
    }
    else{
        st_Fusi.un_Battery.bits.b2_BTMS_Failure = FUSI_OFF_FLAG;
    }
}


void Fusi_Low_Pressure_Critical(void){
    if(System_get_State() == ECU_STATE_DRIVE){
        st_Fusi_Counters.u16_Air_Start_Counter++;
        if(st_Fusi_Counters.u16_Air_Start_Counter >= FUSI_AIR_START_LIMIT_10MS){
            st_Fusi_Counters.u16_Air_Start_Counter = FUSI_AIR_START_LIMIT_10MS + 1;
        }
        if(Jarvis_get_Pressure_mBar() < FUSI_CRITICAL_PRESSURE_MBAR){
            st_Fusi.un_Air.bits.b2_No_Pressure = FUSI_ON_FLAG;
            if(st_Fusi_Counters.u16_Air_Start_Counter > FUSI_AIR_START_LIMIT_10MS){
                st_Fusi.un_Air.bits.b2_No_Pressure_Vision = FUSI_ON_FLAG;
                if(st_Fusi.u8_DTC_Level <= 7){
                    Fusi_set_DTC(0x4104u);
                    st_Fusi.u8_DTC_Level = 7;
                }
            }
        }
        if(Jarvis_get_Pressure_mBar() > FUSI_CRITICAL_PRESSURE_MBAR + 200){
            st_Fusi.un_Air.bits.b2_No_Pressure = FUSI_OFF_FLAG;
            st_Fusi.un_Air.bits.b2_No_Pressure_Vision = FUSI_OFF_FLAG;
        }
    }
    else{
        st_Fusi.un_Air.bits.b2_No_Pressure = FUSI_OFF_FLAG;
        st_Fusi.un_Air.bits.b2_No_Pressure_Vision = FUSI_OFF_FLAG;
        st_Fusi_Counters.u16_Air_Start_Counter = 0;
    }
}

void Fusi_Battery_OverTemp(void);
void Fusi_Steering_Failure(void);
void Fusi_Steering_CAN(void);
void Fusi_Low_SOC_Critical(void);

void Fusi_PSU_Not_Working(void){
    if(System_get_DCLink_State()){
        if(!SCU_get_PSU_in_Working()){
            st_Fusi.un_AuxInv.bits.b2_PSU_Not_Working = FUSI_ON_FLAG;
            if(st_Fusi.u8_DTC_Level <= 2){
                Fusi_set_DTC(0x4C03u);
                st_Fusi.u8_DTC_Level = 2;
            }
            return;
        }
    }
    st_Fusi.un_AuxInv.bits.b2_PSU_Not_Working = FUSI_OFF_FLAG;
}

void Fusi_PSU_Low_Voltage(void){
    if(System_get_DCLink_State()){
        if(SCU_get_PSU_Filtered_Voltage_V() < FUSI_SECURE_LOW_VOLTAGE){
            st_Fusi.un_AuxInv.bits.b2_PSU_Low_Voltage = FUSI_ON_FLAG;
            if(st_Fusi.u8_DTC_Level <= 6){
                Fusi_set_DTC(0x4C04u);
                st_Fusi.u8_DTC_Level = 6;
            }
            return;
        }
    }
    st_Fusi.un_AuxInv.bits.b2_PSU_Low_Voltage = FUSI_OFF_FLAG;
}
void Fusi_PSU_Fault(void){
    if(SCU_get_PSU_in_Error() || SCU_get_PSU_in_Warning()){
        st_Fusi.un_AuxInv.bits.b2_PSU_Fault = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 6){
            Fusi_set_DTC(0x4C05u);
            st_Fusi.u8_DTC_Level = 6;
        }
    }
    else{
        st_Fusi.un_AuxInv.bits.b2_PSU_Fault = FUSI_OFF_FLAG;
    }
}

void Fusi_DCU_Fault(void){
    if(SCU_get_DCU_in_Error() || SCU_get_DCU_in_Warning()){
        st_Fusi.un_AuxInv.bits.b2_DCU_Fault = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 7){
            Fusi_set_DTC(0x4C06u);
            st_Fusi.u8_DTC_Level = 7;
        }
    }
    else{
        st_Fusi.un_AuxInv.bits.b2_DCU_Fault = FUSI_OFF_FLAG;
    }
}

void Fusi_SCU_HV_Fuse(void){
    if((SCU_get_Voltage_V() < FUSI_DCLINK_MIN_VOLTAGE) && (Inverter_Get_Power_Voltage_V() > FUSI_DCLINK_MIN_VOLTAGE)){
        st_Fusi.un_AuxInv.bits.b2_SCU_HV_Fuse = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 7){
            Fusi_set_DTC(0x4C07u);
            st_Fusi.u8_DTC_Level = 7;
        }
    }
    else{
        st_Fusi.un_AuxInv.bits.b2_SCU_HV_Fuse = FUSI_OFF_FLAG;
    }
}

void Fusi_Ignition_Off_Drive(void){
    if((System_get_State() == ECU_STATE_DRIVE) && (Key_get_State() == KEY_OFF)){
        st_Fusi.un_System.bits.b2_Key_Off_on_Drive = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 2){
            Fusi_set_DTC(0x530Au);
            st_Fusi.u8_DTC_Level = 2;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Key_Off_on_Drive = FUSI_OFF_FLAG;
    }

}

void Fusi_Regen_Limit(void){
    /*
    if((System_get_State() == ECU_STATE_DRIVE)){
        if(Battery_get_ImpulseCharge_Current() < FUSI_SECURE_INPULSE_CHARGE_CURRENT_A){
            st_Fusi.un_Motor.bits.b2_Regen_Limitation = FUSI_ON_FLAG;
            if(st_Fusi.u8_DTC_Level <= 4){
                Fusi_set_DTC(0x4D05u);
                st_Fusi.u8_DTC_Level = 4;
            }
            return;
        }
    }
    st_Fusi.un_Motor.bits.b2_Regen_Limitation = FUSI_OFF_FLAG;
    */
}

void Fusi_Check_Motor_Coolant_Level(void){
    if(!GPIO_get_Cooling_Tank() && System_get_DCLink_State()){
        st_Fusi.un_Motor.bits.b2_Coolant_Low = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 5){
            Fusi_set_DTC(0x4D04u);
            st_Fusi.u8_DTC_Level = 5;
        }
        return;
    }
    st_Fusi.un_Motor.bits.b2_Coolant_Low = FUSI_OFF_FLAG;
}

void Fusi_Check_Heating_Coolant_Level(void){
    if(!GPIO_get_Heating_Tank() && System_get_DCLink_State()){
        st_Fusi.un_Heating.bits.b2_Low_Coolant = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x4802u);
            st_Fusi.u8_DTC_Level = 1;
        }
        return;
    }
    st_Fusi.un_Heating.bits.b2_Low_Coolant = FUSI_OFF_FLAG;
}

void Fusi_Check_Vision_CAN(void){
    st_Fusi.un_System.bits.b2_Vision_No_Respoce =  FUSI_OFF_FLAG;
    if(x_sys_get_time_ms() > st_Fusi_Counters.u32_Vision_No_Responce_Time_Limit){
        if(st_Fusi_Counters.u32_Vision_No_Responce_Time_Limit > FUSI_VISON_MAX_RESPONCE_TIME_10MS){
            st_Fusi.un_System.bits.b2_Vision_No_Respoce =  FUSI_ON_FLAG;
            if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x530Fu);
            st_Fusi.u8_DTC_Level = 1;
        }
        }
    }
}

void Fusi_Reset_Vison_Responce_Limit(void){
    st_Fusi_Counters.u32_Vision_No_Responce_Time_Limit = x_sys_get_time_ms() + FUSI_VISON_MAX_RESPONCE_TIME_10MS;
}

void Fusi_Set_Battery_CAN(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_Battery.bits.b2_CAN = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 11){
            Fusi_set_DTC(0x4200u);
            st_Fusi.u8_DTC_Level = 11;
        }
    }
    else{
        st_Fusi.un_Battery.bits.b2_CAN = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_Off_Inverter(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_Off_Inverter_Timout = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 10){
            Fusi_set_DTC(0x530Bu);
            st_Fusi.u8_DTC_Level = 10;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_Off_Inverter_Timout = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_Off_Current(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_Off_Current_Timeout = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x530Cu);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_Off_Current_Timeout = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_Off_PDU(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_Off_PDU_Open_Timeout = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 13){
            Fusi_set_DTC(0x530Du);
            st_Fusi.u8_DTC_Level = 13;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_Off_PDU_Open_Timeout = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_Off_Battery(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_Off_Battery_Open_Timeout = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 13){
            Fusi_set_DTC(0x530Eu);
            st_Fusi.u8_DTC_Level = 13;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_Off_Battery_Open_Timeout = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_On_PDU_Circuits(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_On_PDU_Circuits = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 3){
            Fusi_set_DTC(0x5310u);
            st_Fusi.u8_DTC_Level = 3;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_On_PDU_Circuits = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_On_Battery_CAN(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_On_Battery_CAN = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x5311u);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_On_Battery_CAN = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_On_Protocol_Inverter(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_On_Protocol_Inverter = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x5312u);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_On_Protocol_Inverter = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_On_Battery_Ready(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_On_Battery_Ready = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x5313u);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_On_Battery_Ready = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_On_PDU_Close(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_On_PDU_Close = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 2){
            Fusi_set_DTC(0x5314u);
            st_Fusi.u8_DTC_Level = 2;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_On_PDU_Close = FUSI_OFF_FLAG;
    }
}
void Fusi_Set_Power_On_HVIL(uint8 u8_ref){
    if(u8_ref){
        st_Fusi.un_System.bits.b2_Power_On_HVIL = FUSI_ON_FLAG;
        if(st_Fusi.u8_DTC_Level <= 2){
            Fusi_set_DTC(0x5315u);
            st_Fusi.u8_DTC_Level = 2;
        }
    }
    else{
        st_Fusi.un_System.bits.b2_Power_On_HVIL = FUSI_OFF_FLAG;
    }
}

void Fusi_Get_Battery_CAN(void){
    if(st_Fusi.un_Battery.bits.b2_CAN == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 11){
            Fusi_set_DTC(0x4200u);
            st_Fusi.u8_DTC_Level = 11;
        }
    }
}
void Fusi_Get_Power_Off_Inverter(void){
    if(st_Fusi.un_System.bits.b2_Power_Off_Inverter_Timout == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 10){
            Fusi_set_DTC(0x530Bu);
            st_Fusi.u8_DTC_Level = 10;
        }
    }
}
void Fusi_Get_Power_Off_Current(void){
    if(st_Fusi.un_System.bits.b2_Power_Off_Current_Timeout == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x530Cu);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
}
void Fusi_Get_Power_Off_PDU(void){
    if(st_Fusi.un_System.bits.b2_Power_Off_PDU_Open_Timeout == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 13){
            Fusi_set_DTC(0x530Du);
            st_Fusi.u8_DTC_Level = 13;
        }
    }
}
void Fusi_Get_Power_Off_Battery(void){
    if(st_Fusi.un_System.bits.b2_Power_Off_Battery_Open_Timeout == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 13){
            Fusi_set_DTC(0x530Eu);
            st_Fusi.u8_DTC_Level = 13;
        }
    }
}
void Fusi_Get_Power_On_PDU_Circuits(void){
    if(st_Fusi.un_System.bits.b2_Power_On_PDU_Circuits == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 3){
            Fusi_set_DTC(0x5310u);
            st_Fusi.u8_DTC_Level = 3;
        }
    }
}
void Fusi_Get_Power_On_Battery_CAN(void){
    if(st_Fusi.un_System.bits.b2_Power_On_Battery_CAN == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x5311u);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
}
void Fusi_Get_Power_On_Protocol_Inverter(void){
    if(st_Fusi.un_System.bits.b2_Power_On_Protocol_Inverter == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x5312u);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
}
void Fusi_Get_Power_On_Battery_Ready(void){
    if(st_Fusi.un_System.bits.b2_Power_On_Battery_Ready == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 1){
            Fusi_set_DTC(0x5313u);
            st_Fusi.u8_DTC_Level = 1;
        }
    }
}
void Fusi_Get_Power_On_PDU_Close(void){
    if(st_Fusi.un_System.bits.b2_Power_On_PDU_Close == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 2){
            Fusi_set_DTC(0x5314u);
            st_Fusi.u8_DTC_Level = 2;
        }
    }
}
void Fusi_Get_Power_On_HVIL(void){
    if(st_Fusi.un_System.bits.b2_Power_On_HVIL  == FUSI_ON_FLAG){
        if(st_Fusi.u8_DTC_Level <= 2){
            Fusi_set_DTC(0x5315u);
            st_Fusi.u8_DTC_Level = 2;
        }
    }
}



uint8 Fusi_Change_State_Check(void){
    uint8 u8_res;
    u8_res = TRUE;
    if(!Jarvis_get_Button_Hand_Break())
        u8_res = FALSE;

    if((Gear_get_State() == GEAR_DIRECT) || (Gear_get_State() == GEAR_REVERSE))
        u8_res = FALSE;

    if(Throttle_Get_Thottle_pu() > FUSI_SECURE_THROTTLE_PU)
        u8_res = FALSE;

    if(Inverter_Get_Filtered_Speed_rpm() > FUSI_SECURE_SPEED_RPM)
        u8_res = FALSE;

    if(u8_res){
        st_Fusi.un_System.bits.b2_Change_State_Conditions = FUSI_OFF_FLAG;
    }
    else{
        st_Fusi.un_System.bits.b2_Change_State_Conditions = FUSI_ON_FLAG;

    }
    return u8_res;
}

float32 Fusi_get_speed_Derrating(sint16 s16_Speed_rpm){
    float32 f32_der;
    float32 f32_Speed_rpm;

    if(st_Fusi.un_Actions.bits.b2_Limit_Speed == FUSI_OFF_FLAG)
        return 1.0f;

    if(s16_Speed_rpm >= 0)
        f32_Speed_rpm = s16_Speed_rpm;
    if(s16_Speed_rpm < 0)
        f32_Speed_rpm = - s16_Speed_rpm;

    f32_der = FUSI_DERRATING_SPEED_M * f32_Speed_rpm + FUSI_DERRATING_SPEED_C;

    if(f32_der > 1.0f)
        f32_der = 1.0f;
    if(f32_der < 0.0f)
        f32_der = 0.0f;

    return f32_der;
}

uint8 Fusi_get_Check_Engine(void){
    return st_Fusi.un_Actions.bits.b2_Chek_Engine;
}
uint8 Fusi_get_Request_Stop(void){
    return (st_Fusi.un_Actions.bits.b2_Park_Request == FUSI_ON_FLAG);
}
uint8 Fusi_get_Motor_Off(void){
    return (st_Fusi.un_Actions.bits.b2_Motor_Off == FUSI_ON_FLAG);
}

uint8 Fusi_get_Buzzer(void){
    return st_Fusi.un_Actions.bits.b2_Buzzer;
}

uint8 Fusi_get_Limit_Speed(void){
    return (st_Fusi.un_Actions.bits.b2_Limit_Speed == FUSI_ON_FLAG);
}

uint8 Fusi_get_No_Pressure(void){
    return (st_Fusi.un_Air.bits.b2_No_Pressure == FUSI_ON_FLAG);
}

uint8 Fusi_get_Pressure_Ok(void){
    return (Jarvis_get_Pressure_mBar() > FUSI_SECURE_PRESSURE_MBAR);
}

uint8 Fusi_get_Vision_No_Responce(void){
    return st_Fusi.un_System.bits.b2_Vision_No_Respoce;
}

uint8 Fusi_get_Block_Power_On(void){
    return (st_Fusi.un_Actions.bits.b2_Block_Power_On == FUSI_ON_FLAG);
}

uint8 Fusi_get_Power_Off_Now(void){
    return (st_Fusi.un_Actions.bits.b2_Power_Off_Now == FUSI_ON_FLAG);
}

uint8 Fusi_get_Power_Off_Park(void){
    return (st_Fusi.un_Actions.bits.b2_Power_Off_Park == FUSI_ON_FLAG);
}

void Fusi_Acction_Check_Engine(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_OFF_FLAG;

//Blink
    //System
    if(st_Fusi.un_System.bits.b2_Vision_No_Respoce == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_Power_On_Battery_CAN == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_Power_On_Protocol_Inverter == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_Power_On_Battery_Ready == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_Power_On_PDU_Close == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_BLINKING;
    }

    //PDU Check
    if(st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Precharge_Failure == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Discharge_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }

    //Battery
    if(st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault == FUSI_ON_FLAG ||
        st_Fusi.un_Battery.bits.b2_Isolation_Neg_Warning == FUSI_ON_FLAG ||
        st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault == FUSI_ON_FLAG ||
        st_Fusi.un_Battery.bits.b2_Isolation_Pos_Warning == FUSI_ON_FLAG ||
        st_Fusi.un_Battery.bits.b2_Low_Coolant == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_BLINKING;
    }

    //Motor Check

    //Air Check
    if(st_Fusi.un_Air.bits.b2_Leak == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_BLINKING;
    }

    //AuxInv
    if(st_Fusi.un_AuxInv.bits.b2_PSU_Not_Working == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_BLINKING;
    }

    //Heating
    if(st_Fusi.un_Heating.bits.b2_Low_Coolant == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_BLINKING;
    }

//On
    //System
    if(st_Fusi.un_System.bits.b2_Power_On_PDU_Circuits == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_Power_On_HVIL == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_HVIL_Fault_With_Speed == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_HVIL_Fault_No_Speed == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }

    //PDU Check
    if(st_Fusi.un_PDU.bits.b2_Low_Voltage == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Contactor_Welded == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }

    //Battery Check
    if(st_Fusi.un_Battery.bits.b2_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Catastrofic_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_BTMS_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }

    //Motor Check
    if(st_Fusi.un_Motor.bits.b2_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Motor.bits.b2_Coolant_Low == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }

    //Air Check
    if(st_Fusi.un_Air.bits.b2_Sensor_Error == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }


    //AuxInv
    if(st_Fusi.un_AuxInv.bits.b2_PSU_Low_Voltage == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_PSU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_DCU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_SCU_HV_Fuse == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_CHECK_ENGINE_ON;
    }

    st_Fusi.un_Actions.bits.b2_Chek_Engine = u8_Flag;
}

void Fusi_Acction_Buzzer(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_BUZZER_OFF;

//Blinking Buzzer
    //System Check
    if(st_Fusi.un_System.bits.b2_Change_State_Conditions == FUSI_ON_FLAG ||
        st_Fusi.un_System.bits.b2_Key_Off_on_Drive == FUSI_ON_FLAG ||
        st_Fusi.un_System.bits.b2_Power_On_PDU_Circuits == FUSI_ON_FLAG ||
        st_Fusi.un_System.bits.b2_Power_On_PDU_Close == FUSI_ON_FLAG ||
        st_Fusi.un_System.bits.b2_HVIL_Fault_With_Speed == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }
    //PDU
    if(st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Contactor_Welded == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Precharge_Failure == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Discharge_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }

    //Battery Check
    if(st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Low_Coolant == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Over_SOC_Warning == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }

    //Motor Check
    if(st_Fusi.un_Motor.bits.b2_Coolant_Low == FUSI_ON_FLAG ||
       st_Fusi.un_Motor.bits.b2_Regen_Limitation == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }

    //Air Check
    if(st_Fusi.un_Air.bits.b2_Sensor_Error == FUSI_ON_FLAG ||
        st_Fusi.un_Air.bits.b2_Leak == FUSI_ON_FLAG ||
        st_Fusi.un_Air.bits.b2_Low_Pressure == FUSI_ON_FLAG ||
        st_Fusi.un_Air.bits.b2_No_Pressure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }

    //AuxInv
    if(st_Fusi.un_AuxInv.bits.b2_PSU_Not_Working == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }

//ON Buzzer
    //System Check
    if(st_Fusi.un_System.bits.b2_Emergency_Stop == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_Power_On_HVIL == FUSI_ON_FLAG ||
       st_Fusi.un_System.bits.b2_HVIL_Fault_No_Speed == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_ON;
    }

    //PDU Check
    if(st_Fusi.un_PDU.bits.b2_Low_Voltage == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_ON;
    }

    //Battery Check
    if(st_Fusi.un_Battery.bits.b2_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Catastrofic_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_ON;
    }

    //Motor Check
    if(st_Fusi.un_Motor.bits.b2_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_ON;
    }

    //AuxInv
    if(st_Fusi.un_AuxInv.bits.b2_PSU_Low_Voltage == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_PSU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_DCU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_SCU_HV_Fuse == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_ON;
    }

    //Buzzer reference is OFF
    if(u8_Flag == FUSI_BUZZER_OFF){
        st_Fusi.un_Actions.bits.b2_Buzzer = u8_Flag;
        return;
    }

    //The DTC has change turn ON Buzzer
    if(st_Fusi.t_Buzzer_Timer.u16_Activation_DTC != st_Fusi.u16_DTC){
        st_Fusi.un_Actions.bits.b2_Buzzer = u8_Flag;
        st_Fusi.t_Buzzer_Timer.u32_timeout = 0;
        st_Fusi.t_Buzzer_Timer.u8_State = FUSI_ON_FLAG;
        st_Fusi.t_Buzzer_Timer.u16_Activation_DTC = st_Fusi.u16_DTC;
        return;
    }

    //The DTC has Not change
    st_Fusi.t_Buzzer_Timer.u32_timeout++;

    if(st_Fusi.t_Buzzer_Timer.u8_State){
        st_Fusi.un_Actions.bits.b2_Buzzer = u8_Flag;
        if(st_Fusi.t_Buzzer_Timer.u32_timeout >= FUSI_TIMEOUT_BUZZER_ON_10MS){
            st_Fusi.t_Buzzer_Timer.u8_State = FUSI_OFF_FLAG;
            st_Fusi.t_Buzzer_Timer.u32_timeout = 0;
        }
    }
    else{
        st_Fusi.un_Actions.bits.b2_Buzzer = FUSI_BUZZER_OFF;
        if(st_Fusi.t_Buzzer_Timer.u32_timeout >= FUSI_TIMEOUT_BUZZER_OFF_10MS){
            st_Fusi.t_Buzzer_Timer.u8_State = FUSI_ON_FLAG;
            st_Fusi.t_Buzzer_Timer.u32_timeout = 0;
        }
    }

}

void Fusi_Acction_Block_Power_ON(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_OFF_FLAG;

    //System
    if(st_Fusi.un_System.bits.b2_Power_On_PDU_Circuits == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //PDU Check
    if(st_Fusi.un_PDU.bits.b2_Low_Voltage == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Contactor_Welded == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Battery Check
    if(st_Fusi.un_Battery.bits.b2_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Catastrofic_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Isolation_Neg_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Isolation_Pos_Fault == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Motor Check
    if(st_Fusi.un_Motor.bits.b2_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Air Check
    if(st_Fusi.un_Air.bits.b2_Sensor_Error == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    st_Fusi.un_Actions.bits.b2_Block_Power_On = u8_Flag;
}

void Fusi_Acction_Park_Request(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_OFF_FLAG;

    //System Check
    /*
    if(st_Fusi.un_System.bits.b2_Change_State_Conditions == FUSI_ON_FLAG){
        u8_Flag = FUSI_ON_FLAG;
    }
    */
    if( st_Fusi.un_System.bits.b2_HVIL_Fault_With_Speed == FUSI_ON_FLAG ||
        st_Fusi.un_System.bits.b2_HVIL_Fault_No_Speed == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }


    //PDU Check
    if(st_Fusi.un_PDU.bits.b2_Low_Voltage == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Contactor_Cant_Close == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Contactor_Welded == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Precharge_Failure == FUSI_ON_FLAG ||
        st_Fusi.un_PDU.bits.b2_Discharge_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Battery Check
    if(st_Fusi.un_Battery.bits.b2_Failure == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Catastrofic_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Motor Check
    if(st_Fusi.un_Motor.bits.b2_Failure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Air Check
    if(st_Fusi.un_Air.bits.b2_Sensor_Error  == FUSI_ON_FLAG||
       st_Fusi.un_Air.bits.b2_No_Pressure_Vision == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //AuxInv
    if(st_Fusi.un_AuxInv.bits.b2_PSU_Low_Voltage == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_PSU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_DCU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_SCU_HV_Fuse == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    st_Fusi.un_Actions.bits.b2_Park_Request = u8_Flag;
}

void Fusi_Acction_Limit_Speed(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_OFF_FLAG;

    //Battery Check
    if(st_Fusi.un_Battery.bits.b2_Low_Coolant == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_OverTemp == FUSI_ON_FLAG ||
       st_Fusi.un_Battery.bits.b2_Over_SOC_Warning == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //Motor Check
    if(st_Fusi.un_Motor.bits.b2_Coolant_Low == FUSI_ON_FLAG ||
       st_Fusi.un_Motor.bits.b2_Regen_Limitation == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_BUZZER_BLINKING;
    }

    //Air Check
    if(st_Fusi.un_Air.bits.b2_Sensor_Error == FUSI_ON_FLAG ||
       st_Fusi.un_Air.bits.b2_Low_Pressure == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    //AuxInv
    if(st_Fusi.un_AuxInv.bits.b2_DCU_Fault == FUSI_ON_FLAG ||
       st_Fusi.un_AuxInv.bits.b2_SCU_HV_Fuse == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    st_Fusi.un_Actions.bits.b2_Limit_Speed = u8_Flag;
}

void Fusi_Acction_Power_Off_Park(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_OFF_FLAG;

    //System
    if(st_Fusi.un_System.bits.b2_HVIL_Fault_No_Speed ||
       st_Fusi.un_System.bits.b2_HVIL_Fault_With_Speed
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    st_Fusi.un_Actions.bits.b2_Power_Off_Park = u8_Flag;
}

void Fusi_Acction_Power_Off_Now(void){
    uint8 u8_Flag;
    u8_Flag = FUSI_OFF_FLAG;

    //System
    if(st_Fusi.un_System.bits.b2_Emergency_Stop == FUSI_ON_FLAG
    ){
        u8_Flag = FUSI_ON_FLAG;
    }

    st_Fusi.un_Actions.bits.b2_Power_Off_Now = u8_Flag;
}


void Fusi_Execute(void){
    st_Fusi.u16_DTC = 0;
    st_Fusi.u8_DTC_Level = 0;

    //System Checks
    Fusi_Check_Emergency_Stop();
    Fusi_Ignition_Off_Drive();
    Fusi_Check_Vision_CAN();
    Fusi_HVIL();

    //PDU Checks
    Fusi_Check_DCLink();

    //Motor Checks
    Fusi_Check_Motor_Failure();
    Fusi_Check_Motor_Coolant_Level();
    Fusi_Regen_Limit();

    //Battery Check
    Fusi_Check_Isolation();
    Fusi_Check_Battery_Failure();
    Fusi_Check_Battery_Catastrofic_Failure();
    //Fusi_Check_BTMS();
    Fusi_Check_Battery_High_SOC();

    //Air
    Fusi_Low_Pressure_Critical();

    //AuxInv
    Fusi_PSU_Not_Working();
    Fusi_PSU_Low_Voltage();
    Fusi_PSU_Fault();
    Fusi_DCU_Fault();
    Fusi_SCU_HV_Fuse();

    //Heating
    Fusi_Check_Heating_Coolant_Level();

    //Externals
    Fusi_Get_Battery_CAN();
    Fusi_Get_Power_Off_Inverter();
    Fusi_Get_Power_Off_Current();
    Fusi_Get_Power_Off_PDU();
    Fusi_Get_Power_Off_Battery();
    Fusi_Get_Power_On_PDU_Circuits();
    Fusi_Get_Power_On_Protocol_Inverter();
    Fusi_Get_Power_On_Battery_CAN();
    Fusi_Get_Power_On_Battery_Ready();
    Fusi_Get_Power_On_PDU_Close();
    Fusi_Get_Power_On_HVIL();


    //Actions
    Fusi_Acction_Check_Engine();
    Fusi_Acction_Buzzer();
    Fusi_Acction_Block_Power_ON();
    Fusi_Acction_Limit_Speed();
    Fusi_Acction_Park_Request();
    Fusi_Acction_Power_Off_Park();
    Fusi_Acction_Power_Off_Now();
}

uint8 Fusi_get_DTC_Level(void){
    return st_Fusi.u8_DTC_Level;
}

uint16 Fusi_get_DTC(void){
    return st_Fusi.u16_DTC;
}

uint8 Fusi_get_Change_State_Conditions_Status(void){
    return st_Fusi.un_System.bits.b2_Change_State_Conditions;
}

void Fusi_set_DTC(uint16 u16_DTC){
  st_Fusi.u16_DTC = u16_DTC;
  st_Fusi.u16_DTCs[3] = st_Fusi.u16_DTCs[2];
  st_Fusi.u16_DTCs[2] = st_Fusi.u16_DTCs[1];
  st_Fusi.u16_DTCs[1] = st_Fusi.u16_DTCs[0];
  st_Fusi.u16_DTCs[0] = u16_DTC;
}

void Fusi_Init_DTCs(void){
  st_Fusi.u16_DTCs[3] = 0;
  st_Fusi.u16_DTCs[2] = 0;
  st_Fusi.u16_DTCs[1] = 0;
  st_Fusi.u16_DTCs[0] = 0;
}


