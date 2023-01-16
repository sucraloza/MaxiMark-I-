#ifndef TPDU_H_INCLUDED
#define TPDU_H_INCLUDED

#include "OS_Defs.h"

#define TPDU_DISENGAGE_MAX_POWER_KW (0.01f)

typedef enum
{
    TPDU_OFF = 0x0,
    TPDU_AC_ON = 0x1,
    TPDU_COMP_ON = 0x2,
    TPDU_WAIT_COMP = 0x3,
    TPDU_WAIT_AC = 0x4,
} E_TPDU_State;

typedef struct
{
    uint8 u8_LowerSpeed;
    uint8 u8_Timeout;
} T_TPDU_Outputs;

typedef struct
{
    uint8 u8_Compressor;
    uint8 u8_AC;
    uint8 u8_DisengageEnabled;
} T_TPDU_Inputs;

void TPDU_Init(void);

uint8 TPDU_Run_StateMachine(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs);
void TPDU_Config_Monitoring_msg(T_x_can_msg * t_Msg, T_TPDU_Inputs t_Inputs, T_TPDU_Outputs t_Outputs);

/** Getters */
E_TPDU_State TPDU_Get_State(void);
T_x_can_msg TPDU_Get_Monitoring(void);


#endif /* TPDU_H_INCLUDED */
