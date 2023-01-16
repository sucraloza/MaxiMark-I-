#ifndef GLOBAL_VARS_H_INCLUDED
#define GLOBAL_VARS_H_INCLUDED


// Universal Controls

typedef struct{
    float32 f32_Gain_A0;
    float32 f32_Gain_A1;
    float32 f64_Gain_B1;
    float64 f64_Y1;
    float32 f32_X1;
    float32 f64_GX;
    float64 f64_GY;
} St_Filter;

typedef struct{
    uint8 u8_ECU_State;
    uint8 u8_Drive_Block;
    uint8 u8_Charge_Lock;
    uint8 u8_Charge_State_Detected;
    uint8 u8_IsRegen;
    uint8 u8_Drivers_Feedback;
    uint8 u8_Compresor_Detection;
} St_System_State;

#define INIT_SYSTEM_STATE(v)        \
    v.u8_ECU_State = 0;             \
    v.u8_Drive_Block = 0;           \
    v.u8_Charge_Lock = 0;           \
    v.u8_Charge_State_Detected = 0; \
    v.u8_IsRegen = FALSE;           \
    v.u8_Drivers_Feedback = 0;      \
    v.u8_Compresor_Detection = 0;

typedef struct{
    uint8 b1_PDU_Circuit:1;
    uint8 b1_Isolation_Timeout:1;
    uint8 b1_Isolation_To_Gnd:1;
    uint8 b1_Isolation_Alarm:1;
    uint8 b1_Inverter_Protocol:1;
    uint8 b1_Battery_Timeout:1;
    uint8 b1_Close_Timeout:1;
    uint8 b1_free8:1;
} St_Power_On_Faults;

typedef union{
    uint8 all;
    St_Power_On_Faults bits;
} Un_Power_On_Faults;

typedef struct{
    uint8 b1_Inverter_Standby:1;
    uint8 b1_Current_Drop:1;
    uint8 b1_Open_Timeout:1;
    uint8 b1_free4:1;
    uint8 b1_free5:1;
    uint8 b1_free6:1;
    uint8 b1_free7:1;
    uint8 b1_free8:1;
} St_Power_Off_Faults;

typedef union{
    uint8 all;
    St_Power_Off_Faults bits;
} Un_Power_Off_Faults;


typedef struct{
    uint8 u8_Power_On_State;
    uint8 u8_Power_Off_State;
    uint32 u32_Start_Time_ms;
    uint32 u32_Time_Out_ms;
    Un_Power_On_Faults un_On_Faults;
    Un_Power_Off_Faults un_Off_Faults;
} St_System_Power_OnOff;

#define INIT_SYSTEM_POWER_ONOFF(v)       \
    v.u8_Power_On_State = 0;             \
    v.u8_Power_Off_State = 0;            \
    v.u32_Start_Time_ms = 0;             \
    v.u32_Time_Out_ms = 0;               \
    v.un_On_Faults.all = 0;              \
    v.un_Off_Faults.all = 0;

typedef struct{
    uint8 b1_Free:8;
} St_Vehicle_On_Faults;

typedef union{
    uint8 all;
    St_Vehicle_On_Faults bits;
} Un_Vehicle_On_Faults;

typedef struct{
    uint8 b1_Free:8;
} St_Vehicle_Off_Faults;

typedef union{
    uint8 all;
    St_Vehicle_Off_Faults bits;
} Un_Vehicle_Off_Faults;

typedef struct {
    uint8 u8_Vehicle_On_State;
    uint8 u8_Vehicle_Off_State;
    uint32 u32_Start_Time_ms;
    uint32 u32_Time_Out_ms;
    Un_Vehicle_On_Faults un_On_Faults;
    Un_Vehicle_Off_Faults un_Off_Faults;
} St_System_Vehicle_OnOff;

#define INIT_SYSTEM_VEHICLE_ONOFF(v)        \
    v.u8_Vehicle_On_State = 0;              \
    v.u8_Vehicle_Off_State = 0;             \
    v.u32_Start_Time_ms = 0;                \
    v.u32_Time_Out_ms = 0;                  \
    v.un_On_Faults.all = 0;                 \
    v.un_Off_Faults.all = 0;


#define INIT_SYSTEM_IGN_FILTER(v)       \
    v.f32_Gain_A0 = 0.02f;              \
    v.f32_Gain_A1 = 0.0f;               \
    v.f64_Gain_B1 = -0.98f;             \
    v.f32_X1 = 0.0f;                    \
    v.f64_Y1 = 0.0f;                    \
    v.f64_GX = 0.0f;                    \
    v.f64_GY = 0.0f;

typedef struct{
    uint8 u8_Reference;
    uint8 u8_Feedback;
    uint8 u8_Operation;
    St_Filter st_Filter;
}St_HVIL_Variables;

#define INIT_SYSTEM_HVIL_Filter(v)      \
    v.f32_Gain_A0 = 0.10f;              \
    v.f32_Gain_A1 = 0.0f;               \
    v.f64_Gain_B1 = -0.90f;             \
    v.f32_X1 = 0.0f;                    \
    v.f64_Y1 = 0.0f;                    \
    v.f64_GX = 0.0f;                    \
    v.f64_GY = 0.0f;

#define INIT_SYSTEM_HVIL(v)                 \
    v.u8_Reference =  0;                    \
    v.u8_Feedback = 0;                      \
    v.u8_Operation = 0;                     \
    INIT_SYSTEM_HVIL_Filter(v.st_Filter)

typedef struct{
    St_System_State st_State;
    St_System_Power_OnOff st_Power_OnOff;
    St_System_Vehicle_OnOff st_Vehicle_OnOff;
    St_Filter st_Ign_Filter;
    St_HVIL_Variables st_HVIL;
} St_System_Variables;

#define INIT_VEHICLE_SYSTEMS(v)                 \
    INIT_SYSTEM_STATE(v.st_State)               \
    INIT_SYSTEM_POWER_ONOFF(v.st_Power_OnOff)   \
    INIT_SYSTEM_IGN_FILTER(v.st_Ign_Filter)     \
    INIT_SYSTEM_HVIL(v.st_HVIL)

extern St_System_Variables st_System;
extern T_x_os_mutex t_Mutex_System;

//DEBUGGING
typedef struct{
    uint8 flag;
    uint8 u8_debug1;
    uint8 u8_debug2;
    uint8 u8_debug3;
    uint8 u8_debug4;
    uint8 u8_debug5;
    uint8 u8_debug6;
    uint8 u8_debug7;
}debugging;

#define INIT_DEBUGGING(v)                           \
    v.flag = 0;                                     \
    v.u8_debug1 = 0;                                   \
    v.u8_debug2 = 0;                                   \
    v.u8_debug3 = 0;                                   \
    v.u8_debug4 = 0;                                   \
    v.u8_debug5 = 0;                                   \
    v.u8_debug6 = 0;                                   \
    v.u8_debug7 = 0;

extern debugging debuggeo; 

#endif // INTERRUPTS_H_INCLUDED
