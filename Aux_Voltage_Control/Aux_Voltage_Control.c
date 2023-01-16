#include "OS_Defs.h"
#include "Aux_Voltage_Control.h"

T_AVC_Vars t_AVC;
T_x_os_mutex t_Mutex_AVC;

void AVC_Init(void){
    t_AVC.t_Limits.u8_Max_Current_A = AVC_MAX_CURRENT_A;
    t_AVC.t_Limits.u8_Under_Voltage_V = AVC_UNDER_VOLTAGE_LIMIT;
    t_AVC.t_Feedback.f32_Voltage_V = 0;
    t_AVC.t_Feedback.u8_Current_A = 0;
    t_AVC.t_Feedback.u8_Filtered_Current_A = 0; 
    t_AVC.t_Refs.f32_Voltage_V = 0;
    t_AVC.t_Refs.u8_Enable = FALSE;
    t_AVC.u16_DTC = 0;

    s16_Error = x_os_mutex_init(&t_Mutex_AVC);
    if(s16_Error != C_NO_ERR) Error_Beep();
}


sint16 AVC_Set_Enable(uint8 u8_Enable){
    sint16 s16_Error;
    s16_Error = x_os_mutex_get(&t_Mutex_AVC,10);
    if(s16_Error != C_NO_ERR) return s16_Error;

    if(u8_Enable) t_AVC.t_Refs.u8_Enable = DS_ON;
    else t_AVC.t_Refs.u8_Enable = DS_OFF;
    x_os_mutex_release(&t_Mutex_AVC)
    return 0;
}

sint16 AVC_Set_Voltage_Ref_V(float32 f32_Ref_V){
    sint16 s16_Error;
    s16_Error = x_os_mutex_get(&t_Mutex_AVC,10);
    if(s16_Error != C_NO_ERR) return s16_Error;

    if(f32_Ref_V > AVC_MAX_AUX_VOLTAGE_V)  t_AVC.t_Refs.f32_Voltage_V = AVC_MAX_AUX_VOLTAGE_V;
    else t_AVC.t_Refs.f32_Voltage_V = f32_Ref_V;
    x_os_mutex_release(&t_Mutex_AVC)
    return 0;
}

sint16 AVC_Set_Under_Voltage_Limit_V(uint8 u8_Limit_V){
    sint16 s16_Error;
    s16_Error = x_os_mutex_get(&t_Mutex_AVC,10);
    if(s16_Error != C_NO_ERR) return s16_Error;

    t_AVC.t_Limits.u8_Under_Voltage_V = u8_Limit_V;
    x_os_mutex_release(&t_Mutex_AVC)
    return 0;
}

sint16 AVC_Set_Max_Current_A(uint8 u8_Ref_A){
    sint16 s16_Error;
    s16_Error = x_os_mutex_get(&t_Mutex_AVC,10);
    if(s16_Error != C_NO_ERR) return s16_Error;

    t_AVC.t_Limits.u8_Max_Current_A = u8_Ref_A;
    x_os_mutex_release(&t_Mutex_AVC)
    return 0;
}


float32 AVC_Get_Act_Voltage_V(void){ return t_AVC.t_Feedback.f32_Voltage_V; }
uint8 AVC_Get_Act_Current_A(void){ return t_AVC.t_Feedback.u8_Current_A; }
float32 AVC_Get_Filtered_Voltage_V(void){ return Filter_get_Output(&t_AVC.t_Voltage_Filter); }
float32 AVC_Get_Filtered_Current_A(void){ return Filter_get_Output(&t_AVC.t_Current_Filter); }
uint8 AVC_Get_Act_Fieltered_Current_A(void){ return t_AVC.t_Feedback.u8_Filtered_Current_A}
float32 AVC_Get_Ref_Voltage_V(void){ return t_AVC.t_Refs.f32_Voltage_V; }
uint8 AVC_Get_Enable(void){ return (t_AVC.t_Refs.u8_Enable == DS_ON)}
uint8 AVC_Get_Max_Current_A(void){ return t_AVC.t_Limits.u8_Max_Current_A; }
uint8 AVC_Get_Under_Voltage(void){ return t_AVC.t_Limits.u8_Under_Voltage_V; }
uint8 AVC_Get_CAN_Failure(void);
uint16 AVC_Get_DTC(void){ return t_AVC.u16_DTC; }
uint16 AVC_Get_Under_Voltage_Failure(void){ return (t_AVC.t_Feedback.f32_Voltage_V < t_AVC.t_Limits.u8_Under_Voltage_V); }
