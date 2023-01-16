#ifndef AUX_VOLTAGE_CONTROL_H_INCLUDED
#define AUX_VOLTAGE_CONTROL_H_INCLUDED

#define AVC_UNDER_VOLTAGE_LIMIT 22
#define AVC_MAX_CURRENT_A       80
#define AVC_MAX_AUX_VOLTAGE_V   28

typedef struct{
    uint8 u8_Under_Voltage_V;
    uint8 u8_Max_Current_A;
} T_AVC_Limits;

typedef struct{
    float32 f32_Voltage_V;
    uint8 u8_Current_A;
} T_AVC_Feedback;

typedef struct{
    float32 f32_Voltage_V;
    uint8 u8_Enable;
} T_AVC_Reference;

typedef struct{
    T_AVC_Reference t_Refs;
    T_AVC_Feedback t_Feedback;
    T_AVC_Limits t_Limits;
    uint16 u16_DTC;
    T_Filter_Vars t_Voltage_Filter;
    T_Filter_Vars t_Current_Filter;
} T_AVC_Vars;

extern void AVC_Init(void);

extern sint16 AVC_Set_Enable(uint8 u8_Enable);
extern sint16 AVC_Set_Voltage_Ref_V(float32 f32_Ref_V);
extern sint16 AVC_Set_Under_Voltage_Limit_V(uint8 u8_Limit_V);
extern sint16 AVC_Set_Max_Current_A(uint8 u8_Ref_A);

extern float32 AVC_Get_Act_Voltage_V(void);
extern uint8 AVC_Get_Act_Current_A(void);
extern float32 AVC_Get_Filtered_Voltage_V(void);
extern uint8 AVC_Get_Filtered_Current_A(void);
extern uint8 AVC_Get_Act_Fieltered_Current_A(void);
extern float32 AVC_Get_Ref_Voltage_V(void);
extern uint8 AVC_Get_Enable(void);
extern uint8 AVC_Get_Max_Current_A(void);
extern uint8 AVC_Get_Under_Voltage(void);
extern uint8 AVC_Get_CAN_Failure(void);
extern uint16 AVC_Get_DTC(void);
extern uint16 AVC_Get_Under_Voltage_Failure(void);

#endif /* AUX_VOLTAGE_CONTROL_H_INCLUDED */