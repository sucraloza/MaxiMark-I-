#include "OS_Defs.h"
#include "btms.h"

static const sint16 BTMS_MIN_TEMPERATURE = -40;
static const sint16 BTMS_MAX_TEMPERATURE = 215;
static const float32 BTMS_MIN_POWER = 0.0f;
static const float32 BTMS_MAX_POWER = 6553.5f;

static T_BTMS t_btms;

static void *mpv_MsgObjRx_BTMS;
static void *mpv_MsgObjTx_BTMS;

/** Init Function **/
sint8 BTMS_Init(void)
{
    t_btms.e_MQ_State = BTMS_WORK_SHUTDOWN;
    t_btms.e_MQ_State_backup = BTMS_WORK_SHUTDOWN;
    t_btms.u8_MQ_Fault_Codes = (uint8) (BTMS_MQ_ERROR_NORMAL);
    t_btms.u8_On = FALSE;
    t_btms.u16_HighVoltage_V = 0;
    t_btms.s8_Temperature_C = 25;
    t_btms.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.e_Charging_Satus = BTMS_NOT_CHARGING;
    t_btms.u8_BTMS_Msg_Counter = 11;

    t_btms.t_Feedback.e_FaultCode = T2B_NO_FAULT;
    t_btms.t_Feedback.e_FaultLevel = T2B_NORMAL;
    t_btms.t_Feedback.s16_InletTemperature_C = 0;
    t_btms.t_Feedback.s16_OutletTemperature_C = 0;
    t_btms.t_Feedback.f32_RequestedPower_kW = 0.0f;
    t_btms.t_Feedback.e_RelayStatus = BTMS_RELAY_INVALID_1;
    t_btms.t_Feedback.e_WorkStatus = BTMS_WORK_INVALID;

    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_OFF;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_OPEN;
    t_btms.t_Setpoints.u16_HighVoltage_V = 0;
    t_btms.t_Setpoints.s8_Temperature_C = 0;
    t_btms.t_Setpoints.u8_RollingCounter = 0;

    sint16 s16_Error = C_NO_ERR;
    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;        // no callback function

    // init a RX object on CAN node1
    t_CanObjInit.t_Id.u32_Id = BTMS_TX_MASK;  // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;
    s16_Error |= x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_BTMS);

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer

    // init a RX object on CAN node1
    t_CanObjInit.t_Id.u32_Id = BTMS_RX_MASK;  // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;
    s16_Error |= x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjRx_BTMS);

    if(s16_Error != C_NO_ERR){
        Error_Beep();
    }

    return s16_Error;
}

/** Machine state functions **/
void BTMS_Sleep(void){

    GPIO_set_T15_BTMS(DS_OFF);
    GPIO_set_PDU_Contactor_BTMS(DS_OFF);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_OFF;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_OPEN;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = BTMS_VOLTAGE_INVALID;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(t_btms.u16_HighVoltage_V < 500){ //eSTO ESTA MAL (?)
        return;
    }

    if((t_btms.u8_On == FALSE ))
    {
        return;
    }
    // Change from 500 to 5000
    t_btms.u32_Timeout = Timeout_Set_ms(5000);
    t_btms.e_MQ_State = BTMS_STATE_T15_ON;
}

void BTMS_T15_On(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_OFF);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_OFF;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_OPEN;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = BTMS_VOLTAGE_INVALID;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(!Timeout_Check(t_btms.u32_Timeout))
    {
        return;
    }

    if(t_btms.t_Feedback.e_WorkStatus !=  BTMS_WORK_SHUTDOWN ||
       t_btms.t_Feedback.e_RelayStatus !=  FALSE ||
       t_btms.t_Feedback.e_FaultLevel !=  T2B_NORMAL ||
       t_btms.t_Feedback.e_FaultCode != T2B_NO_FAULT
       )
    {
        t_btms.e_MQ_State = BTMS_STATE_SLEEP;
        t_btms.u8_MQ_Fault_Codes |= (uint8) (BTMS_MQ_ERROR_T15);
        return;
    }

    t_btms.u32_Timeout = Timeout_Set_ms(100);
    t_btms.e_MQ_State = BTMS_STATE_PDU_CLOSE;

}

void BTMS_PDU_Close(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_ON);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_OFF;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_OPEN;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = BTMS_VOLTAGE_INVALID;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(!Timeout_Check(t_btms.u32_Timeout))
    {
        return;
    }
    // Change from 5000 to 30000
    t_btms.u32_Timeout = Timeout_Set_ms(30000);
    t_btms.e_MQ_State = BTMS_STATE_PRECHARGE;
}

void BTMS_Precharge(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_ON);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_ON;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_CLOSE;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = t_btms.u16_HighVoltage_V;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(t_btms.t_Feedback.e_RelayStatus){
        t_btms.e_WorkStatus = BTMS_WORK_SELF_LOOP; // New
        t_btms.e_MQ_State = BTMS_STATE_OPERATION;
        return;
    }

    if(!Timeout_Check(t_btms.u32_Timeout))
    {
        return;
    }

    if(!t_btms.t_Feedback.e_RelayStatus){
        t_btms.e_MQ_State = BTMS_STATE_WAIT_FOR_SHUTDOWN;
        t_btms.u8_MQ_Fault_Codes |= (uint8) (BTMS_MQ_ERROR_PRECHARGE);
    }
}

void BTMS_Operation(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_ON);
    t_btms.t_Setpoints.e_WorkStatus = t_btms.e_WorkStatus;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_ON;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_CLOSE;
    t_btms.t_Setpoints.e_ChargingState = t_btms.e_Charging_Satus;
    t_btms.t_Setpoints.u16_HighVoltage_V = t_btms.u16_HighVoltage_V;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(t_btms.u8_On == DS_OFF)
    {
        t_btms.u32_Timeout = Timeout_Set_ms(1000);
        t_btms.e_MQ_State = BTMS_STATE_WAIT_FOR_SHUTDOWN;
    }

    if(t_btms.t_Feedback.e_FaultLevel > T2B_NO_FAULT)
    {
        t_btms.u32_Timeout = Timeout_Set_ms(1000);
        t_btms.u8_MQ_Fault_Codes |= (uint8) (BTMS_MQ_ERROR_BTMS_FAULT);
        t_btms.e_MQ_State = BTMS_STATE_WAIT_FOR_SHUTDOWN;
    }
}

void BTMS_Wait_For_Shutdown(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_ON);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_ON;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_CLOSE;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = t_btms.u16_HighVoltage_V;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(t_btms.t_Feedback.e_WorkStatus ==  BTMS_WORK_SHUTDOWN){
        t_btms.u32_Timeout = Timeout_Set_ms(5000);
        t_btms.e_MQ_State = BTMS_STATE_POWER_OFF;
        return;
    }

    if(Timeout_Check(t_btms.u32_Timeout))
    {
        t_btms.u32_Timeout = Timeout_Set_ms(2000);
        t_btms.u8_MQ_Fault_Codes |= (uint8) (BTMS_WAIT_SHUTDOWN_TIMEOUT);
        t_btms.e_MQ_State = BTMS_STATE_POWER_OFF;
        return;
    }
}

void BTMS_Power_Off(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_ON);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_OFF;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_CLOSE;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = t_btms.u16_HighVoltage_V;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(t_btms.t_Feedback.e_RelayStatus == FALSE){
        t_btms.u32_Timeout = Timeout_Set_ms(300);
        t_btms.e_MQ_State = BTMS_STATE_PDU_OPEN;
        return;
    }

    if(Timeout_Check(t_btms.u32_Timeout))
    {
        t_btms.u32_Timeout = Timeout_Set_ms(500);
        t_btms.u8_MQ_Fault_Codes |= (uint8) (BTMS_POWER_OFF_TIMEOUT);
        t_btms.e_MQ_State = BTMS_STATE_PDU_OPEN;
        return;
    }
}

void BTMS_PDU_Open(void){
    GPIO_set_T15_BTMS(DS_ON);
    GPIO_set_PDU_Contactor_BTMS(DS_OFF);
    t_btms.t_Setpoints.e_WorkStatus = BTMS_WORK_SHUTDOWN;
    t_btms.t_Setpoints.e_Request_HV = BTMS_REQUEST_HV_OFF;
    t_btms.t_Setpoints.e_RelayStatus = BTMS_RELAY_OPEN;
    t_btms.t_Setpoints.e_ChargingState = BTMS_NOT_CHARGING;
    t_btms.t_Setpoints.u16_HighVoltage_V = BTMS_VOLTAGE_INVALID;
    t_btms.t_Setpoints.s8_Temperature_C = t_btms.s8_Temperature_C;

    if(!Timeout_Check(t_btms.u32_Timeout))
    {
        return;
    }

    t_btms.e_MQ_State = BTMS_STATE_SLEEP;
}

void BTMS_Run_Machine_State(void){
    switch (t_btms.e_MQ_State)
    {
    case BTMS_STATE_SLEEP:
        BTMS_Sleep();
        break;
    case BTMS_STATE_T15_ON:
        BTMS_T15_On();
        break;
    case BTMS_STATE_PDU_CLOSE:
        BTMS_PDU_Close();
        break;
    case BTMS_STATE_PRECHARGE:
        BTMS_Precharge();
        break;
    case BTMS_STATE_OPERATION:
        BTMS_Operation();
        break;
    case BTMS_STATE_WAIT_FOR_SHUTDOWN:
        BTMS_Wait_For_Shutdown();
        break;
    case BTMS_STATE_POWER_OFF:
        BTMS_Power_Off();
        break;
    case BTMS_STATE_PDU_OPEN:
        BTMS_PDU_Open();
        break;
    default:
        t_btms.e_MQ_State = t_btms.e_MQ_State_backup;
        break;
    }
    t_btms.e_MQ_State_backup = t_btms.e_MQ_State;
}

/** CAN FUNCTIONS **/
void BTMS_Rx_FeedBack(uint8 *au8_data)
{
    E_T2B_FaultCode e_FaultCode;
    E_T2B_FaultLevel e_FaultLevel;
    sint16 s16_InletTemperature_C;
    sint16 s16_OutletTemperature_C;
    float32 f32_RequestedPower_kW;
    uint16 u16_RequestedPower;
    E_BTMS_HVRelay_Status e_RelayStatus;
    E_BTMS_WorkStatus e_WorkStatus;

    e_WorkStatus = (E_BTMS_WorkStatus) ((au8_data[0] >> 0) & 0x3);
    e_RelayStatus = (E_BTMS_HVRelay_Status) ((au8_data[0] >> 2) & 0x3);

    s16_OutletTemperature_C = au8_data[1];
    s16_OutletTemperature_C = s16_OutletTemperature_C -40;
    s16_InletTemperature_C = au8_data[2];
    s16_InletTemperature_C = s16_InletTemperature_C -40;

    u16_RequestedPower = au8_data[5];
    u16_RequestedPower = u16_RequestedPower << 8;

    u16_RequestedPower = u16_RequestedPower + au8_data[6];
    f32_RequestedPower_kW = u16_RequestedPower * 0.1f;

    e_FaultCode = (E_T2B_FaultCode) (au8_data[7] & 0x3F);
    e_FaultLevel = (E_T2B_FaultLevel) ((au8_data[7] >> 6) & 0x3);

    t_btms.t_Feedback.e_WorkStatus = e_WorkStatus;
    t_btms.t_Feedback.e_RelayStatus = e_RelayStatus;
    t_btms.t_Feedback.e_FaultCode = e_FaultCode;
    t_btms.t_Feedback.e_FaultLevel = e_FaultLevel;
    t_btms.t_Feedback.s16_OutletTemperature_C = s16_OutletTemperature_C;
    t_btms.t_Feedback.s16_InletTemperature_C = s16_InletTemperature_C;
    t_btms.t_Feedback.f32_RequestedPower_kW = f32_RequestedPower_kW;
}

sint16 BTMS_Recived(void)
{
    T_x_can_msg t_msg;
    uint8 u8_received = FALSE;
    sint16 s16_Error_Can;
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_BTMS, &t_msg);
     // -------  New --------
     while(s16_Error_Can == C_NO_ERR){
        switch(t_msg.t_Id.u32_Id)
        {
            case BTMS_ID_RX_FEEDBACK:
                BTMS_Rx_FeedBack(t_msg.au8_Data);
                x_can_obj_send_msg(mpv_MsgObjTx_Battery, &t_msg);
                u8_received = TRUE;
                break;
            default:
                break;
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_BTMS, &t_msg);
    }

    return (u8_received) ? C_NO_ERR : C_TIMEOUT;
}

void BTMS_Get_Monitoring_Data(T_x_can_msg *t_msg)
{
    t_msg->u8_Dlc = 8;

    t_msg->au8_Data[0] =(uint8) (t_btms.u8_On);
    t_msg->au8_Data[1] =(uint8) (t_btms.e_MQ_State);
    t_msg->au8_Data[2] = t_btms.u8_MQ_Fault_Codes;
    t_msg->au8_Data[3] =(uint8) (t_btms.e_WorkStatus);
    t_msg->au8_Data[4] =(uint8) (t_btms.e_Charging_Satus);
    t_msg->au8_Data[5] =(uint8) (t_btms.s8_Temperature_C);
}

// ------------ new ------------------
void BTMS_Send_Monitoring(uint32 u32_Id, void* oppv_Hanlde)
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = u32_Id;
    t_Msg.t_Id.u8_Xtd = TRUE;

    BTMS_Get_Monitoring_Data(&t_Msg);

    x_can_obj_send_msg(oppv_Hanlde, &t_Msg);
}


sint16 BTMS_Tx_Control(void)
{
    uint8 u8_crc;
    T_x_can_msg t_msg;
    t_msg.t_Id.u32_Id = BTMS_ID_TX_CONTROL;
    t_msg.t_Id.u8_Xtd = TRUE;
    t_msg.u8_Dlc = 8;

    t_msg.au8_Data[0] = ((uint8) (t_btms.t_Setpoints.e_WorkStatus) & 0x3);
    t_msg.au8_Data[0] |= ((uint8) (t_btms.t_Setpoints.e_Request_HV) & 0x3) << 2;
    t_msg.au8_Data[0] |= ((uint8) (t_btms.t_Setpoints.e_ChargingState) & 0x3) << 4;
    t_msg.au8_Data[0] |= ((uint8) (t_btms.t_Setpoints.e_RelayStatus) & 0x3) << 6;
    u8_crc = t_msg.au8_Data[0];

    t_msg.au8_Data[1] = (uint8)(t_btms.t_Setpoints.u16_HighVoltage_V >> 8);
    u8_crc ^= t_msg.au8_Data[1];
    t_msg.au8_Data[2] = (uint8)(t_btms.t_Setpoints.u16_HighVoltage_V);
    u8_crc ^= t_msg.au8_Data[2];
    t_msg.au8_Data[3] = 0;
    u8_crc ^= t_msg.au8_Data[3];
    t_msg.au8_Data[4] = (uint8)(t_btms.t_Setpoints.s8_Temperature_C + 40);
    u8_crc ^= t_msg.au8_Data[4];
    t_msg.au8_Data[5] = 0;
    u8_crc ^= t_msg.au8_Data[5];
    t_msg.au8_Data[6] = t_btms.t_Setpoints.u8_RollingCounter;
    u8_crc ^= t_msg.au8_Data[6];
    t_msg.au8_Data[7] = u8_crc;

    t_btms.t_Setpoints.u8_RollingCounter++;

    return x_can_obj_send_msg(mpv_MsgObjTx_BTMS, &t_msg);
}

void BTMS_Send(void){
    //if(!t_btms.u8_On) return;
    uint16 u16_Send_Error;
    t_btms.u8_BTMS_Msg_Counter++;
    if(t_btms.u8_BTMS_Msg_Counter >= 11)
    {
        u16_Send_Error = BTMS_Tx_Control();
        t_btms.u8_BTMS_Msg_Counter = 0;
        if(u16_Send_Error != C_NO_ERR) t_btms.u8_MQ_Fault_Codes |= BTMS_MQ_ERROR_CAN_SEND ;
        else t_btms.u8_MQ_Fault_Codes = t_btms.u8_MQ_Fault_Codes & (~((uint8)(BTMS_MQ_ERROR_CAN_SEND)));
    }
}

/** Getters */
E_BTMS_MQ_States BTMS_get_Mq_State(void)
{
    return t_btms.e_MQ_State;
}

E_T2B_FaultCode BTMS_Get_FaultCode(void)
{
    return t_btms.t_Feedback.e_FaultCode;
}

E_T2B_FaultLevel BTMS_Get_FaultLevel(void)
{
    return t_btms.t_Feedback.e_FaultLevel;
}

sint16 BTMS_Get_InletTemperature(void)
{
    return t_btms.t_Feedback.s16_InletTemperature_C;
}

sint16 BTMS_Get_OutletTemperature(void)
{
    return t_btms.t_Feedback.s16_OutletTemperature_C;
}

float32 BTMS_Get_RequestedPower(void)
{
    return t_btms.t_Feedback.f32_RequestedPower_kW;
}

E_BTMS_HVRelay_Status BTMS_Get_RelayStatus(void)
{
    return t_btms.t_Feedback.e_RelayStatus;
}

E_BTMS_WorkStatus BTMS_Get_WorkStatus(void)
{
    return t_btms.t_Feedback.e_WorkStatus;
}

uint8 BTMS_get_Mq_State_Errors(void)
{
    return t_btms.u8_MQ_Fault_Codes;
}

void BTMS_Get_Setpoints(T_BTMS_Setpoints *pt_setpoints)
{
    *pt_setpoints = t_btms.t_Setpoints;
}
/** Setters */
void BTMS_Set_OnOff(uint8 u8_ref){
    t_btms.u8_On = u8_ref;
    //MON_TX_Var32(t_btms.u8_On);
    //if(u8_ref) t_btms.u8_On = DS_ON;
    //else t_btms.u8_On = DS_OFF;
}

sint16 BTMS_Set_WorkMode(E_BTMS_WorkStatus e_ref){
    if (e_ref >= BTMS_WORK_INVALID) return C_RANGE;
    t_btms.e_WorkStatus = e_ref;
    return C_NO_ERR;
}

sint16 BTMS_Set_ChargingState(E_BTMS_ChargingState e_ref){
    if (e_ref >= BTMS_CHARGING_INVALID_1) return C_RANGE;
    t_btms.e_Charging_Satus = e_ref;
    return C_NO_ERR;
}

void BTMS_Set_HVVoltage_V(uint16 u16_voltage_V){
    t_btms.u16_HighVoltage_V = u16_voltage_V;
}
void BTMS_Set_Ref_Temperature_C(sint8 s8_Temperature){
    t_btms.s8_Temperature_C = s8_Temperature;
}

// New
void BTMS_Set_Ref_Temperature(sint8 u8_Temperature){
    t_btms.t_Setpoints.s8_Temperature_C = u8_Temperature;
}

// New
void BTMS_Set_Ref_WorkStatus(uint8 u8_WorkStatus){
    t_btms.t_Setpoints.e_WorkStatus = u8_WorkStatus;
}
