#ifndef AC_COMPRESSOR_H_INCLUDED
#define AC_COMPRESSOR_H_INCLUDED

#define AC_COMP_NOMINAL_SPEED_RPM 6000
#define AC_COMP_MINIMUN_SPEED_RPM 3000
#define AC_COMP_COUNTER_LIMIT 10

// ------- Public --------
extern void AC_Compressor_Init(uint8 u8_CAN_Chanel);
extern void AC_Comp_Set_Enable(uint8 u8_ref);
extern void AC_Comp_Send(void);
extern void AC_Comp_Recived(void);
extern void AC_Comp_Feedback_Interface(T_x_can_msg * pt_Msg);
extern void AC_Comp_Run(void);
extern void AC_Comp_Monitoring(T_x_can_msg * pt_Msg);
// ------- Private --------

void AC_Comp_RX_Feedback(T_x_can_msg * pt_Msg);
void AC_Comp_TX_Control(void);

// ------- Variables ------

typedef enum
{
    AC_COMP_MQ_SLEEP = 0x0,
    AC_COMP_MQ_WAIT = 0x01,
    AC_COMP_MQ_START = 0x02,
    AC_COMP_MQ_RUN = 0x03,
    AC_COMP_MQ_STOP = 0x04,
    AC_COMP_MQ_FAULT = 0x05,
} E_AC_Comp_MQ_Status;

typedef enum
{
    AC_COMP_OFF = 0x0,
    AC_COMP_ON = 0x01,
    AC_COMP_ERROR = 0x03,
} E_AC_Comp_Status;

typedef enum
{
    AC_COMP_NO_ERROR = 0x0,
    AC_COMP_OVER_CURRENT = 0x01,
    AC_COMP_OVER_VOLTAGE = 0x02,
    AC_COMP_UNDER_VOLTAGE = 0x04,
    AC_COMP_OVER_TEMPERATURE = 0x08,
    AC_COMP_COMUNICATION_ERROR = 0x10,
    AC_COMP_START_ERROR = 0x20,
} E_AC_Comp_Errors;

typedef struct{
    uint8 b1_Over_Current:1;
    uint8 b1_Over_Voltage:1;
    uint8 b1_Under_Voltage:1;
    uint8 b1_Over_Temperature:1;
    uint8 b1_Comunication_Error:1;
    uint8 b1_Start_Error:1;
    uint8 rsv:3;
} T_AC_Comp_Erros;

typedef union{
    uint8 all;
    T_AC_Comp_Erros bits;
} U_AC_Comp_Erros;

typedef struct{
    U_AC_Comp_Erros u_Errors;
    E_AC_Comp_Status e_Status;
    sint8 u8_Temp_C;
    uint16 u16_Speed_rpm;
    uint16 u16_Input_Voltage_V;
    float32 f32_Max_Power_kW;
    float32 f32_Input_Current_A;
} T_AC_Comp_Feedback;

typedef struct{
    uint8 u8_Enable;
    uint16 u16_Speed_rpm;
} T_AC_Comp_Control;

typedef struct{
    T_AC_Comp_Feedback t_Feedback;
    T_AC_Comp_Control t_Refs;
    E_AC_Comp_MQ_Status e_Mq_State;
    uint8 u8_Mq_ref;
    uint8 u8_Start_Error;
    uint8 u8_Counter;
    uint32 u32_Timeout;
} T_AC_Comp_Vars;


#endif // AC_COMPRESSOR_H_INCLUDED
