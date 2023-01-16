#include "OS_Defs.h"

//Access Functions
void PDU_Set_Open(void){
    if(x_os_mutex_get(&t_Mutex_PDU,10) != C_NO_ERR){
        return;
    }
    st_PDU.PDU_Circuit.u8_Circuit_State_Reference = PDU_CIRCUIT_OPEN;
    x_os_mutex_release(&t_Mutex_PDU);
}
void PDU_Set_Close(void){
    if(x_os_mutex_get(&t_Mutex_PDU,10) != C_NO_ERR){
        return;
    }
    st_PDU.PDU_Circuit.u8_Circuit_State_Reference = PDU_CIRCUIT_CLOSE;
    x_os_mutex_release(&t_Mutex_PDU);
}

uint8 PDU_Is_Open(void){
    uint8 u8_result;
    u8_result = 1;
    if(st_PDU.PDU_Circuit.u8_Circuit_State != PDU_CIRCUIT_OPEN)
        u8_result = 0;
    if(GPIO_get_PDU_Precharge() != DS_OFF)
        u8_result = 0;
    if(GPIO_get_PDU_Contactor_Main() != DS_OFF)
        u8_result = 0;
    if(GPIO_get_PDU_Discharge() != DS_OFF)
        u8_result = 0;
    return u8_result;
}

uint8 PDU_Is_Closed(void){
    uint8 u8_result;
    u8_result = 1;
    if(st_PDU.PDU_Circuit.u8_Circuit_State != PDU_CIRCUIT_CLOSE)
        u8_result = 0;
    if(GPIO_get_PDU_Precharge() != DS_OFF)
        u8_result = 0;
    if(GPIO_get_PDU_Contactor_Main() != DS_ON)
        u8_result = 0;
    if(GPIO_get_PDU_Discharge() != DS_OFF)
        u8_result = 0;
    return u8_result;
}

uint8 PDU_Get_Fault(void){
    uint8 u8_result;
    u8_result = FALSE;
    if(st_PDU.PDU_Circuit.u8_Circuit_State == PDU_CIRCUIT_WELD_FAULT)
        u8_result = TRUE;

    return u8_result;
}

//Machine States
void PDU_Machine_State(void){
    if(x_os_mutex_get(&t_Mutex_PDU,10) != C_NO_ERR){
        return;
    }

    switch(st_PDU.PDU_Circuit.u8_Circuit_State){
        case PDU_CIRCUIT_OPEN:
            GPIO_set_PDU_Precharge(DS_OFF);
            GPIO_set_PDU_Contactor_Main(DS_OFF);
            GPIO_set_PDU_Discharge(DS_OFF);
            if(PDU_Is_Open() == TRUE && st_PDU.PDU_Circuit.u8_Circuit_State_Reference == PDU_CIRCUIT_CLOSE){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_PRECHARGE;
                st_PDU.PDU_Circuit.u8_Precharge_Circuit_Ok =  TRUE;
            }
        break;
        case PDU_CIRCUIT_PRECHARGE:
            CAPL_Send_Inv_Votage_On();

            GPIO_set_PDU_Precharge(DS_ON);
            GPIO_set_PDU_Contactor_Main(DS_OFF);
            GPIO_set_PDU_Discharge(DS_OFF);
            if(Inverter_Get_Power_Voltage_V() >= st_PDU.PDU_Circuit.u16_Precharge_Circuit_Ok_Voltage_V){
                st_PDU.PDU_Circuit.u8_Precharge_Circuit_Ok =  TRUE;
            }
            else{
                st_PDU.PDU_Circuit.u8_Precharge_Circuit_Ok =  FALSE;
            }
            if(x_sys_get_time_ms() - st_PDU.PDU_Circuit.u32_Initial_Time_ms > st_PDU.PDU_Circuit.u32_Timeout_Precharge_ms){
                if(!st_PDU.PDU_Circuit.u8_Precharge_Circuit_Ok){
                    st_PDU.Errors.bits.b2_No_Voltage = PDU_FAULT_ON;
                }
                st_PDU.Errors.bits.b2_Precharge = PDU_FAULT_ON;
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISENGAGE;
            }
            if(Inverter_Get_Power_Voltage_V() >= st_PDU.PDU_Circuit.u16_Contactor_Engage_Voltage_V){
                st_PDU.Errors.bits.b2_Precharge = PDU_FAULT_OFF;
                st_PDU.Errors.bits.b2_No_Voltage = PDU_FAULT_OFF;
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_BYPASS;
            }
            if(st_PDU.PDU_Circuit.u8_Circuit_State_Reference == PDU_CIRCUIT_OPEN){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISENGAGE;
            }
        break;
        case PDU_CIRCUIT_BYPASS:
            GPIO_set_PDU_Precharge(DS_ON);
            GPIO_set_PDU_Contactor_Main(DS_ON);
            GPIO_set_PDU_Discharge(DS_OFF);
            if(x_sys_get_time_ms() - st_PDU.PDU_Circuit.u32_Initial_Time_ms > st_PDU.PDU_Circuit.u32_Timeout_Bypass_ms){
                st_PDU.Errors.bits.b2_Bypass = PDU_FAULT_ON;
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISENGAGE;
            }
            if(Inverter_Get_Power_Voltage_V() >= st_PDU.PDU_Circuit.u16_Operational_Voltage_V){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_CLOSE;
                st_PDU.Errors.bits.b2_Bypass = PDU_FAULT_OFF;
            }
            if(st_PDU.PDU_Circuit.u8_Circuit_State_Reference == PDU_CIRCUIT_OPEN){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISENGAGE;
            }
        break;
        case PDU_CIRCUIT_CLOSE:
            GPIO_set_PDU_Precharge(DS_OFF);
            GPIO_set_PDU_Contactor_Main(DS_ON);
            GPIO_set_PDU_Discharge(DS_OFF);
            if(System_get_PDU_Current() < 100 && st_PDU.PDU_Circuit.u8_Circuit_State_Reference == PDU_CIRCUIT_OPEN){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISENGAGE;
            }
        break;
        case PDU_CIRCUIT_DISENGAGE:
            CAPL_Send_Inv_Votage_Off();

            GPIO_set_PDU_Precharge(DS_OFF);
            GPIO_set_PDU_Contactor_Main(DS_OFF);
            GPIO_set_PDU_Discharge(DS_OFF);
            if(x_sys_get_time_ms() - st_PDU.PDU_Circuit.u32_Initial_Time_ms > st_PDU.PDU_Circuit.u32_Timeout_Disengage_ms){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISCHARGE;
            }
        break;
        case PDU_CIRCUIT_DISCHARGE:
            GPIO_set_PDU_Precharge(DS_OFF);
            GPIO_set_PDU_Contactor_Main(DS_OFF);
            if(Inverter_Get_Power_Voltage_V() >= st_PDU.PDU_Circuit.u16_Safe_Voltage_V){
                if(x_sys_get_time_ms() - st_PDU.PDU_Circuit.u32_Initial_Time_ms > st_PDU.PDU_Circuit.u32_Timeout_Discharge_ms){
                    GPIO_set_PDU_Discharge(DS_OFF);
                }
                else{
                    GPIO_set_PDU_Discharge(DS_ON);
                }
            }
            if(x_sys_get_time_ms() - st_PDU.PDU_Circuit.u32_Initial_Time_ms > st_PDU.PDU_Circuit.u32_Timeout_Discharge_ms){
                if(Inverter_Get_Power_Voltage_V() >= st_PDU.PDU_Circuit.u16_Contactor_Engage_Voltage_V){
                    st_PDU.Errors.bits.b2_Welded = PDU_FAULT_ON;
                    st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_WELD_FAULT;
                }
                else{
                    st_PDU.Errors.bits.b2_Discharge = PDU_FAULT_ON;
                    if(Inverter_Get_Power_Voltage_V() <= st_PDU.PDU_Circuit.u16_Safe_Voltage_V){
                        st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_OPEN;
                    }
                }
            }
            else{
                if(Inverter_Get_Power_Voltage_V() <= st_PDU.PDU_Circuit.u16_Safe_Voltage_V){
                    st_PDU.Errors.bits.b2_Welded = PDU_FAULT_OFF;
                    st_PDU.Errors.bits.b2_Discharge = PDU_FAULT_OFF;
                    st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_OPEN;
                }
            }
        break;
        case PDU_CIRCUIT_WELD_FAULT:
            GPIO_set_PDU_Precharge(DS_OFF);
            GPIO_set_PDU_Contactor_Main(DS_OFF);
            GPIO_set_PDU_Discharge(DS_OFF);
        break;
        default:
            if(PDU_Is_Closed() == TRUE || Inverter_Get_Power_Voltage_V() > st_PDU.PDU_Circuit.u16_Safe_Voltage_V){
                st_PDU.PDU_Circuit.u32_Initial_Time_ms = x_sys_get_time_ms();
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_DISENGAGE;
            }
            else{
                st_PDU.PDU_Circuit.u8_Circuit_State = PDU_CIRCUIT_OPEN;
            }
    }

    x_os_mutex_release(&t_Mutex_PDU);
}

uint8 PDU_get_State(void){
    return st_PDU.PDU_Circuit.u8_Circuit_State;
}

uint8 PDU_get_Fault_Precharge(void){
    return st_PDU.Errors.bits.b2_Precharge;
}

uint8 PDU_get_Fault_No_Voltage(void){
    return st_PDU.Errors.bits.b2_No_Voltage;
}

uint8 PDU_get_Fault_Bypass(void){
    return st_PDU.Errors.bits.b2_Bypass;
}

uint8 PDU_get_Fault_Welded(void){
    return st_PDU.Errors.bits.b2_Welded;
}

uint8 PDU_get_Fault_Discharge(void){
    return st_PDU.Errors.bits.b2_Discharge;
}

uint8 PDU_get_DC_Charge_Failure(void){
    if(st_PDU.Errors.bits.b2_Precharge || st_PDU.Errors.bits.b2_No_Voltage || st_PDU.Errors.bits.b2_Bypass){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

sint16 PDU_Update_Battery_Voltage_V(uint16 u16_Batt_Voltage_V){
    if(u16_Batt_Voltage_V > PDU_MIN_OPERATIONAL_VOLTAGE_V){
        st_PDU.PDU_Circuit.u16_Operational_Voltage_V = u16_Batt_Voltage_V - 1;
        st_PDU.PDU_Circuit.u16_Contactor_Engage_Voltage_V =  u16_Batt_Voltage_V - 20;
        return 0;
    }
    else{
        st_PDU.PDU_Circuit.u16_Operational_Voltage_V = PDU_MIN_OPERATIONAL_VOLTAGE_V - 1;
        st_PDU.PDU_Circuit.u16_Contactor_Engage_Voltage_V =  PDU_MIN_OPERATIONAL_VOLTAGE_V - 20;
        return -1;
    }
}
