#ifndef MONITORING_VARS_H_INCLUDED
#define MONITORING_VARS_H_INCLUDED

typedef struct {
    uint16 u16_PNG_Send_Turn;
    uint8 u8_Fix_Message_Flag;
    uint16 u16_Fixed_PNG;
    uint8 u8_Number_Turns_per_Cycle;
    uint8 u8_Auxiliary_ECU_State;
    uint8 u8_vel;
} St_Monitoring_Control_1;

#define INIT_MONITORING_CONTROL(v)      \
    v.u16_PNG_Send_Turn = 0;            \
    v.u8_Fix_Message_Flag = 0;          \
    v.u16_Fixed_PNG = 0;                \
    v.u8_Number_Turns_per_Cycle = 2;    \
    v.u8_Auxiliary_ECU_State = 0;

// New
typedef struct {
    uint8 u8_BTMS_OnOff;
    uint8 u8_BTMS_Work_Status;
    uint8 u8_BTMS_Ref_Temp;
    uint8 u8_BTMS_Manual_Control;
} St_Monitoring_BTMS;

#define INIT_BTMS_MONITORING(v)         \
    v.u8_BTMS_OnOff = 0;                \
    v.u8_BTMS_Work_Status = 0;          \
    v.u8_BTMS_Ref_Temp = 25;            \
    v.u8_BTMS_Manual_Control = 0;

typedef struct
{
  St_Monitoring_Control_1 st_Monitoring_Control_1;
  St_Monitoring_BTMS st_Monitoring_BTMS;
} St_Monitoring_Control;


#define MONITORING_INIT(v)            \
    INIT_MONITORING_CONTROL(v.st_Monitoring_Control_1);    \
    INIT_BTMS_MONITORING(v.st_Monitoring_BTMS);            \



extern St_Monitoring_Control st_Monitoring;

#endif // MONITORING_VARS_H_INCLUDED
