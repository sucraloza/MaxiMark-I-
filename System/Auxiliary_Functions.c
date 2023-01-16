#include "OS_Defs.h"

uint16 System_Get_Aux_Current(void){
    return 20;
}

void System_Set_Inv_Limits(void){
    uint16 u16_Aveilable_Current;
    u16_Aveilable_Current = Battery_get_ImpulseDischarge_Current() - System_Get_Aux_Current();
    Inverter_Set_Discharge_Current_A(u16_Aveilable_Current);
    Inverter_Set_Charge_Current_A(Battery_get_ImpulseCharge_Current());
    Inverter_Set_Max_Voltage_V(700);
    Inverter_Set_Min_Voltage_V(500);
}

uint8 System_get_Is_Regen(void){
    if(Gear_get_State() == GEAR_DIRECT){
        if(Throttle_Get_Thottle_pu() < -0.03f){
            st_System.st_State.u8_IsRegen = TRUE;
        }
        if(Throttle_Get_Thottle_pu() >= 0.00f){
            st_System.st_State.u8_IsRegen = FALSE;
        }
    }
    else{
        st_System.st_State.u8_IsRegen = FALSE;
    }
    return st_System.st_State.u8_IsRegen;
}

void Update_Vision(void){
    Vision_Set_Batteries_Temperature_C(Battery_get_Temperature());
    Vision_Set_SOC(Battery_get_SOC());
    Vision_Set_Voltage_DC_V(SCU_get_PSU_Filtered_Voltage_V());
    Vision_Set_Air_Pressure_mBar(Jarvis_get_Pressure_mBar());
    Vision_Set_Engine_Temperature_C(SCU_get_PSU_Temperature_C());
    Vision_Set_Gun_Connected(Battery_get_Gun_Connection());
    Vision_Set_Handbreak(Jarvis_get_Button_Hand_Break());
    Vision_Set_Check_Engine(Fusi_get_Check_Engine());
    Vision_Set_Engine_Limitation(Fusi_get_Limit_Speed());
    Vision_Set_Isolation(System_Get_Isolation());
    Vision_Set_Gear(Gear_get_State());
    Vision_Set_Is_Charging(!System_get_Charge_Lock());
    Vision_Set_Emergency_Button(GPIO_get_Emergency());
    Vision_Set_Stop_Required(Fusi_get_Request_Stop());
    Vision_Set_Jarvis_Major_Version(Jarvis_get_Major_Version());
    Vision_Set_Jarvis_Minor_Version(Jarvis_get_Minor_Version());
    Vision_Set_Jarvis_Pach_Version(Jarvis_get_Pach_Version());
    Vision_Set_Fusi_DTC_Level(Fusi_get_DTC_Level());
    Vision_Set_Fusi_DTC(Fusi_get_DTC());
    Vision_Set_Change_Page(Buttons_Get_Change_Page());
    float32 f32_km;
    Odometer_Get_Km(&f32_km);
    Vision_Set_Odometer_km(f32_km);

}
void Update_Vision_Batteries(void){
    Vision_Set_Batteries_Temperature_C(Battery_get_Temperature());
    Vision_Set_SOC(Battery_get_SOC());
}

void Update_Vision_Converter(void){
    Vision_Set_Voltage_DC_V(SCU_get_PSU_Filtered_Voltage_V());
    Vision_Set_Air_Pressure_mBar(Jarvis_get_Pressure_mBar());
}

void Update_Vision_System(void){
    Vision_Set_Engine_Temperature_C(SCU_get_PSU_Temperature_C());
    Vision_Set_Gun_Connected(Battery_get_Gun_Connection());
    Vision_Set_Handbreak(Jarvis_get_Button_Hand_Break());
    Vision_Set_Check_Engine(Fusi_get_Check_Engine());
    Vision_Set_Engine_Limitation(Fusi_get_Limit_Speed());
}

void Update_Vision_System_Critical(void){
    Vision_Set_Isolation(System_Get_Isolation());
    Vision_Set_Gear(Gear_get_State());
    Vision_Set_Is_Charging(!System_get_Charge_Lock());
    Vision_Set_Emergency_Button(GPIO_get_Emergency());
    Vision_Set_Stop_Required(Fusi_get_Request_Stop());
}

void Update_Vision_Versions(void){
    Vision_Set_Jarvis_Major_Version(Jarvis_get_Major_Version());
    Vision_Set_Jarvis_Minor_Version(Jarvis_get_Minor_Version());
    Vision_Set_Jarvis_Pach_Version(Jarvis_get_Pach_Version());
}

void Update_Vision_Fusi(void){
    Vision_Set_Fusi_DTC_Level(Fusi_get_DTC_Level());
    Vision_Set_Fusi_DTC(Fusi_get_DTC());
}

void System_Detect_Drive_Block(void){
    if(x_os_mutex_get(&t_Mutex_System,10) !=  C_NO_ERR){
        return;
    }
    st_System.st_State.u8_Drive_Block = Battery_get_Gun_Connection();
    x_os_mutex_release(&t_Mutex_System);
}

uint16 System_Get_Isolation(void){
    return Battery_get_Isolation_kohm();
}

float32 System_get_PDU_Current(void){
    return Inverter_Get_Power_Current_A() + Jarvis_get_Aux_Current();
}

void Battery_Thermal_Management(void){
    if(!System_get_DCLink_State())
    {
        BTMS_Set_OnOff(FALSE);
        BTMS_Set_HVVoltage_V(0);
        BTMS_Set_WorkMode(BTMS_WORK_SHUTDOWN);
        BTMS_Set_Ref_Temperature_C(25);
        return;
    }

    BTMS_Set_HVVoltage_V(Inverter_Get_Power_Voltage_V());

    if(st_Monitoring.st_Monitoring_BTMS.u8_BTMS_Manual_Control){
        BTMS_Set_OnOff(st_Monitoring.st_Monitoring_BTMS.u8_BTMS_OnOff);
        BTMS_Set_WorkMode(st_Monitoring.st_Monitoring_BTMS.u8_BTMS_Work_Status);
        BTMS_Set_Ref_Temperature_C(st_Monitoring.st_Monitoring_BTMS.u8_BTMS_Ref_Temp);
        return;
    }

    sint8 s8_Bat_Avg_Temp_C = Battery_get_Temperature();
    BTMS_Set_OnOff(TRUE);

    if(s8_Bat_Avg_Temp_C <= 15)
    {
        BTMS_Set_WorkMode(BTMS_WORK_SHUTDOWN);
        BTMS_Set_Ref_Temperature_C(23);
    }
    else if(s8_Bat_Avg_Temp_C >= 17 && s8_Bat_Avg_Temp_C <= 23)
    {
        BTMS_Set_WorkMode(BTMS_WORK_SELF_LOOP);
        BTMS_Set_Ref_Temperature_C(23);
    }
    else if(s8_Bat_Avg_Temp_C >= 25)
    {
        BTMS_Set_WorkMode(BTMS_WORK_COOLING);
        BTMS_Set_Ref_Temperature_C(23);
    }
}

void Update_Buzzer(void)
{
    sint16 s16_kmh = Inverter_get_Speed_kmh();
    uint8 u8_kmh = (uint8) ((s16_kmh < 0) ? -s16_kmh : s16_kmh);
    Buzzer_Set_Speed(u8_kmh);
}

uint8 get_EEPROM_State(void)
{
    return st_EEPROM.u8_EEPROM_Enable;
}
