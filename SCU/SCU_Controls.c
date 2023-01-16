#include "OS_Defs.h"
#include "SCU_Controls.h"

T_SCU_Variables t_SCU;
T_x_os_mutex t_Mutex_SCU;

T_x_can_msg t_SCU_Msg;
T_x_os_mutex t_Mutex_SCU_Msg;

void *mpv_MsgObjTx_SCU;
void *mpv_MsgObjRx_SCU;
uint8 u8_SCU_Msg_Counter;

void SCU_Init(void){
    sint16 s16_Error;

    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;        // no callback function

    // init a RX object on CAN node2
    t_CanObjInit.t_Id.u32_Id = CONVERTER_TX_MASK;  // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;
    x_can_obj_init(X_CAN_BUS_02, &t_CanObjInit, &mpv_MsgObjTx_SCU);

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer

    // init a RX object on CAN node2
    t_CanObjInit.t_Id.u32_Id = CONVERTER_RX_MASK;  // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;
    x_can_obj_init(X_CAN_BUS_02, &t_CanObjInit, &mpv_MsgObjRx_SCU);

    //PSU INIT
    t_SCU.t_PSU.u8_InverterStatus = 0;
    t_SCU.t_PSU.u8_InverterReady = 0;
    t_SCU.t_PSU.u8_ErrorStatus = 0;
    t_SCU.t_PSU.f32_Voltage_V = 0;
    t_SCU.t_PSU.f32_Current_A = 0;
    t_SCU.t_PSU.f32_Power_kW = 0;
    t_SCU.t_PSU.s8_Temperature_C = 0;

    //DCU INIT
    t_SCU.t_DCU.u8_InverterStatus = 0;
    t_SCU.t_DCU.u8_InverterReady = 0;
    t_SCU.t_DCU.u8_ErrorStatus = 0;
    t_SCU.t_DCU.s16_MotorSpeed_rpm = 0;
    t_SCU.t_DCU.f32_MotorPower_kW = 0;

    //Device Init
    t_SCU.t_Status.u_Flags.all = 0;
    t_SCU.t_Status.u16_ErrorCode = 0;
    t_SCU.t_Status.u16_VoltageDC_V = 0;
    t_SCU.t_Status.f32_PowerDC_kW = 0;
    t_SCU.t_Status.s8_DeviceTemperature_C = 0;

    //Setpoints Init
    t_SCU.t_Setpoints.u8_PSU_Max_Current_A = PSU_MAXCURRENT;
    t_SCU.t_Setpoints.u8_PSU_Voltage_V = 26;
    t_SCU.t_Setpoints.s16_DCU_Speed_rpm = 0;
    t_SCU.t_Setpoints.u8_Enable_DCU = 0;
    t_SCU.t_Setpoints.u8_Enable_PSU = 0;

    //Errors Init
    t_SCU.u_Errors.all = 0;

    //Filters Init
    Filter_Init(&t_SCU.t_PSU_Voltage_Filter,PSU_VOLTAGE_FILTER_A0,PSU_VOLTAGE_FILTER_A1,PSU_VOLTAGE_FILTER_B1);
    Filter_Init(&t_SCU.t_PSU_Current_Filter,PSU_CURRENT_FILTER_A0,PSU_CURRENT_FILTER_A1,PSU_CURRENT_FILTER_B1);

    TPDU_Init();

    u8_SCU_Msg_Counter = 0;
    s16_Error = x_os_mutex_init(&t_Mutex_SCU);
    s16_Error |= x_os_mutex_init(&t_Mutex_SCU_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();
}

sint8 SCU_Master_Control_Tx()
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = CONVERTER_MASTER_CONTROL_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = CONVERTER_MASTER_CONTROL_ENABLE | (1 << 2);
    t_Msg.au8_Data[1] = DCLINK_VOLTAGE >> 2; // DCLINK_VOLTAGE / 4
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    return x_can_obj_send_msg(mpv_MsgObjTx_SCU, &t_Msg);
}

sint16 SCU_DCU_Tx()
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = CONVERTER_DCU_SETPOINT_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_speed = t_SCU.t_Setpoints.s16_DCU_Speed_rpm + 32000u;
    uint16 u16_torque = (DCU_TORQUE_SETPOINT + 6400) * 5;

    uint8 u8_Enable = t_SCU.t_Setpoints.u8_Enable_DCU;


    t_Msg.au8_Data[0] = u8_Enable;
    t_Msg.au8_Data[1] = DCLINK_VOLTAGE >> 2; // DCLINK_VOLTAGE / 4
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = u16_speed >> 0;
    t_Msg.au8_Data[5] = u16_speed >> 8;
    t_Msg.au8_Data[6] = u16_torque >> 0;
    t_Msg.au8_Data[7] = u16_torque >> 8;

    return x_can_obj_send_msg(mpv_MsgObjTx_SCU, &t_Msg);
}

sint16 SCU_PSU_Tx()
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = CONVERTER_PSU_SETPOINT_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

	uint16 u16_voltage = t_SCU.t_Setpoints.u8_PSU_Voltage_V * 1000;
    sint16 u16_current = (PSU_MAXCURRENT + 1600u) * 20;

    t_Msg.au8_Data[0] = t_SCU.t_Setpoints.u8_Enable_PSU;
    t_Msg.au8_Data[1] = DCLINK_VOLTAGE >> 2; // DCLINK_VOLTAGE / 4
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = u16_voltage >> 0;
    t_Msg.au8_Data[5] = u16_voltage >> 8;
    t_Msg.au8_Data[6] = u16_current >> 0;
    t_Msg.au8_Data[7] = u16_current >> 8;

    return x_can_obj_send_msg(mpv_MsgObjTx_SCU, &t_Msg);
}

void SCU_Status_Rx(void)
{
    if(t_SCU_Msg.t_Id.u32_Id != CONVERTER_STATUS_RX_ID){
        return;
    }

    uint16 u16_Flags = t_SCU_Msg.au8_Data[1];
    u16_Flags = u16_Flags << 8;
    u16_Flags = u16_Flags + t_SCU_Msg.au8_Data[0];
    uint16 u16_ErrorCode = (t_SCU_Msg.au8_Data[3] << 8) | t_SCU_Msg.au8_Data[2];
    uint16 u16_HighVoltage_V = 4 * t_SCU_Msg.au8_Data[4];
	float32 f32_PowerDC = 0.005f * ((t_SCU_Msg.au8_Data[6] << 8) | t_SCU_Msg.au8_Data[5]) - 160;
	sint8 s8_temp_C = ((sint16) t_SCU_Msg.au8_Data[7]) - 40;

    if(x_os_mutex_get(&t_Mutex_SCU,10) != C_NO_ERR){
        return;
    }
    t_SCU.t_Status.u_Flags.all = u16_Flags;
    t_SCU.t_Status.u16_ErrorCode = u16_ErrorCode;
    if(u16_HighVoltage_V < 1000){
		t_SCU.t_Status.u16_VoltageDC_V = u16_HighVoltage_V;
	}

    t_SCU.t_Status.f32_PowerDC_kW = f32_PowerDC;

    if(s8_temp_C != -41){
		t_SCU.t_Status.s8_DeviceTemperature_C = s8_temp_C;
	}

	x_os_mutex_release(&t_Mutex_SCU);
}

void SCU_DCU_Rx(void)
{
    if(t_SCU_Msg.t_Id.u32_Id != CONVERTER_DCU_VALUES_RX_ID){
        return;
    }


    uint8 u8_InverterStatus = (t_SCU_Msg.au8_Data[0] & 0x03);
    uint8 u8_InverterReady = (t_SCU_Msg.au8_Data[0] & 0x0C) >> 2;
    uint8 u8_ErrorStatus = (t_SCU_Msg.au8_Data[0] & 0x30) >> 4;

    sint16 s16_MotorSpeed_rpm = ((t_SCU_Msg.au8_Data[2] << 8) | t_SCU_Msg.au8_Data[1]) - 32000;

    float32 f32_MotorPower_kW = 0.005f * ((t_SCU_Msg.au8_Data[6] << 8) | t_SCU_Msg.au8_Data[5]) - 160;

    if(x_os_mutex_get(&t_Mutex_SCU,10) != C_NO_ERR){
        return;
    }

    t_SCU.t_DCU.u8_InverterStatus = u8_InverterStatus;
    t_SCU.t_DCU.u8_InverterReady = u8_InverterReady;
    t_SCU.t_DCU.u8_ErrorStatus = u8_ErrorStatus;

    if(s16_MotorSpeed_rpm < 10000){
		t_SCU.t_DCU.s16_MotorSpeed_rpm = s16_MotorSpeed_rpm;
	}

    t_SCU.t_DCU.f32_MotorPower_kW = f32_MotorPower_kW;

    x_os_mutex_release(&t_Mutex_SCU);

}

void SCU_PSU_Rx(void)
{
    if(t_SCU_Msg.t_Id.u32_Id != CONVERTER_PSU_VALUES_RX_ID){
        return;
    }

    uint8 u8_InverterStatus = (t_SCU_Msg.au8_Data[0] & 0x03);
    uint8 u8_InverterReady = (t_SCU_Msg.au8_Data[0] & 0x0C) >> 2;
    uint8 u8_ErrorStatus = (t_SCU_Msg.au8_Data[0] & 0x30) >> 4;
    float32 f32_Voltage = 0.001f * ((t_SCU_Msg.au8_Data[2] << 8) | t_SCU_Msg.au8_Data[1]);
    float32 f32_Current = 0.05f * ((t_SCU_Msg.au8_Data[4] << 8) | t_SCU_Msg.au8_Data[3]) - 1600;
    float32 f32_Power = 0.005f * ((t_SCU_Msg.au8_Data[6] << 8) | t_SCU_Msg.au8_Data[5]) - 160;

	sint8 s8_temp_C = ((sint16) t_SCU_Msg.au8_Data[7]) - 40;

    if(x_os_mutex_get(&t_Mutex_SCU,10) != C_NO_ERR){
        return;
    }
    t_SCU.t_PSU.u8_InverterStatus = u8_InverterStatus;
    t_SCU.t_PSU.u8_InverterReady = u8_InverterReady;
    t_SCU.t_PSU.u8_ErrorStatus = u8_ErrorStatus;
    t_SCU.t_PSU.f32_Voltage_V = f32_Voltage;
    t_SCU.t_PSU.f32_Current_A = f32_Current;
    t_SCU.t_PSU.f32_Power_kW = f32_Power;

    Filter(&t_SCU.t_PSU_Voltage_Filter, f32_Voltage);
    Filter(&t_SCU.t_PSU_Current_Filter, f32_Current);

    if(s8_temp_C != -41){
		t_SCU.t_PSU.s8_Temperature_C = s8_temp_C;
	}

    x_os_mutex_release(&t_Mutex_SCU);
}

void SCU_Send_Msg(){
    uint16 u16_Send_Error;
    u8_SCU_Msg_Counter++;
    if(u8_SCU_Msg_Counter >= 10){
        u16_Send_Error = SCU_Master_Control_Tx();
        u16_Send_Error |= SCU_DCU_Tx();
        u16_Send_Error |= SCU_PSU_Tx();
        u8_SCU_Msg_Counter = 0;
        if(u16_Send_Error != C_NO_ERR){
            t_SCU.u_Errors.bits.b2_CAN_Send = 0x3;
        }
        else{
            t_SCU.u_Errors.bits.b2_CAN_Send = 0x0;
        }
    }
}

uint8 SCU_Recived_Msg(){
    sint16 s16_Error;
    sint16 s16_Error_Can;
    uint8 u8_Recived;
    u8_Recived = FALSE;

    //SCU_Check_CAN();
    s16_Error = x_os_mutex_get(&t_Mutex_SCU_Msg,10);
    if(s16_Error != C_NO_ERR){
        return u8_Recived;
    }
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_SCU, &t_SCU_Msg);
    while(s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR){
        u8_Recived = TRUE;
        switch(t_SCU_Msg.t_Id.u32_Id){
            case CONVERTER_STATUS_RX_ID:
                x_os_mutex_release(&t_Mutex_SCU_Msg);
                SCU_Status_Rx();
                break;
            case CONVERTER_DCU_VALUES_RX_ID:
                x_os_mutex_release(&t_Mutex_SCU_Msg);
                SCU_DCU_Rx();
                break;
            case CONVERTER_PSU_VALUES_RX_ID:
                x_os_mutex_release(&t_Mutex_SCU_Msg);
                SCU_PSU_Rx();
                break;
            default:
                x_os_mutex_release(&t_Mutex_SCU_Msg);
        }
        s16_Error = x_os_mutex_get(&t_Mutex_SCU_Msg,10);    //Trato de obtener el mutex denuevo
        if(s16_Error != C_NO_ERR){
            break;                                              //No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_SCU, &t_SCU_Msg);
    }
    x_os_mutex_release(&t_Mutex_SCU_Msg);

    return u8_Recived;
}

void SCU_set_PSU_Enable(uint8 u8_ref){
    if(x_os_mutex_get(&t_Mutex_SCU,10) == C_NO_ERR){
        t_SCU.t_Setpoints.u8_Enable_PSU = u8_ref & 0x3;
        x_os_mutex_release(&t_Mutex_SCU);
    }
}
void SCU_set_PSU_Voltage_V(uint8 u8_ref){
    if(x_os_mutex_get(&t_Mutex_SCU,10) == C_NO_ERR){
        if(u8_ref > 28)
            u8_ref = 28;
        t_SCU.t_Setpoints.u8_PSU_Voltage_V = u8_ref;
        x_os_mutex_release(&t_Mutex_SCU);
    }
}
void SCU_set_PSU_Max_Current_A(uint8 u8_ref){
    if(x_os_mutex_get(&t_Mutex_SCU,10) == C_NO_ERR){
        t_SCU.t_Setpoints.u8_PSU_Max_Current_A = u8_ref;
        x_os_mutex_release(&t_Mutex_SCU);
    }
}
void SCU_set_DCU_Enable(uint8 u8_ref){
    if(x_os_mutex_get(&t_Mutex_SCU,10) == C_NO_ERR){
        t_SCU.t_Setpoints.u8_Enable_DCU = u8_ref & 0x3;
        x_os_mutex_release(&t_Mutex_SCU);
    }
}
void SCU_set_DCU_Speed_rpm(sint16 s16_ref){
    if(x_os_mutex_get(&t_Mutex_SCU,10) == C_NO_ERR){
        if(s16_ref > CONVERTER_MAX_SPEED_RPM)
            s16_ref = CONVERTER_MAX_SPEED_RPM;
        if(s16_ref < -CONVERTER_MAX_SPEED_RPM)
            s16_ref = -CONVERTER_MAX_SPEED_RPM;
        t_SCU.t_Setpoints.s16_DCU_Speed_rpm = s16_ref;
        x_os_mutex_release(&t_Mutex_SCU);
    }
}

float32 SCU_get_PSU_Current_A(void){ return t_SCU.t_PSU.f32_Current_A; }
float32 SCU_get_PSU_Voltage_V(void){ return t_SCU.t_PSU.f32_Voltage_V; }
float32 SCU_get_PSU_Filtered_Current_A(void){ return Filter_get_Output(&t_SCU.t_PSU_Current_Filter); }
float32 SCU_get_PSU_Filtered_Voltage_V(void){ return Filter_get_Output(&t_SCU.t_PSU_Voltage_Filter); }
float32 SCU_get_PSU_Power_kW(void){ return t_SCU.t_PSU.f32_Power_kW; }
sint8 SCU_get_PSU_Temperature_C(void){ return t_SCU.t_PSU.s8_Temperature_C; }
uint8 SCU_get_PSU_Power_Status(void){ return (t_SCU.t_PSU.u8_InverterStatus & 0x3); }
uint8 SCU_get_PSU_Ready(void){ return (t_SCU.t_PSU.u8_InverterReady & 0x3); }
uint8 SCU_get_PSU_Error(void){ return (t_SCU.t_PSU.u8_ErrorStatus & 0x3); }
uint8 SCU_get_PSU_in_Error(void){ return (t_SCU.t_PSU.u8_ErrorStatus == CONVERTER_ERROR); }
uint8 SCU_get_PSU_in_Warning(void){ return (t_SCU.t_PSU.u8_ErrorStatus == CONVERTER_WARNING); }
uint8 SCU_get_PSU_in_Under_Voltage(void){ return (Filter_get_Output(&t_SCU.t_PSU_Voltage_Filter) < PSU_UNDER_VOLTAGE); }
uint8 SCU_get_PSU_in_Working(void){ return ((!SCU_get_PSU_in_Under_Voltage()) && (SCU_get_PSU_Filtered_Current_A() > 0)); }


sint16 SCU_get_DCU_Speed_rpm(void){ return t_SCU.t_DCU.s16_MotorSpeed_rpm; }
sint16 SCU_get_DCU_Torque_Nm(void){ return 0; }
float32 SCU_get_DCU_Power_kW(void){ return t_SCU.t_DCU.f32_MotorPower_kW; }
uint8 SCU_get_DCU_Power_Status(void){ return (t_SCU.t_DCU.u8_InverterStatus & 0x3); }
uint8 SCU_get_DCU_Ready(void){ return (t_SCU.t_DCU.u8_InverterReady & 0x3); }
uint8 SCU_get_DCU_Error(void){ return (t_SCU.t_DCU.u8_ErrorStatus & 0x3); }
uint8 SCU_get_DCU_in_Error(void){ return (t_SCU.t_DCU.u8_ErrorStatus == CONVERTER_ERROR); }
uint8 SCU_get_DCU_in_Warning(void){ return (t_SCU.t_DCU.u8_ErrorStatus == CONVERTER_WARNING); }


uint16 SCU_get_Voltage_V(void){ return t_SCU.t_Status.u16_VoltageDC_V; }
float32 SCU_get_Power_kW(void){ return t_SCU.t_Status.f32_PowerDC_kW; }
sint8 SCU_get_Temperature_C(void){ return t_SCU.t_Status.s8_DeviceTemperature_C; }
uint8 SCU_get_Clamp_T15(void){ return t_SCU.t_Status.u_Flags.bits.b_Clamp_T15_State; }
uint8 SCU_get_Init_Status(void){ return t_SCU.t_Status.u_Flags.bits.b_Init_State; }
uint8 SCU_get_Precharge_Status(void){ return t_SCU.t_Status.u_Flags.bits.b_Precharge_State; }
uint8 SCU_get_Error_Lamp_Status(void){ return t_SCU.t_Status.u_Flags.bits.b_Lamp_State; }
uint16 SCU_get_DTC(void){ return t_SCU.t_Status.u16_ErrorCode; }
uint8 SCU_get_Dev_Num(void){ return t_SCU.t_Status.u_Flags.bits.b_Dev_Num; }
uint8 SCU_get_Send_Error(void){ return t_SCU.u_Errors.bits.b2_CAN_Send; }
uint8 SCU_get_Recived_Error(void){ return t_SCU.u_Errors.bits.b2_CAN_Recived; }
