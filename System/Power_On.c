#include "OS_Defs.h"

void Power_On_Fault_Triger(uint8 u8_Mask_Fault){
    st_System.st_Power_OnOff.un_On_Faults.all |= u8_Mask_Fault;
}

void Power_Off_Fault_Triger(uint8 u8_Mask_Fault){
    st_System.st_Power_OnOff.un_Off_Faults.all |= u8_Mask_Fault;
}

void Power_On_Check_Circuits(void){
    
    //Check Relays Open and not Welding fault
    if(PDU_Get_Fault()){
        Power_On_Fault_Triger(POWER_ON_FAULT_PDU_CIRUIT);
        Fusi_Set_Power_On_PDU_Circuits(FUSI_ON_FLAG);
        System_Power_Off_Goto_State(POWER_OFF_STATE_WAIT_BATTERY);
        System_Change_State(ECU_STATE_POWER_OFF);
        return;
    }
    if(!PDU_Is_Open()){
        return;
    }

    HVIL_Activate();

    if(System_Power_OnOff_Get_Timeout()){
        if(HVIL_get_Feedback() == HVIL_CLOSE){
            Fusi_Set_Power_On_HVIL(FUSI_OFF_FLAG);
            System_Power_On_Goto_State(POWER_ON_STATE_POWER_ON_T15);
        }
        else{
            Fusi_Set_Power_On_HVIL(FUSI_ON_FLAG);
            System_Power_Off_Goto_State(POWER_OFF_STATE_WAIT_BATTERY);
            System_Change_State(ECU_STATE_POWER_OFF);
        }
    }
}

void Power_On_Power_T15(void){
    GPIO_set_T15_Digi(DS_ON);
    GPIO_set_T15_Screen(DS_OFF);
    GPIO_set_T15_BCU(DS_ON);
    GPIO_set_T15_Inv(DS_ON);

    GPIO_set_Cooling_Tank(DS_ON);
    GPIO_set_Heating_Tank(DS_ON);

    CAPL_Send_Battery_Timer();

    Telemetry_Odometer();


    if(System_Power_OnOff_Get_Timeout()){
        System_Power_On_Goto_State(POWER_ON_STATE_BUTTONS);
    }
}

void Power_On_Buttons(void){
    if(Buttons_Reset()){
        System_Power_On_Goto_State(POWER_ON_STATE_BATTERY_CAN);
    }
}

void Power_On_Battery_CAN(void){
    if(System_Power_OnOff_Get_Timeout()){
        if(!Battery_Get_CAN_OK()){
            Fusi_Set_Power_On_Battery_CAN(FUSI_ON_FLAG);
            System_Power_Off_Goto_State(POWER_OFF_STATE_POWER_OFF_T15);
            System_Change_State(ECU_STATE_POWER_OFF);
        }else{
            System_Power_On_Goto_State(POWER_ON_STATE_PROTOCOL_INVERTER);
        }
    }
}

void Power_On_Protocol_Inverter(void){
    if(System_Power_OnOff_Get_Timeout()){
        Power_On_Fault_Triger(POWER_ON_FAULT_INVERTER_PROTOCOL);
        Fusi_Set_Power_On_Protocol_Inverter(FUSI_ON_FLAG);
        System_Power_Off_Goto_State(POWER_OFF_STATE_POWER_OFF_T15);
        System_Change_State(ECU_STATE_POWER_OFF);
    }

    Inverter_Set_Operation_State(INV_REQ_STATE_STANDBY);
    Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
    Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
    Inverter_Set_Torque_nm(0.0f);

    Inverter_Tx_Protocol();

    if(Inverter_Get_Potocol_Flag()){
        System_Power_On_Goto_State(POWER_ON_STATE_BATTERY_READY);
    }
}

void Power_On_Check_Battery_Ready(void){
    if(System_Power_OnOff_Get_Timeout()){
        Power_On_Fault_Triger(POWER_ON_FAULT_BATTERY_TIMEOUT);
        Fusi_Set_Power_On_Battery_Ready(FUSI_ON_FLAG);
        System_Power_Off_Goto_State(POWER_OFF_STATE_BATTERY_OPEN);
        System_Change_State(ECU_STATE_POWER_OFF);
    }

    if(PDU_Update_Battery_Voltage_V(Battery_get_Pack_Voltage_V()) != C_NO_ERR){
        return;
    }

    Battery_Set_Power_On();
    CAPL_Send_Batt_On();

    if(Battery_Is_Ready()){
        System_Power_On_Goto_State(POWER_ON_STATE_CIRCUIT_CLOSE);

    }

}
void Power_On_PDU_Close(void){
    if(System_Power_OnOff_Get_Timeout() || PDU_get_DC_Charge_Failure()){
        Power_On_Fault_Triger(POWER_ON_FAULT_CLOSE_TIMEOUT);
        Fusi_Set_Power_On_PDU_Close(FUSI_ON_FLAG);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CIRCUIT_OPEN);
        System_Change_State(ECU_STATE_POWER_OFF);
    }

    PDU_Set_Close();

    if(PDU_Is_Closed()){
        System_Change_State(ECU_STATE_AUXILIARIES);
    }
}
void Power_On_Fault(void){
    PDU_Set_Open();
    if(Key_get_State() ==  KEY_OFF){
        System_Change_State(ECU_STATE_SLEEP);
    }
}

void Power_Off_Check_Inv_Standby(void){
    if(System_Power_OnOff_Get_Timeout()){
        GPIO_set_T15_Inv(DS_OFF);
        Power_Off_Fault_Triger(POWER_OFF_FAULT_INV_STANDBY);
        Fusi_Set_Power_Off_Inverter(FUSI_ON_FLAG);
        System_Power_Off_Goto_State(POWER_OFF_STATE_DISABLE_SYSTEMS);
    }
    Inverter_Set_Operation_State(INV_REQ_STATE_STANDBY);
    Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
    Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
    Inverter_Set_Torque_nm(0.0f);
    //CAPL_Send_Inv_Neutral();
    //CAPL_Send_Inv_Standby();

    CAPL_Send_Inv_Standby();

    if((Inverter_Get_Speed_rpm() > 100) || (Inverter_Get_Speed_rpm() < -100)){
        return;
    }

    if((Inverter_Get_Operational_State() != INV_STATE_STANDBY) && (Inverter_Get_Operational_State() != INV_STATE_FAILURE)){
        return;
    }

    sint16 s16_Current_A;
    s16_Current_A = Inverter_Get_Power_Current_A();
    if((s16_Current_A <= 10) && (s16_Current_A >= -10)){
        System_Power_Off_Goto_State(POWER_OFF_STATE_DISABLE_SYSTEMS);
    }
}

void Power_Off_Disable_Systems(void){
    if(System_Power_OnOff_Get_Timeout()){
        Power_Off_Fault_Triger(POWER_OFF_FAULT_AUX_CURRENT_TIMEOUT);
        Fusi_Set_Power_Off_Current(FUSI_ON_FLAG);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CIRCUIT_OPEN);
    }

    sint16 s16_Current_A;
    s16_Current_A = System_Get_Aux_Current();

    SCU_set_PSU_Enable(FALSE);
    SCU_set_PSU_Voltage_V(PSU_VOLTAGE);
    Steering_set_Off();

    if((s16_Current_A <= 25) && (s16_Current_A >= -25)){
        System_Power_Off_Goto_State(POWER_OFF_STATE_CIRCUIT_OPEN);
    }
}

void Power_Off_PDU_Open(void){
    if(System_Power_OnOff_Get_Timeout()){
        System_Power_Off_Goto_State(POWER_OFF_STATE_FAULT);
        Power_Off_Fault_Triger(POWER_OFF_FAULT_OPEN_TIMEOUT);
        Fusi_Set_Power_Off_PDU(FUSI_ON_FLAG);
    }

    PDU_Set_Open();

    if(PDU_Is_Open() || PDU_Get_Fault()){
        System_Power_Off_Goto_State(POWER_OFF_STATE_BATTERY_OPEN);
    }
    if(PDU_Get_Fault()){

    }
}

void Power_Off_Battery_Open(void){
    if(System_Power_OnOff_Get_Timeout()){
        System_Power_Off_Goto_State(POWER_OFF_STATE_FAULT);
        Power_Off_Fault_Triger(POWER_OFF_FAULT_BATTERY_TIMEOUT);
        Fusi_Set_Power_Off_Battery(FUSI_ON_FLAG);
    }

    CAPL_Send_Batt_Off();

    Battery_Set_Power_Off();

    if(Battery_Is_Open()){
        System_Power_Off_Goto_State(POWER_OFF_STATE_WAIT);

    }

}

void Power_Off_Wait(void){
    Inverter_Set_Operation_State(INV_REQ_STATE_SHUTDOWN);
    Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
    Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
    Inverter_Set_Torque_nm(0.0f);

    CAPL_Send_Inv_Power_Off();

    if((Inverter_Get_Operational_State() == INV_STATE_READY_TO_SHUTDOWN) || (Inverter_Get_Operational_State() == INV_STATE_FAILURE)){
        System_Power_Off_Goto_State(POWER_OFF_STATE_POWER_OFF_T15);
    }


    if(System_Power_OnOff_Get_Timeout()){
        System_Power_Off_Goto_State(POWER_OFF_STATE_POWER_OFF_T15);
    }
}

void Power_Off_Power_Off_T15(void){
    GPIO_set_T15_Digi(DS_ON);
    GPIO_set_T15_Screen(DS_OFF);
    GPIO_set_T15_BCU(DS_OFF);

    GPIO_set_Cooling_Tank(FALSE);
    GPIO_set_Heating_Tank(FALSE);
    GPIO_set_Steering_Tank(FALSE);

    Odometer_Save();

    if(System_Power_OnOff_Get_Timeout()){
        Inverter_Set_Operation_State(INV_REQ_STATE_SHUTDOWN);
        GPIO_set_T15_Inv(DS_OFF);
        Inverter_Set_Potocol_Flag_False();
        System_Power_Off_Goto_State(POWER_OFF_STATE_WAIT_BATTERY);
    }
}

void Power_Off_Wait_Battery(void){
    GPIO_set_T15_BCU(DS_OFF);
    CAPL_Send_Battery_Ign_Off();
    HVIL_Deactivate();

    if((System_Power_OnOff_Get_Timeout() || (!Battery_Get_CAN_OK())) && (Key_get_State() ==  KEY_OFF)){
        System_Change_State(ECU_STATE_SLEEP);
        Fusi_Set_Power_On_Battery_CAN(FUSI_OFF_FLAG);
        Fusi_Set_Power_On_Protocol_Inverter(FUSI_OFF_FLAG);
        Fusi_Set_Power_On_Battery_Ready(FUSI_OFF_FLAG);
        Fusi_Set_Power_On_PDU_Close(FUSI_OFF_FLAG);
    }
}

void Power_Off_Fault(void){
    SCU_set_PSU_Enable(FALSE);
    SCU_set_PSU_Voltage_V(PSU_VOLTAGE);
    PDU_Set_Open();
    Battery_Set_Power_Off();
    if(System_Power_OnOff_Get_Timeout()){
        System_Change_State(ECU_STATE_FAULT);
    }
}
