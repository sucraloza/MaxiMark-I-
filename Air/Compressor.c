#include "OS_Defs.h"
#include "Compressor.h"

T_Compressor_Vars t_Compressor;


void Compressor_Init(void){
    t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
    t_Compressor.u8_Charge_Air = FALSE;
    t_Compressor.u8_Fault_Leak = DS_OFF;
    t_Compressor.u8_Fault_Motor = DS_OFF;
    t_Compressor.u8_OverHeat = DS_OFF;
    t_Compressor.u8_Compresor_Ok = DS_OFF;
    t_Compressor.u16_Air_Pressure_mbar = 0;
    t_Compressor.s16_Speed_rpm = 0;
    t_Compressor.u32_Timeout = 0;
    t_Compressor.s8_Temperatura_C = 0;
}

uint8 Compressor_get_Ok(void)
{
    return t_Compressor.u8_Compresor_Ok;
}
uint8 Compressor_get_State(void)
{
    return t_Compressor.u8_State;
}
uint8 Compressor_get_Charge_Air(void)
{
    return t_Compressor.u8_Charge_Air;
}
uint8 Compressor_get_Fault_Leak(void)
{
    return (t_Compressor.u8_Fault_Leak == DS_ON);
}
uint8 Compressor_get_Fault_Motor(void)
{
    return (t_Compressor.u8_Fault_Motor == DS_ON);
}
uint8 Compressor_get_Over_Heat(void)
{
    return (t_Compressor.u8_OverHeat == DS_ON);
}

void Compresor_Monitor(uint32 u32_ID, void* oppv_Mailbox)
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = u32_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = t_Compressor.u8_State;
    t_Msg.au8_Data[1] = t_Compressor.u8_Charge_Air;

    t_Msg.au8_Data[2] = (t_Compressor.u8_Fault_Leak & 0x3) << 6;
    t_Msg.au8_Data[2] |= (t_Compressor.u8_Fault_Motor & 0x3) << 4;
    t_Msg.au8_Data[2] |= (t_Compressor.u8_OverHeat & 0x3) << 2;
    t_Msg.au8_Data[2] |= (t_Compressor.u8_Compresor_Ok & 0x3) << 0;

    t_Msg.au8_Data[3] = (uint8) t_Compressor.s8_Temperatura_C + 40;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(oppv_Mailbox, &t_Msg);
}

void Compressor_Run(void){
    t_Compressor.u16_Air_Pressure_mbar = Jarvis_get_Pressure_mBar();
    t_Compressor.s16_Speed_rpm = SCU_get_DCU_Speed_rpm();

    if(SCU_get_DCU_in_Error())
    {
        t_Compressor.u8_Fault_Motor = DS_ON;
    }
    else
    {
        t_Compressor.u8_Fault_Motor = DS_OFF;
    }

    Compressor_Overheat();

    switch(t_Compressor.u8_State)
    {
        case COMPRESSOR_STATE_DISABLE:
            Compressor_State_Disable();
            break;
        case COMPRESSOR_STATE_COMPRESOR_OK:
            Compressor_State_Compresor_Ok();
            break;
        case COMPRESSOR_STATE_START:
            Compressor_State_Start();
            break;
        case COMPRESSOR_STATE_ON:
            Compressor_State_On();
            break;
        case COMPRESSOR_STATE_BLAST:
            Compressor_State_Blast();
            break;
        case COMPRESSOR_STATE_STANDBY:
            Compressor_State_Standby();
            break;
        case COMPRESSOR_STATE_OVERHEAT:
            Compressor_State_Overheat();
            break;
        case COMPRESSOR_STATE_TIMEOUT:
            Compressor_State_Timeout();
            break;
        case COMPRESSOR_STATE_COMPRESOR_OK_HEAT:
            Compressor_State_Compresor_Ok_Heat();
            break;
        case COMPRESSOR_STATE_HEATING:
            Compressor_State_Heating();
            break;
        case COMPRESSOR_STATE_OFF:
            Compressor_State_Off();
            break;
        case COMPRESSOR_STATE_FAULT:
            Compressor_State_Fault();
            break;
        default:
            t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
            Compressor_State_Disable();
            break;
    }
}

void Compressor_State_Disable(void)
{
    t_Compressor.u8_Charge_Air = FALSE;
    t_Compressor.u8_Fault_Leak = DS_OFF;
    t_Compressor.u8_Compresor_Ok = DS_OFF;

    if(System_get_State() == ECU_STATE_DRIVE)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_COMPRESOR_OK;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_OK_TIMEOUT_MS);
        return;
    }

    if((System_get_State() == ECU_STATE_CHARGE) && System_get_Charge_State_Detected())
    {
        // No pressure reading is performed while charging
        t_Compressor.u8_State = COMPRESSOR_STATE_OFF;
        return;
    }
}

void Compressor_State_Compresor_Ok(void)
{
    // Este estado se puede omitir, derivando desde DISABLE a STANDBY de inmediato
    t_Compressor.u8_Charge_Air = FALSE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.u16_Air_Pressure_mbar < COMPRESSOR_MIN_PRESSURE_MBAR)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_START;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_OK_TIMEOUT_MS);
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }

    t_Compressor.u8_State = COMPRESSOR_STATE_STANDBY;  
}

void Compressor_State_Start(void)
{
    t_Compressor.u8_Charge_Air = TRUE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.s16_Speed_rpm > COMPRESSOR_OFF_SPEED_RPM)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_ON;
        t_Compressor.u8_Compresor_Ok = DS_ON;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_TIMEOUT_MS);
        return;
    }

    if(Timeout_Check(t_Compressor.u32_Timeout))
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_On(void)
{
    t_Compressor.u8_Charge_Air = TRUE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.u16_Air_Pressure_mbar >= COMPRESSOR_OFF_PRESSURE_MBAR)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_BLAST;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_BLAST_MS);
        return;
    }

    if(Timeout_Check(t_Compressor.u32_Timeout))
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_TIMEOUT;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_WAIT_MS);
        return;
    }

    if(t_Compressor.u8_OverHeat == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_OVERHEAT;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Blast(void)
{
    t_Compressor.u8_Charge_Air = TRUE;
    t_Compressor.u8_Fault_Leak = DS_OFF;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(Timeout_Check(t_Compressor.u32_Timeout))
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_STANDBY;
        return;
    }

    if(t_Compressor.u8_OverHeat == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_OVERHEAT;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Standby(void)
{
    t_Compressor.u8_Charge_Air = FALSE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.u16_Air_Pressure_mbar <= COMPRESSOR_MIN_PRESSURE_MBAR)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_ON;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_TIMEOUT_MS);
        return;
    }

    if(t_Compressor.u8_OverHeat == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_OVERHEAT;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Overheat(void)
{
    t_Compressor.u8_Charge_Air = FALSE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.u8_OverHeat == DS_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_STANDBY;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Timeout(void)
{
    t_Compressor.u8_Charge_Air = FALSE;
    t_Compressor.u8_Fault_Leak = DS_ON;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(Timeout_Check(t_Compressor.u32_Timeout))
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_STANDBY;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Compresor_Ok_Heat(void)
{
    t_Compressor.u8_Charge_Air = TRUE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.s16_Speed_rpm > COMPRESSOR_OFF_SPEED_RPM)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_HEATING;
        t_Compressor.u32_Timeout = Timeout_Set_ms(COMPRESSOR_HEAT_MS);
        t_Compressor.u8_Compresor_Ok = DS_ON;
        return;
    }

    if(Timeout_Check(t_Compressor.u32_Timeout))
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Heating(void)
{
    t_Compressor.u8_Charge_Air = TRUE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(Timeout_Check(t_Compressor.u32_Timeout))
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_OFF;
        return;
    }

    if(t_Compressor.u8_OverHeat == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_OFF;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Off(void)
{
    t_Compressor.u8_Charge_Air = FALSE;

    if(System_get_State() == ECU_STATE_POWER_OFF)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_DISABLE;
        return;
    }

    if(t_Compressor.u8_Fault_Motor == DS_ON)
    {
        t_Compressor.u8_State = COMPRESSOR_STATE_FAULT;
        return;
    }
}

void Compressor_State_Fault(void)
{
    t_Compressor.u8_Charge_Air = FALSE;
    t_Compressor.u8_Compresor_Ok = DS_OFF;
}

void Compressor_Overheat(void)
{
    float32 f32_Current_mA;
    float32 f32_Res_Ohm;

    f32_Current_mA = GPIO_get_Compresor_Current_uA() *0.001f;
    f32_Res_Ohm = (GPIO_SUPPLY_COMP_VOLTAGE_mV/f32_Current_mA) - 130;

    t_Compressor.s8_Temperatura_C = (f32_Res_Ohm - 100) / 0.385f;

    if(t_Compressor.s8_Temperatura_C > COMPRESOR_TEMP_ACTIVATION_LIMIT_C)
    {
        t_Compressor.u8_OverHeat = DS_ON;
    }

    if(t_Compressor.s8_Temperatura_C < COMPRESOR_TEMP_CLEAR_LIMIT_C)
    {
        t_Compressor.u8_OverHeat = DS_OFF;
    }

}
