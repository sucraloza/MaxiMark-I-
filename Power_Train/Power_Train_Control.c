#include "OS_Defs.h"

void Inverter_Safe_Send(sint16 s16_Error_Inv){
    s16_Error_Inv = s16_Error_Inv;
}

uint8 Inverter_Get_Potocol_Flag(void){
    return st_Inverter.un_State.bits.b1_Protocol_Set;
}
uint8 Inverter_Get_Operational_State(void){
    return st_Inverter.un_State.bits.b4_Operational_State;
}
uint8 Inverter_Get_Operational_Mode(void){
    return st_Inverter.un_State.bits.b3_Operational_Mode;
}
uint8 Inverter_Get_Command_Mode(void){
    return st_Inverter.un_State.bits.b2_Command_Mode;
}
uint8 Inverter_Get_Thermal_Indicator_pu(void){
    return st_Inverter.st_Cooling.u8_Thermal_Indicator_pu;
}
float32 Inverter_Get_Power_Current_A(void){
    return st_Inverter.st_Electrical.f32_Power_Current_A;
}
uint16 Inverter_Get_Power_Voltage_V(void){
    return st_Inverter.st_Electrical.u16_Power_Voltage_V;
}
sint16 Inverter_Get_Speed_rpm(void){
    return st_Inverter.st_Mechanical.s16_Speed_rpm;
}
sint16 Inverter_Get_Filtered_Speed_rpm(void){
    return st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm;
}
sint16 Inverter_get_Speed_kmh(void){
    return st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm * SPEED_FACTOR;
}
uint16 Inverter_Get_Available_Drive_Torque_Nm(void){
    return st_Inverter.st_Mechanical.u16_Drive_Torque_nm;
}
uint16 Inverter_Get_Available_Regen_Torque_Nm(void){
    return st_Inverter.st_Mechanical.u16_Regen_Torque_nm;
}
uint8 Inverter_Get_EmergencyStop(void){
    return st_Inverter.st_Fauts.un_Flags.bits.b1_Emergency_Stop;
}

uint8 Inverter_Is_Vehicle_Stop(void){
    if(( st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm <= INV_STOP_SPEED_RPM ) && ( st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm >= -INV_STOP_SPEED_RPM )){
        return TRUE;
    }
    return FALSE;
}

uint8 Inverter_Get_Derrating(void){
    uint8 u8_res;

    u8_res = st_Inverter.un_State.bits.b4_Traction_Derraiting & 0x0F;
    u8_res = u8_res << 4;
    u8_res |= st_Inverter.un_State.bits.b4_Regen_Derraiting & 0x0F;

    return u8_res;
}

void Inverter_Set_Potocol_Flag_False(void){
    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        st_Inverter.un_State.bits.b1_Protocol_Set = FALSE;
        return;
    }
    st_Inverter.un_State.bits.b1_Protocol_Set = FALSE;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Set_Operation_State(uint8 u8_Ref_State){

    if(u8_Ref_State != INV_REQ_STATE_OPERATIONAL &&
       u8_Ref_State != INV_REQ_STATE_STANDBY &&
       u8_Ref_State != INV_REQ_STATE_SHUTDOWN)
    {
        return;
    }

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State = u8_Ref_State;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Set_Operation_Mode(uint8 u8_Ref_Mode){
    if(u8_Ref_Mode != INV_REQ_MODE_EV &&
       u8_Ref_Mode != INV_REQ_MODE_NEUTRAL)
    {
        return;
    }

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = u8_Ref_Mode;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Set_Command_Mode(uint8 u8_Ref_Mode){
    if(u8_Ref_Mode != INV_REQ_CMD_MODE_SPEED &&
       u8_Ref_Mode != INV_REQ_CMD_MODE_TORQUE)
    {
        return;
    }

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode = u8_Ref_Mode;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Set_Torque_nm(float32 f32_Ref_Torque_nm){
    //TODO Absolut limits torque

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Refs.f32_Torque_nm = f32_Ref_Torque_nm;
    st_Inverter.st_Refs.f32_Safe_Torque_nm = f32_Ref_Torque_nm;
    st_Inverter.st_Refs.un_Flags.bits.b1_Torque_Valid = 0;
    st_Inverter.st_Refs.un_Flags.bits.b1_Speed_Valid = 1;
    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Set_Discharge_Current_A(uint16 u16_Ref_Discharge_Current_A){
    //TODO Absolute Limits Discharge Current

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Limits.u16_Max_Discharge_Current_A = u16_Ref_Discharge_Current_A;

    x_os_mutex_release(&t_Mutex_Inverter);
}
void Inverter_Set_Charge_Current_A(uint16 u16_Ref_Charge_Current_A){
    //TODO Absolute Limits Charge Current

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Limits.u16_Max_Charge_Current_A = u16_Ref_Charge_Current_A;

    x_os_mutex_release(&t_Mutex_Inverter);
}
void Inverter_Set_Max_Voltage_V(uint16 u16_Ref_Max_Voltage_V){
    //TODO Absolute Limits Max Voltage

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Limits.u16_Max_Battery_Voltage_V = u16_Ref_Max_Voltage_V;

    x_os_mutex_release(&t_Mutex_Inverter);
}
void Inverter_Set_Min_Voltage_V(uint16 u16_Ref_Min_Voltage){
    //TODO Absolute Limits Min Voltage

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        return;
    }

    st_Inverter.st_Limits.u16_Min_Battery_Voltage_V = u16_Ref_Min_Voltage;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Filter_Speed(void){
    float32 f32_X0;
    float64 f64_Y0;
    sint16  s16_Error;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm = -1.0f;
    }
    f32_X0 = st_Inverter.st_Mechanical.s16_Speed_rpm;
    st_Inverter.st_Speed_Filter.f64_GX = st_Inverter.st_Speed_Filter.f32_Gain_A0 * f32_X0 + st_Inverter.st_Speed_Filter.f32_Gain_A1 * st_Inverter.st_Speed_Filter.f32_X1;
    st_Inverter.st_Speed_Filter.f64_GY =  - ( st_Inverter.st_Speed_Filter.f64_Gain_B1) * (st_Inverter.st_Speed_Filter.f64_Y1);
    f64_Y0 = st_Inverter.st_Speed_Filter.f64_GX + st_Inverter.st_Speed_Filter.f64_GY ;

    st_Inverter.st_Speed_Filter.f32_X1 = f32_X0;
    st_Inverter.st_Speed_Filter.f64_Y1 = f64_Y0;

    st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm = f64_Y0;
    x_os_mutex_release(&t_Mutex_Inverter);

    Odometer_Set_Speed((uint8) (SPEED_FACTOR * st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm));
}

float32 Inverter_get_Reverse_Speed_derrating_pu(void){
    float32 f32_der;
    f32_der = INV_REVERSE_DERRATING_M * st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm + INV_REVERSE_DERRATING_C;
    if(f32_der > 1.0f)
        f32_der = 1.0f;
    if(f32_der < 0.0f)
        f32_der = 0.0f;
    return f32_der;
}

void Inverter_Calculate_Torque(void){
    float f32_Ref_Torque;
    uint8 u8_Gear_Position;
    u8_Gear_Position = Gear_get_State();

    if(x_os_mutex_get(&t_Mutex_Inverter,10) != C_NO_ERR){
        //TODO FAULT INVERTER MUTEX
        return;
    }

    if(Inverter_Get_EmergencyStop() || Fusi_get_Motor_Off()){
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State = INV_REQ_STATE_OPERATIONAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_NEUTRAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_EV;
        st_Inverter.st_Refs.f32_Torque_nm = 0;
        st_Inverter.st_Refs.f32_Safe_Torque_nm = 0;
        st_Inverter.st_Refs.s16_Speed_rpm = 0;
        st_Inverter.st_Refs.s16_Safe_Speed_rpm = 0;

        x_os_mutex_release(&t_Mutex_Inverter);
        return;
    }

    if(u8_Gear_Position == GEAR_NEUTRAL){
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State = INV_REQ_STATE_OPERATIONAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_NEUTRAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_EV;
        //st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode = INV_REQ_CMD_MODE_TORQUE;
        st_Inverter.st_Refs.f32_Torque_nm = 0;
        st_Inverter.st_Refs.f32_Safe_Torque_nm = 0;
        st_Inverter.st_Refs.s16_Speed_rpm = 0;
        st_Inverter.st_Refs.s16_Safe_Speed_rpm = 0;
    }
    
    if(u8_Gear_Position == GEAR_DIRECT && st_Inverter.un_State.bits.b4_Operational_State == INV_STATE_OPERATIONAL){
        f32_Ref_Torque = st_Inverter.st_Parameters.f32_Direct_Torque_Gain_nm * Throttle_Get_Thottle_pu();
        if(f32_Ref_Torque > 0){
            f32_Ref_Torque = Fusi_get_speed_Derrating(st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm) * f32_Ref_Torque;
        }

        if(((!Steering_is_On()) || Fusi_get_No_Pressure()) && st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm >= 0 ){
            if(f32_Ref_Torque > 0){
                f32_Ref_Torque = 0;
            }
        }

        if(st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm < 100){
            if(f32_Ref_Torque < 0){
                f32_Ref_Torque = 0;
            }
        }

        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State = INV_REQ_STATE_OPERATIONAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_EV;
        st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode = INV_REQ_CMD_MODE_TORQUE;

        if(f32_Ref_Torque >= 0){
            if(f32_Ref_Torque <= st_Inverter.st_Parameters.f32_Maximun_Foward_Torque_nm){
                st_Inverter.st_Refs.f32_Torque_nm = f32_Ref_Torque;
                st_Inverter.st_Refs.f32_Safe_Torque_nm = f32_Ref_Torque;
            }
            else{
                st_Inverter.st_Refs.f32_Torque_nm = st_Inverter.st_Parameters.f32_Maximun_Foward_Torque_nm;
                st_Inverter.st_Refs.f32_Safe_Torque_nm = st_Inverter.st_Parameters.f32_Maximun_Foward_Torque_nm;
            }
        }
        else{
            if(f32_Ref_Torque >= st_Inverter.st_Parameters.f32_Maximun_Regen_Torque_nm){
                st_Inverter.st_Refs.f32_Torque_nm = f32_Ref_Torque;
                st_Inverter.st_Refs.f32_Safe_Torque_nm = f32_Ref_Torque;
            }
            else{
                st_Inverter.st_Refs.f32_Torque_nm = st_Inverter.st_Parameters.f32_Maximun_Regen_Torque_nm;
                st_Inverter.st_Refs.f32_Safe_Torque_nm = st_Inverter.st_Parameters.f32_Maximun_Regen_Torque_nm;
            }
        }
    }

    if(u8_Gear_Position == GEAR_REVERSE && st_Inverter.un_State.bits.b4_Operational_State == INV_STATE_OPERATIONAL){
        f32_Ref_Torque = st_Inverter.st_Parameters.f32_Reverse_Torque_Gain_nm * Throttle_Get_Thottle_pu();
        f32_Ref_Torque = Inverter_get_Reverse_Speed_derrating_pu() * f32_Ref_Torque;

        if(Fusi_get_No_Pressure() && (st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm <= 0)){
            if(f32_Ref_Torque < 0){
                f32_Ref_Torque = 0;
            }
        }

        if(st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm > 100){
            if(f32_Ref_Torque > 0){
                f32_Ref_Torque = 0;
            }
        }

        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State = INV_REQ_STATE_OPERATIONAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_EV;
        st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode = INV_REQ_CMD_MODE_TORQUE;


        if(f32_Ref_Torque >= st_Inverter.st_Parameters.f32_Maximun_Reverse_Torque_nm){
            st_Inverter.st_Refs.f32_Torque_nm = f32_Ref_Torque;
            st_Inverter.st_Refs.f32_Safe_Torque_nm = f32_Ref_Torque;
        }
        else{
            st_Inverter.st_Refs.f32_Torque_nm = st_Inverter.st_Parameters.f32_Maximun_Reverse_Torque_nm;
            st_Inverter.st_Refs.f32_Safe_Torque_nm = st_Inverter.st_Parameters.f32_Maximun_Reverse_Torque_nm;
        }

    }

    //Security Check
    if((u8_Gear_Position != GEAR_DIRECT) && (u8_Gear_Position != GEAR_REVERSE) && (u8_Gear_Position != GEAR_NEUTRAL)){
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State = INV_REQ_STATE_OPERATIONAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode = INV_REQ_MODE_NEUTRAL;
        st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode = INV_REQ_CMD_MODE_TORQUE;
        st_Inverter.st_Refs.f32_Torque_nm = 0;
        st_Inverter.st_Refs.f32_Safe_Torque_nm = 0;
        st_Inverter.st_Refs.s16_Speed_rpm = 0;
        st_Inverter.st_Refs.s16_Safe_Speed_rpm = 0;
    }

    x_os_mutex_release(&t_Mutex_Inverter);
}

uint8 Inverter_Get_Error_Flags(void){
    return st_Inverter.st_Fauts.un_Flags.all;
}
uint16 Inverter_Get_Error_Code(void){
    return st_Inverter.st_Fauts.u16_Inv_Event_Id;
}

float32 Inverter_Get_Auxiliary_Voltage_V(void){
    return st_Inverter.st_Electrical.f32_Auxiliary_Voltage_V;
}

float32 Inverter_Get_Avg_Current_A(void){
    return st_Inverter.st_Electrical.f32_Avg_Current_A;
}

float32 Inverter_Get_Avg_Torque_nm(void){
    return st_Inverter.st_Mechanical.f32_Avg_Torque_nm;
}

float32 Inverter_Get_Max_Torque_nm(void){
    return st_Inverter.st_Mechanical.f32_Max_Meesured_Torque_nm;
}

float32 Inverter_Get_Min_Torque_nm(void){
    return st_Inverter.st_Mechanical.f32_Min_Meesured_Torque_nm;
}

sint8 Inverter_Get_Avg_Speed_kmh(void){
    return st_Inverter.st_Mechanical.f32_Avg_Speed_kmh;
}

sint8 Inverter_Get_Max_Speed_kmh(void){
    return st_Inverter.st_Mechanical.s8_Max_Speed_kmh;
}

void Inverter_Reset_Max_Torque_nm(void){
    if(x_os_mutex_get(&t_Mutex_Inverter,10) == C_NO_ERR){
        st_Inverter.st_Mechanical.f32_Max_Meesured_Torque_nm = -3000;
        x_os_mutex_release(&t_Mutex_Inverter);
    }
}

void Inverter_Reset_Min_Torque_nm(void){
    if(x_os_mutex_get(&t_Mutex_Inverter,10) == C_NO_ERR){
        st_Inverter.st_Mechanical.f32_Min_Meesured_Torque_nm = 3000;
        x_os_mutex_release(&t_Mutex_Inverter);
    }
}

void Inverter_Reset_Max_Speed_kmh(void){
    if(x_os_mutex_get(&t_Mutex_Inverter,10) == C_NO_ERR){
        st_Inverter.st_Mechanical.s8_Max_Speed_kmh = 0;
        x_os_mutex_release(&t_Mutex_Inverter);
    }
}
