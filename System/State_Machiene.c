#include "OS_Defs.h"

void State_Sleep(void){
    //TODO Reiniciar variables de flujo
    //TODO Extrar fallas desde EPROPM

    Key_set_Enable(TRUE);
    System_Detect_Drive_Block();

    if(Battery_get_Gun_Connection() == FALSE){
        System_set_Charge_State_Detected(FALSE);
    }

    Air_Disable();
    Cooling_Set_Enable_Pump(FALSE);
    SCU_set_DCU_Enable(FALSE);
    SCU_set_PSU_Enable(FALSE);
    Jarvis_Set_Rpm(0.0f);
    Baterry_Set_Speed_RPM(0);
    Tachograph_Set_Rpm(0);
    Steering_set_Off();

    //Open PDU Circuits
    PDU_Set_Open();
    Baterry_Set_Pos_Contator(BATTERY_RELAY_STATE_OPEN);

    if(!PDU_Is_Open()){
        return;
    }

    //Turn Off Subsystems
    GPIO_set_T15_BCU(DS_OFF);
    GPIO_set_T15_Inv(DS_OFF);

    GPIO_set_T15_Digi(DS_ON);

    GPIO_set_Cooling_Tank(DS_ON);
    GPIO_set_Heating_Tank(DS_ON);


    Vision_off(Fusi_get_DTC(),Key_get_State());

    if(GPIO_get_Emergency()){
        Vision_Set_Timeout();
        return;
    }

    if(Fusi_get_Block_Power_On()){
        return;
    }

    if(Battery_get_Gun_Connection() && (System_get_Charge_State_Detected() == FALSE) && (Battery_get_SOC() < MAX_CHARGE_SOC_LIMIT)){
        System_Change_State(ECU_STATE_POWER_ON);
        System_Power_On_Goto_State(POWER_ON_STATE_CHECK_CIRCUITS);
        System_set_Charge_Lock(FALSE);
    }

    if((Key_get_State() == KEY_ACCESORY) || (Key_get_State() == KEY_START)){
        System_Change_State(ECU_STATE_POWER_ON);
        System_Power_On_Goto_State(POWER_ON_STATE_CHECK_CIRCUITS);
        System_set_Charge_Lock(TRUE);
    }

    if (System_get_Aux_State() == ECU_STATE_GPIO_CHECK)
    {
        System_Change_State(ECU_STATE_GPIO_CHECK);
    }
}

void State_Power_On(void){
    Key_set_Enable(FALSE);

    switch(st_System.st_Power_OnOff.u8_Power_On_State){
        case POWER_ON_STATE_CHECK_CIRCUITS:
            Power_On_Check_Circuits();
            break;
        case POWER_ON_STATE_POWER_ON_T15:
            Power_On_Power_T15();
            break;
        case POWER_ON_STATE_BUTTONS:
            Power_On_Buttons();
            break;
        case POWER_ON_STATE_BATTERY_CAN:
            Power_On_Battery_CAN();
            break;
        case POWER_ON_STATE_PROTOCOL_INVERTER:
            Power_On_Protocol_Inverter();
            break;
        case POWER_ON_STATE_BATTERY_READY:
            Power_On_Check_Battery_Ready();
            break;
        case POWER_ON_STATE_CIRCUIT_CLOSE:
            Power_On_PDU_Close();
            break;
        case POWER_ON_STATE_FAULT:
            Key_set_Enable(TRUE);
            Power_On_Fault();
            break;
        default:
            System_Power_On_Goto_State(POWER_ON_STATE_FAULT);
            break;
    }

    Vision_Set_Vehicle_State(VISION_VEHICLE_STATE_INIT);
}

void State_Auxiliary(void){

    if(Battery_get_Gun_Connection()){
        Key_set_Enable(FALSE);
    }
    else{
        Key_set_Enable(TRUE);
    }

    SCU_set_PSU_Enable(TRUE);
    SCU_set_PSU_Voltage_V(PSU_VOLTAGE);
    SCU_set_DCU_Enable(FALSE);
    Cooling_Set_Enable_Pump(TRUE);
    Jarvis_Set_Rpm(0.0f);
    Panel_Set_Rpm(0);
    Baterry_Set_Speed_RPM(0);
    Tachograph_Set_Rpm(0);
    Steering_set_Off();
    Vision_Set_Vehicle_State(VISION_VEHICLE_STATE_CONTACT);
    BTMS_Set_ChargingState(BTMS_NOT_CHARGING);

    if(!PDU_Is_Closed()){
        //TODO FAULT
        return;
    }

    PDU_Set_Close();
    Baterry_Set_Pos_Contator(BATTERY_RELAY_STATE_OPEN);

    //Emergency Button
    if(Inverter_Get_EmergencyStop() || GPIO_get_Emergency()){
        System_Change_State(ECU_STATE_POWER_OFF);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CHECK_INV_STANDBY);
        return;
    }


    if((Key_get_State() == KEY_OFF) && Battery_get_SOC() >= MAX_CHARGE_SOC_LIMIT){
        System_Change_State(ECU_STATE_POWER_OFF);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CHECK_INV_STANDBY);
        return;
    }

    if(Battery_get_Charging() && Battery_get_SOC() >= MAX_CHARGE_SOC_LIMIT){
        System_Change_State(ECU_STATE_POWER_OFF);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CHECK_INV_STANDBY);
    }

    //State change to Charge
    if((Key_get_State() == KEY_OFF) && ((Battery_get_Gun_Connection() == TRUE)) && !System_get_Charge_Lock()){
        if(Fusi_Change_State_Check()){
            System_Change_State(ECU_STATE_CHARGE);
        }
        return;
    }

    //State change to Power Off
    if((Key_get_State() == KEY_OFF) && System_get_Charge_Lock()){
        System_Change_State(ECU_STATE_POWER_OFF);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CHECK_INV_STANDBY);
    }

    if(Fusi_get_Power_Off_Now() || Fusi_get_Power_Off_Park()){
        System_Change_State(ECU_STATE_POWER_OFF);
        System_Power_Off_Goto_State(POWER_OFF_STATE_CHECK_INV_STANDBY);
    }


    //State change to Vehicle On
    if((Key_get_State() == KEY_START) && (System_get_Drive_Block() == FALSE)
    && Jarvis_get_Button_Hand_Break() && (Gear_get_State() != GEAR_DIRECT)
    && (Gear_get_State() != GEAR_REVERSE))
    {
        if(Fusi_Change_State_Check()){
            System_Change_State(ECU_STATE_DRIVE);
        }
    }

}


void State_Drive(void){
    SCU_set_PSU_Enable(TRUE);
    SCU_set_PSU_Voltage_V(PSU_VOLTAGE_DRIVE);
    Cooling_Set_Enable_Pump(TRUE);
    Jarvis_Set_Rpm(Inverter_Get_Filtered_Speed_rpm());
    Panel_Set_Rpm(Inverter_Get_Filtered_Speed_rpm());
    Baterry_Set_Speed_RPM(Inverter_Get_Filtered_Speed_rpm());
    Tachograph_Set_Rpm(Inverter_Get_Filtered_Speed_rpm());
    Steering_set_On();

    GPIO_set_Steering_Tank(DS_ON);

    Vision_Set_Vehicle_State(VISION_VEHICLE_STATE_READY);

    if(Fusi_get_Pressure_Ok()){
        Vision_Set_Air_Charging(FALSE);
    }
    else{
        Vision_Set_Air_Charging(TRUE);
    }

    if(Air_get_Compresor_Ok()){
        Vision_Set_Waiting(FALSE);
    }
    else{
        Vision_Set_Waiting(TRUE);
    }


    PDU_Set_Close();
    Baterry_Set_Pos_Contator(BATTERY_RELAY_STATE_CLOSE);

    Inverter_Calculate_Torque();

    if(Key_get_State() == KEY_OFF){
        Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
        Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
        Inverter_Set_Torque_nm(0.0f);
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(Inverter_Get_EmergencyStop() || GPIO_get_Emergency()){
        Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
        Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
        Inverter_Set_Torque_nm(0.0f);
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(Battery_get_Gun_Connection()){
        Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
        Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
        Inverter_Set_Torque_nm(0.0f);
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(Fusi_get_Power_Off_Now()){
        Inverter_Set_Operation_Mode(INV_REQ_MODE_NEUTRAL);
        Inverter_Set_Command_Mode(INV_REQ_CMD_MODE_TORQUE);
        Inverter_Set_Torque_nm(0.0f);
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(!PDU_Is_Closed()){
        //TODO FAULT
        return;
    }
}

void State_Charge(void){
    System_set_Charge_Lock(TRUE);
    
    SCU_set_PSU_Enable(TRUE);
    SCU_set_PSU_Voltage_V(PSU_VOLTAGE);
    Cooling_Set_Enable_Pump(TRUE);
    Jarvis_Set_Rpm(0.0f);
    Panel_Set_Rpm(0);
    Baterry_Set_Speed_RPM(0);
    Tachograph_Set_Rpm(0);
    Steering_set_Off();
    Vision_Set_Vehicle_State(VISION_VEHICLE_STATE_CHARGING);

    PDU_Set_Close();
    Baterry_Set_Pos_Contator(BATTERY_RELAY_STATE_OPEN);

    if(Battery_get_Charging()){
        System_set_Charge_State_Detected(TRUE);
        BTMS_Set_ChargingState(BTMS_CHARGING);
    }

    if(GPIO_get_Emergency() || Battery_get_SOC() > MAX_CHARGE_SOC_LIMIT ){
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(Battery_get_Charge_Finished() || Battery_get_Charging_Error() || (Battery_get_Gun_Connection() == FALSE) || (System_get_Charge_State_Detected() && Battery_get_Not_Charging())){
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(Fusi_get_Power_Off_Now() || Fusi_get_Power_Off_Park()){
        System_Change_State(ECU_STATE_AUXILIARIES);
    }

    if(!PDU_Is_Closed()){
        //TODO FAULT
        return;
    }
}
void State_Fault(void){

}

void State_Power_Off(void){
    Key_set_Enable(FALSE);
    Vision_Set_Vehicle_State(VISION_VEHICLE_STATE_POWER_OFF);
    switch(st_System.st_Power_OnOff.u8_Power_Off_State){
        case POWER_OFF_STATE_CHECK_INV_STANDBY:
            Power_Off_Check_Inv_Standby();
            break;
        case POWER_OFF_STATE_DISABLE_SYSTEMS:
            Power_Off_Disable_Systems();
            break;
        case POWER_OFF_STATE_CIRCUIT_OPEN:
            Power_Off_PDU_Open();
            break;
        case POWER_OFF_STATE_BATTERY_OPEN:
            Power_Off_Battery_Open();
            break;
        case POWER_OFF_STATE_WAIT:
            Power_Off_Wait();
            break;
        case POWER_OFF_STATE_POWER_OFF_T15:
            Power_Off_Power_Off_T15();
            break;
        case POWER_OFF_STATE_WAIT_BATTERY:
            Key_set_Enable(TRUE);
            Power_Off_Wait_Battery();
            Vision_Set_Timeout();
            break;
        case POWER_OFF_STATE_FAULT:
            Power_Off_Fault();
            break;
        default:
            System_Power_Off_Goto_State(POWER_OFF_STATE_FAULT);
            break;

    }
}

void State_GPIO_Check(void)
{
    if (System_get_Aux_State() == ECU_STATE_SLEEP)
    {
        System_Change_State(ECU_STATE_SLEEP);
    }

    EEPROM_Rx();
}

void Run_System_Machine_State(void){

    if (st_System.st_State.u8_ECU_State != ECU_STATE_GPIO_CHECK)
    {
        PDU_Machine_State();
        System_PDU_To_Battery();
        Jarvis_Set_Is_Regen(System_get_Is_Regen());
        Fusi_Execute();
        Steering_Set_Vehicle_Speed_kmh(Inverter_get_Speed_kmh());
        Steering_Machine();
        AC_Config_Run();
        // --------- BTMS -------------
        Battery_Thermal_Management();
        //BTMS_Set_HVVoltage_V(Inverter_Get_Power_Voltage_V());
        BTMS_Run_Machine_State();
    }

    if((st_System.st_State.u8_ECU_State != ECU_STATE_SLEEP) && (st_System.st_State.u8_ECU_State != ECU_STATE_POWER_ON)){
        System_Set_Inv_Limits();
    }

    switch(st_System.st_State.u8_ECU_State){
        case ECU_STATE_SLEEP:
            State_Sleep();
            break;
        case ECU_STATE_POWER_ON:
            State_Power_On();
            break;
        case ECU_STATE_AUXILIARIES:
            State_Auxiliary();
            break;
        case ECU_STATE_DRIVE:
            State_Drive();
            break;
        case ECU_STATE_CHARGE:
            State_Charge();
            break;
        case ECU_STATE_FAULT:
            State_Fault();
            break;
        case ECU_STATE_POWER_OFF:
            State_Power_Off();
            break;
        case ECU_STATE_GPIO_CHECK:
            State_GPIO_Check();
            break;
        default:
            System_Power_On_Goto_State(POWER_ON_STATE_FAULT);
            break;
    }
}
