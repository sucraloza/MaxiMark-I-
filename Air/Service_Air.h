#ifndef SERVICE_AIR_H_INCLUDED
#define SERVICE_AIR_H_INCLUDED

#define SERVICE_AIR_NOMINAL_SPEED_RPM 3000
#define SERVICE_AIR_MINIMUN_SPEED_RPM 2000

// ------- Variables ------

typedef enum
{
    SERVICE_AIR_MQ_SLEEP = 0x0,
    SERVICE_AIR_MQ_WAIT = 0x01,
    SERVICE_AIR_MQ_START = 0x02,
    SERVICE_AIR_MQ_RUN = 0x03,
    SERVICE_AIR_MQ_STOP = 0x04,
    SERVICE_AIR_MQ_FAULT = 0x05,
} E_Service_Air_MQ_Status;

typedef enum
{
    SERVICE_AIR_OFF = 0x0,
    SERVICE_AIR_ON = 0x01,
    SERVICE_AIR_ERROR = 0x03,
} E_Service_Air_Status;


typedef struct{
    E_Service_Air_Status e_Status;
    uint16 u16_Speed_rpm;
} T_Service_Air_Feedback;

typedef struct{
    uint8 u8_Enable;
    uint16 u16_Speed_rpm;
} T_Service_Air_Control;

typedef struct{
    T_Service_Air_Feedback t_Feedback;
    T_Service_Air_Control t_Refs;
    E_Service_Air_MQ_Status e_Mq_State;
    uint8 u8_Mq_ref;
    uint8 u8_Start_Error;
    uint32 u32_Timeout;
} T_Service_Air_Vars;

// ------- Public --------
extern void Service_Air_Init(void);
extern void Service_Air_Set_Ref(uint8 u8_ref);
extern void Service_Air_Monitoring(T_x_can_msg * pt_Msg);
extern void Service_Air_set_Feedback(uint16 u16_Speed_rpm, E_Service_Air_Status e_Status);
extern uint8 Service_Air_Get_Enable(void);
extern uint16 Service_Air_Get_Speed(void);
extern void Service_Air_Run(void);
// ------- Private --------


#endif // SERVICE_AIR_H_INCLUDED
