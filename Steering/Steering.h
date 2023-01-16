#ifndef STEERING_VARS_H_INCLUDED
#define STEERING_VARS_H_INCLUDED

#define STR_STATE_OFF           0x00
#define STR_STATE_ENABLE        0x03
#define STR_STATE_CLOSE         0x05
#define STR_STATE_OPERATION     0x06
#define STR_STATE_DISABLE       0x09
#define STR_STATE_FAULT         0x0F

#define STEERING_RX_MASK    0x000000FFu
#define STEERING_TX_MASK    0x000000FEu

typedef struct
{
    uint8 b2_CAN_Send:2;
    uint8 b2_CAN_Recived:2;
    uint8 rsv:4;
} T_Steering_Errors;

typedef union
{
    uint8 all;
    T_Steering_Errors bits;
} U_Steering_Errors;


typedef struct{
    U_Steering_Errors u_Errors;
    uint8 u8_State;
    uint8 u8_On;
    uint8 u8_T15_Ref;
    uint8 u8_Contactor_Ref;
    uint32 u32_Timeout_ms;
    uint16 u16_Speed_kmh;
} T_Steering_Vars;

extern void Steering_Init(void);

extern void Steering_Set_Vehicle_Speed_kmh(sint16 s16_Speed_kmh);

extern void Steering_Send_Msg(void);
extern void Steering_Recived_Msg(void);

extern void Steering_Machine(void);

extern inline void Steering_set_On(void);
extern inline void Steering_set_Off(void);
extern uint8 Steering_is_On(void);

extern void Steering_Config_Monitoring_msg(T_x_can_msg * t_Msg);

#endif // STEERING_VARS_H_INCLUDED
