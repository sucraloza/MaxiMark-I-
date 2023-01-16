#include "OS_Defs.h"

float64 System_HVIL_Filter(float32 f32_X0){
    float32 f64_Y0;
    st_System.st_HVIL.st_Filter.f64_GX = st_System.st_HVIL.st_Filter.f32_Gain_A0 * f32_X0 + st_System.st_HVIL.st_Filter.f32_Gain_A1 * st_System.st_HVIL.st_Filter.f32_X1;
    st_System.st_HVIL.st_Filter.f64_GY =  - (st_System.st_HVIL.st_Filter.f64_Gain_B1) * (st_System.st_HVIL.st_Filter.f64_Y1);
    f64_Y0 = st_System.st_HVIL.st_Filter.f64_GX + st_System.st_HVIL.st_Filter.f64_GY ;
    //y0 = 0.2 x0 + 0.8 y1;

    st_System.st_HVIL.st_Filter.f32_X1 = f32_X0;
    st_System.st_HVIL.st_Filter.f64_Y1 = f64_Y0;

    return f64_Y0;
}

void System_Output_Update(void){
    if(HVIL_get_ref_GPIO()) GPIO_set_HVIL_Ref(DS_ON);
    else GPIO_set_HVIL_Ref(DS_OFF);
    Telemetry_Track_HVIL_Reference(HVIL_get_ref_GPIO());

    GPIO_Update_Outputs();
}

void System_Input_Update(void){
    GPIO_Update_Inputs();
    Key_Update(GPIO_get_Key_Acc(),GPIO_get_Key_Start());
    Telemetry_Track_Key_State(Key_get_State());
    HVIL_set_Feedback(GPIO_get_HVIL_Feedback());
    Telemetry_Track_HVIL_Feedback(HVIL_get_Feedback());
}

uint8 System_get_Traction_Ready(void){
    if(st_System.st_State.u8_ECU_State != ECU_STATE_DRIVE)
        return FALSE;
    if(st_System.st_State.u8_Drive_Block)
        return FALSE;
    return TRUE;
}

uint8 System_get_Drive_Block(void){
    return st_System.st_State.u8_Drive_Block;
}

void System_set_Drive_Block(uint8 u8_Block_Ref){
    if(x_os_mutex_get(&t_Mutex_System,10) == C_NO_ERR){
        st_System.st_State.u8_Drive_Block = u8_Block_Ref;
        x_os_mutex_release(&t_Mutex_System);
    }
}

uint8 System_get_Charge_Lock(void){
    return st_System.st_State.u8_Charge_Lock;
}

void System_set_Charge_Lock(uint8 u8_Charge_Lock){
    if(x_os_mutex_get(&t_Mutex_System,10) == C_NO_ERR){
        st_System.st_State.u8_Charge_Lock = u8_Charge_Lock;
        x_os_mutex_release(&t_Mutex_System);
    }
}

uint8 System_get_Charge_State_Detected(void){
    return st_System.st_State.u8_Charge_State_Detected;
}
void System_set_Charge_State_Detected(uint8 u8_ref){
    if(x_os_mutex_get(&t_Mutex_System,10) == C_NO_ERR){
        st_System.st_State.u8_Charge_State_Detected = u8_ref;
        x_os_mutex_release(&t_Mutex_System);
    }
}

void System_Change_State(uint8 u8_New_State){
    if(x_os_mutex_get(&t_Mutex_System,10) != C_NO_ERR){
        return;
    }
    st_System.st_State.u8_ECU_State = u8_New_State;
    x_os_mutex_release(&t_Mutex_System);
    Telemetry_Track_ECU_State(u8_New_State);
}

uint8 System_get_State(void){
    return st_System.st_State.u8_ECU_State;
}

uint8 System_get_DCLink_State(void){
    if(st_System.st_State.u8_ECU_State == ECU_STATE_AUXILIARIES ||
       st_System.st_State.u8_ECU_State == ECU_STATE_DRIVE ||
       st_System.st_State.u8_ECU_State == ECU_STATE_CHARGE
       )
    {
        return TRUE;
    }
    return FALSE;
}

uint8 System_get_Motor_Coolant_Level(void){
    return GPIO_get_Cooling_Tank();
}

uint8 System_get_Heating_Tank_Level(void){
    return GPIO_get_Heating_Tank();
}

uint8 System_get_Steering_Tank_Level(void){
    return GPIO_get_Steering_Tank();
}

//System Power On Functions
uint8 System_Power_OnOff_Get_Timeout(void){
    //TODO FIX 49 DAYS TIMER LOOP
    //if(x_sys_get_time_ms() < st_System.st_Power_On.u32_Start_Time_ms)
    //    st_System.st_Power_On.u32_Start_Time_ms = x_sys_get_time_ms(); //just_extend the period

    if(x_sys_get_time_ms() >= st_System.st_Power_OnOff.u32_Time_Out_ms)
        return TRUE;
    else
        return FALSE;
}

void System_Power_On_Goto_State(uint8 u8_New_State){
    if(x_os_mutex_get(&t_Mutex_System,10) != C_NO_ERR){
        return;
    }
    switch(u8_New_State){
        case POWER_ON_STATE_CHECK_CIRCUITS:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 1000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_CHECK_CIRCUITS;
            break;
        case POWER_ON_STATE_POWER_ON_T15:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 1000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_POWER_ON_T15;
            break;
        case POWER_ON_STATE_BUTTONS:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 2000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_BUTTONS;
            break;
        case POWER_ON_STATE_BATTERY_CAN:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 3000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_BATTERY_CAN;
            break;
        case POWER_ON_STATE_PROTOCOL_INVERTER:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 2000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_PROTOCOL_INVERTER;
            break;
        case POWER_ON_STATE_BATTERY_READY:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 2000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_BATTERY_READY;
            break;
        case POWER_ON_STATE_CIRCUIT_CLOSE:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 10000;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_CIRCUIT_CLOSE;
            break;
        case POWER_ON_STATE_FAULT:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_FAULT;
            break;
        default:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            st_System.st_Power_OnOff.u8_Power_On_State = POWER_ON_STATE_FAULT;
            break;
    }
    x_os_mutex_release(&t_Mutex_System);

}

void System_Power_Off_Goto_State(uint8 u8_New_State){
    if(x_os_mutex_get(&t_Mutex_System,10) != C_NO_ERR){
        return;
    }
    switch(u8_New_State){
        case POWER_OFF_STATE_CHECK_INV_STANDBY:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 10000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_CHECK_INV_STANDBY;
            break;
        case POWER_OFF_STATE_DISABLE_SYSTEMS:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 2000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_DISABLE_SYSTEMS;
            break;
        case POWER_OFF_STATE_BATTERY_OPEN:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 5000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_BATTERY_OPEN;
            break;
        case POWER_OFF_STATE_CIRCUIT_OPEN:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 90000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_CIRCUIT_OPEN;
            break;
        case POWER_OFF_STATE_WAIT:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 2000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_WAIT;
            break;
        case POWER_OFF_STATE_POWER_OFF_T15:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 1000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_POWER_OFF_T15;
            break;
        case POWER_OFF_STATE_WAIT_BATTERY:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 7000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_WAIT_BATTERY;
            break;
        case POWER_OFF_STATE_FAULT:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 1000;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_OFF_STATE_FAULT;
            break;
        default:
            st_System.st_Power_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Power_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            st_System.st_Power_OnOff.u8_Power_Off_State = POWER_ON_STATE_FAULT;
            break;
    }
    x_os_mutex_release(&t_Mutex_System);

}

uint8 System_get_Power_On_State(){
    return st_System.st_Power_OnOff.u8_Power_On_State;
}

uint8 System_get_Power_Off_State(){
    return st_System.st_Power_OnOff.u8_Power_Off_State;
}

//System Vehicle On Functions
uint8 System_Vehicle_OnOff_Get_Timeout(void){
    //TODO FIX 49 DAYS TIMER LOOP
    //if(x_sys_get_time_ms() < st_System.st_Vehicle_OnOff.u32_Start_Time_ms)
    //    st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms(); //just_extend the period

    if(x_sys_get_time_ms() >= st_System.st_Vehicle_OnOff.u32_Time_Out_ms)
        return TRUE;
    else
        return FALSE;
}

void System_Vehicle_On_Goto_State(uint8 u8_New_State){
    if(x_os_mutex_get(&t_Mutex_System,10) != C_NO_ERR){
        return;
    }
    switch(u8_New_State){
        case VEHICLE_ON_STATE_CHECK_CHARGING:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 500;
            st_System.st_Vehicle_OnOff.u8_Vehicle_On_State = VEHICLE_ON_STATE_CHECK_CHARGING;
            break;
        case VEHICLE_ON_STATE_CHECK_AIRPUMP:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 30000;
            st_System.st_Vehicle_OnOff.u8_Vehicle_On_State = VEHICLE_ON_STATE_CHECK_AIRPUMP;
            break;
        case VEHICLE_ON_STATE_CHECK_THROTTLE:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            st_System.st_Vehicle_OnOff.u8_Vehicle_On_State = VEHICLE_ON_STATE_CHECK_THROTTLE;
            break;
        case VEHICLE_ON_STATE_CHECK_BREAK:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            st_System.st_Vehicle_OnOff.u8_Vehicle_On_State = VEHICLE_ON_STATE_CHECK_BREAK;
            break;
        case VEHICLE_ON_STATE_INV_OPERATIONAL:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 500;
            st_System.st_Vehicle_OnOff.u8_Vehicle_On_State = VEHICLE_ON_STATE_INV_OPERATIONAL;
            break;
        default:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            //st_System.st_Vehicle_OnOff.u8_Vehicle_On_State = VEHICLE_ON_STATE_CHECK_CHARGING;
            break;
    }
    x_os_mutex_release(&t_Mutex_System);

}

void System_Vehicle_Off_Goto_State(uint8 u8_New_State){
    if(x_os_mutex_get(&t_Mutex_System,10) != C_NO_ERR){
        return;
    }
    switch(u8_New_State){
        case VEHICLE_OFF_STATE_INVERTER_NEUTRAL:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 2000;
            st_System.st_Vehicle_OnOff.u8_Vehicle_Off_State = VEHICLE_OFF_STATE_INVERTER_NEUTRAL;
            break;
        case VEHICLE_OFF_STATE_INVERTER_STANDBY:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 500;
            st_System.st_Vehicle_OnOff.u8_Vehicle_Off_State = VEHICLE_OFF_STATE_INVERTER_STANDBY;
            break;
        default:
            st_System.st_Vehicle_OnOff.u32_Start_Time_ms = x_sys_get_time_ms();
            st_System.st_Vehicle_OnOff.u32_Time_Out_ms = x_sys_get_time_ms() + 100;
            //st_System.st_Vehicle_OnOff.u8_Vehicle_Off_State = VEHICLE_OFF_STATE_INVERTER_NEUTRAL;
            break;
    }
    x_os_mutex_release(&t_Mutex_System);

}

float32 Throttle_Get_Thottle_pu(void){
    //return st_Throttle.f32_Post_Ramp_Value_pu;
    return st_Throttle.f32_Final_Value_pu;
}

uint8 Throttle_Get_State(void){
    return 0;
}

void System_PDU_To_Battery(void){
    if(GPIO_get_PDU_Contactor_Main() == DS_OFF){
        Baterry_Set_Aux_Contator(BATTERY_RELAY_STATE_OPEN);
        return;
    }
    if(GPIO_get_PDU_Contactor_Main() == DS_ON){
        Baterry_Set_Aux_Contator(BATTERY_RELAY_STATE_CLOSE);
        return;
    }
    Baterry_Set_Aux_Contator(BATTERY_RELAY_STATE_INVALID);

}

uint8 System_get_Aux_State(void)
{
    return st_Monitoring.st_Monitoring_Control_1.u8_Auxiliary_ECU_State;
}

//System init error
void Error_Beep(void)
{
    x_sys_set_beeper(1000);
    x_sys_delay_us(100000);
    x_sys_set_beeper(X_OFF);
    return;
}

void Error_Beepers(void)
{
    x_sys_set_beeper(2000);
    x_sys_delay_us(500000);
    x_sys_set_beeper(X_OFF);
}
