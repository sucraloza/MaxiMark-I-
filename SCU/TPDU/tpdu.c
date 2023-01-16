#include "tpdu.h"

#define TRANSITION_DELAY ((uint32) 100)

E_TPDU_State e_State;
T_x_can_msg t_Msg_TPDU;

void TPDU_SetState_Off(void);
void TPDU_SetState_AC(void);
void TPDU_SetState_Comp(void);
void TPDU_SetState_WaitComp(uint8 u8_RelayComp, uint8 u8_RelayAC);
void TPDU_SetState_WaitAC(uint8 u8_RelayComp, uint8 u8_RelayAC);

void TPDU_OnState_Off(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs);
void TPDU_OnState_AC(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs);
void TPDU_OnState_Comp(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs);
void TPDU_OnState_WaitComp(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs);
void TPDU_OnState_WaitAC(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs);

uint32 u32_Timeout;
uint8 u8_IsWaiting = FALSE;

void TPDU_Init(void)
{
    e_State = TPDU_OFF;

    t_Msg_TPDU.t_Id.u32_Id = 0x18148814;
    t_Msg_TPDU.t_Id.u8_Xtd = TRUE;
    t_Msg_TPDU.u8_Dlc = 8;
    t_Msg_TPDU.au8_Data[0] = (uint8) e_State;
    t_Msg_TPDU.au8_Data[1] = 0;
    t_Msg_TPDU.au8_Data[2] = 0;
    t_Msg_TPDU.au8_Data[3] = 0;
    t_Msg_TPDU.au8_Data[4] = 0;
    t_Msg_TPDU.au8_Data[5] = 0;
    t_Msg_TPDU.au8_Data[6] = 0;
    t_Msg_TPDU.au8_Data[7] = 0;

}

uint8 TPDU_Run_StateMachine(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs)
{
    switch (e_State)
    {
        case TPDU_OFF: TPDU_OnState_Off(t_Inputs, pt_Outputs); break;
        case TPDU_WAIT_COMP: TPDU_OnState_WaitComp(t_Inputs, pt_Outputs); break;
        case TPDU_COMP_ON: TPDU_OnState_Comp(t_Inputs, pt_Outputs); break;
        case TPDU_WAIT_AC: TPDU_OnState_WaitAC(t_Inputs, pt_Outputs); break;
        case TPDU_AC_ON: TPDU_OnState_AC(t_Inputs, pt_Outputs); break;
    }
    TPDU_Config_Monitoring_msg(&t_Msg_TPDU,t_Inputs,*pt_Outputs);
    return FALSE;
}

/** Getters */
E_TPDU_State TPDU_Get_State(void)
{
    return e_State;
}

T_x_can_msg TPDU_Get_Monitoring(void)
{
    return t_Msg_TPDU;
}



/** Private functions */
void TPDU_SetState_Off(void)
{
    e_State = TPDU_OFF;
}

void TPDU_SetState_AC(void)
{
    e_State = TPDU_AC_ON;
}

void TPDU_SetState_Comp(void)
{
    e_State = TPDU_COMP_ON;
}

void TPDU_SetState_WaitComp(uint8 u8_RelayComp, uint8 u8_RelayAC)
{
    if (u8_RelayComp && u8_RelayAC) return;

    GPIO_set_TPDU_AC(DS_ON * u8_RelayAC);
    GPIO_set_TPDU_Compressor(DS_ON * u8_RelayComp);

    u32_Timeout = Timeout_Set_ms(TRANSITION_DELAY);
    e_State = TPDU_WAIT_COMP;
}

void TPDU_SetState_WaitAC(uint8 u8_RelayComp, uint8 u8_RelayAC)
{
    if (u8_RelayComp && u8_RelayAC) return;

    GPIO_set_TPDU_AC(DS_ON * u8_RelayAC);
    GPIO_set_TPDU_Compressor(DS_ON * u8_RelayComp);

    u32_Timeout = Timeout_Set_ms(TRANSITION_DELAY);
    e_State = TPDU_WAIT_AC;
}

void TPDU_OnState_Off(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs)
{
    if (t_Inputs.u8_Compressor && t_Inputs.u8_DisengageEnabled)
    {
        TPDU_SetState_WaitComp(X_ON, X_OFF);
    }
    else if (!t_Inputs.u8_Compressor && t_Inputs.u8_AC && t_Inputs.u8_DisengageEnabled)
    {
        TPDU_SetState_WaitAC(X_OFF, X_ON);
    }

    pt_Outputs->u8_LowerSpeed = (!t_Inputs.u8_DisengageEnabled && (t_Inputs.u8_Compressor || t_Inputs.u8_AC));
    pt_Outputs->u8_Timeout = FALSE;
}

void TPDU_OnState_AC(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs)
{
    pt_Outputs->u8_LowerSpeed = (!t_Inputs.u8_DisengageEnabled && (t_Inputs.u8_Compressor || !t_Inputs.u8_AC));
    pt_Outputs->u8_Timeout = TRUE;

    if ((t_Inputs.u8_Compressor || !t_Inputs.u8_AC) && t_Inputs.u8_DisengageEnabled)
    {
        TPDU_SetState_WaitAC(X_OFF, X_OFF);
        pt_Outputs->u8_Timeout = FALSE;
    }
}

void TPDU_OnState_Comp(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs)
{
    pt_Outputs->u8_LowerSpeed = (!t_Inputs.u8_DisengageEnabled && !t_Inputs.u8_Compressor);
    pt_Outputs->u8_Timeout = TRUE;

    if (!t_Inputs.u8_Compressor && t_Inputs.u8_DisengageEnabled)
    {
        TPDU_SetState_WaitComp(X_OFF, X_OFF);
        pt_Outputs->u8_Timeout = FALSE;
    }
}

void TPDU_OnState_WaitComp(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs)
{
    pt_Outputs->u8_LowerSpeed = FALSE;
    pt_Outputs->u8_Timeout = FALSE;

    uint8 u8_timeout = Timeout_Check(u32_Timeout);
    if (!u8_timeout) return;

    if (t_Inputs.u8_Compressor) TPDU_SetState_Comp();
    else TPDU_SetState_Off();
}

void TPDU_OnState_WaitAC(T_TPDU_Inputs t_Inputs, T_TPDU_Outputs* pt_Outputs)
{
    pt_Outputs->u8_LowerSpeed = FALSE;
    pt_Outputs->u8_Timeout = FALSE;

    uint8 u8_timeout = Timeout_Check(u32_Timeout);
    if (!u8_timeout) return;

    if (t_Inputs.u8_AC && !t_Inputs.u8_Compressor) TPDU_SetState_AC();
    else TPDU_SetState_Off();
}

void TPDU_Config_Monitoring_msg(T_x_can_msg * t_Msg, T_TPDU_Inputs t_Inputs, T_TPDU_Outputs t_Outputs){
    (*t_Msg).u8_Dlc = 8;
    (*t_Msg).au8_Data[0] = (uint8) e_State;
    (*t_Msg).au8_Data[1] = t_Inputs.u8_AC;
    (*t_Msg).au8_Data[2] = t_Inputs.u8_Compressor;
    (*t_Msg).au8_Data[3] = t_Inputs.u8_DisengageEnabled;
    (*t_Msg).au8_Data[4] = t_Outputs.u8_LowerSpeed;
    (*t_Msg).au8_Data[5] = t_Outputs.u8_Timeout;
    (*t_Msg).au8_Data[6] = 0;
    (*t_Msg).au8_Data[7] = 0;
}
