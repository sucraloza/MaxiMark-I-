#include "OS_Defs.h"

void *mpv_MsgObjTx_Monitoring; // module global defined TX object for Monitoring
void *mpv_MsgObjRx_Monitoring; // module global defined RX object for Monitoring

T_x_can_msg t_Monitoring_Msg;          //Message Holder RX object
T_x_os_mutex t_Mutex_Monitoring_Msg;


void MON_TX_Int_Ok(uint8 u8_var1, uint8 u8_var2){
    uint32 u32_Int_Id;
    T_x_can_msg t_Msg;
    u32_Int_Id = u8_var1;
    u32_Int_Id = u32_Int_Id << 16;
    t_Msg.t_Id.u32_Id = 0x18008814 + u32_Int_Id;  // From PGN 0 to 5
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = u8_var1;
    t_Msg.au8_Data[1] = u8_var2;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);

}

void MON_TX_Error_Var(sint32 s32_Error){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x14068814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = s32_Error & 0xFF;
    t_Msg.au8_Data[1] = (s32_Error >> 8) & 0xFF;
    t_Msg.au8_Data[2] = (s32_Error >> 16) & 0xFF;
    t_Msg.au8_Data[3] = (s32_Error >> 24) & 0xFF;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_Var16(uint16 u16_Var){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x14078814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = u16_Var & 0xFF;
    t_Msg.au8_Data[1] = (u16_Var >> 8) & 0xFF;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_GPIO_State(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C088814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 6;

    uint16 u16_T15 = GPIO_get_T15_Refs();
    uint16 u16_PDU = GPIO_get_PDU_Refs();
    uint16 u16_Gen = GPIO_get_Gen_Refs();

    t_Msg.au8_Data[0] = (uint8)(u16_T15 & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((u16_T15 >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)(u16_PDU & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((u16_PDU >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8)(u16_Gen & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u16_Gen >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_PDU_Vars(void){
    uint32 u32_Elapsed_Time_ms;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180A8814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    if(st_PDU.PDU_Circuit.u8_Circuit_State == PDU_CIRCUIT_PRECHARGE ||
       st_PDU.PDU_Circuit.u8_Circuit_State == PDU_CIRCUIT_BYPASS ||
       st_PDU.PDU_Circuit.u8_Circuit_State == PDU_CIRCUIT_DISENGAGE ||
       st_PDU.PDU_Circuit.u8_Circuit_State == PDU_CIRCUIT_DISCHARGE)
    {
        u32_Elapsed_Time_ms =  x_sys_get_time_ms() - st_PDU.PDU_Circuit.u32_Initial_Time_ms;
    }
    else{
        u32_Elapsed_Time_ms = 0;
    }
    t_Msg.au8_Data[0] = st_PDU.PDU_Circuit.u8_Circuit_State;
    t_Msg.au8_Data[1] = st_PDU.PDU_Circuit.u8_Circuit_State_Reference;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = (uint8)(u32_Elapsed_Time_ms & 0xFF);
    t_Msg.au8_Data[5] = (uint8)((u32_Elapsed_Time_ms >> 8) & 0xFF);
    t_Msg.au8_Data[6] = (uint8)((u32_Elapsed_Time_ms >> 16) & 0xFF);
    t_Msg.au8_Data[7] = (uint8)((u32_Elapsed_Time_ms >> 24) & 0xFF);


    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_System_Inputs_Vars(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180B8814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint32 u32_Inputs = GPIO_get_Digital_Inputs();

    t_Msg.au8_Data[0] = (uint8)(u32_Inputs & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((u32_Inputs >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((u32_Inputs >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)(GPIO_get_Compresor_Current_uA()*0.001f);
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = (uint8) (EEPROM_get_HVIL_Enable());


    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_System_Status(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180C8814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = st_System.st_State.u8_ECU_State;
    t_Msg.au8_Data[1] = st_System.st_Power_OnOff.u8_Power_On_State;
    t_Msg.au8_Data[2] = st_System.st_Power_OnOff.un_On_Faults.all;
    t_Msg.au8_Data[3] = st_System.st_Power_OnOff.u8_Power_Off_State;
    t_Msg.au8_Data[4] = st_System.st_Power_OnOff.un_Off_Faults.all;;
    t_Msg.au8_Data[5] = Gear_get_State();
    t_Msg.au8_Data[6] = Key_get_State();
    t_Msg.au8_Data[7] = st_System.st_State.u8_Drive_Block & 0x03;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_Throttle(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x180E8814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint8 Throttle_Raw_pu;
    uint8 Throttle_Filtered_pu;
    uint8 Throttle_Post_Hysteresis_pu;
    uint8 Throttle_Post_Ramp_pu;
    sint8 Throttle_Post_Regen_pu;
    sint8 Throttle_Final_pu;

    Throttle_Raw_pu = (uint8)(st_Throttle.f32_Raw_Value_pu * 100);
    Throttle_Filtered_pu = (uint8)(st_Throttle.f64_Filtered_Value_pu * 100);
    Throttle_Post_Hysteresis_pu = (uint8)(st_Throttle.f32_Post_Hysteresis_Value_pu * 100);
    Throttle_Post_Ramp_pu = (uint8)(st_Throttle.f32_Post_Ramp_Value_pu * 100);
    Throttle_Post_Regen_pu = (sint8)(st_Throttle.f32_Post_Regen_Value_pu * 100);
    Throttle_Final_pu = (sint8)(st_Throttle.f32_Final_Value_pu * 100);

    t_Msg.au8_Data[0] = Throttle_Raw_pu;
    t_Msg.au8_Data[1] = Throttle_Filtered_pu;
    t_Msg.au8_Data[2] = Throttle_Post_Hysteresis_pu;
    t_Msg.au8_Data[3] = Throttle_Post_Ramp_pu;
    t_Msg.au8_Data[4] = Throttle_Post_Regen_pu;
    t_Msg.au8_Data[5] = Throttle_Final_pu;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;


    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_Debug(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x140F8814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    

    /* T_x_can_bus_status CAN4status;
    sint16 s16_CAN4err;
    s16_CAN4err = x_can_bus_get_status(X_CAN_BUS_04, &CAN4status);

    T_x_can_obj_status Rx_Tachograph;
    sint16 s16_Rx_tachograph;
    s16_Rx_tachograph = x_can_obj_get_status(mpv_MsgObjRx_Tachograph, &Rx_Tachograph);
    if (Rx_Tachograph.u8_OverflowCount){u16_contadora += Rx_Tachograph.u8_OverflowCount;} */

    t_Msg.au8_Data[0] = debuggeo.flag;
    t_Msg.au8_Data[1] = debuggeo.u8_debug4;
    t_Msg.au8_Data[2] = debuggeo.u8_debug5;
    t_Msg.au8_Data[3] = debuggeo.u8_debug6;
    t_Msg.au8_Data[4] = debuggeo.u8_debug7;
    t_Msg.au8_Data[5] = debuggeo.u8_debug1;
    t_Msg.au8_Data[6] = debuggeo.u8_debug2;
    t_Msg.au8_Data[7] = 0;


    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_HVIL(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18108814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 3;

    t_Msg.au8_Data[0] = st_System.st_HVIL.u8_Reference;
    t_Msg.au8_Data[1] = st_System.st_HVIL.u8_Operation;
    t_Msg.au8_Data[2] = st_System.st_HVIL.u8_Feedback;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_Throttle_1(float64 f64_In_1,float64 f64_In_2,float64 f64_In_3,float64 f64_In_4){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18118814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_In_1,u16_In_2,u16_In_3,u16_In_4;
    u16_In_1 = (uint16) ((f64_In_1 + 1.0f) * 10000);
    u16_In_2 = (uint16) ((f64_In_2 + 1.0f) * 10000);
    u16_In_3 = (uint16) ((f64_In_3 + 1.0f) * 10000);
    u16_In_4 = (uint16) ((f64_In_4 + 1.0f) * 10000);

    t_Msg.au8_Data[0] = (uint8) (u16_In_1 & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((u16_In_1 >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) (u16_In_2 & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((u16_In_2 >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) (u16_In_3 & 0xFF);
    t_Msg.au8_Data[5] = (uint8) ((u16_In_3 >> 8) & 0xFF);
    t_Msg.au8_Data[6] = (uint8) (u16_In_4 & 0xFF);
    t_Msg.au8_Data[7] = (uint8) ((u16_In_4 >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_Steering(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18128814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    Steering_Config_Monitoring_msg(&t_Msg);
    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
// BTMS Feedback
void MON_TX_BTMS(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18138814;
    t_Msg.t_Id.u8_Xtd = TRUE;

    BTMS_Get_Monitoring_Data(&t_Msg);
    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void MON_TX_TPDU(void){
    T_x_can_msg t_Msg;
    t_Msg = TPDU_Get_Monitoring();
    t_Msg.t_Id.u32_Id = 0x18148814;
    t_Msg.t_Id.u8_Xtd = TRUE;
    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}


void MON_RX_Odometer(void)
{
    if (t_Monitoring_Msg.t_Id.u32_Id != 0x1C011488u) return;

    uint32 u32_km = 0;
    u32_km |= t_Monitoring_Msg.au8_Data[3];
    u32_km = u32_km << 8;
    u32_km |= t_Monitoring_Msg.au8_Data[2];
    u32_km = u32_km << 8;
    u32_km |= t_Monitoring_Msg.au8_Data[1];
    u32_km = u32_km << 8;
    u32_km |= t_Monitoring_Msg.au8_Data[0];

    float32 f32_km = u32_km *0.1f;
    Odometer_Set_Km(f32_km);
}

void MON_RX_GPIO_Command(void)
{
    if (t_Monitoring_Msg.t_Id.u32_Id != 0x1C001488u) return;

    if(t_Monitoring_Msg.au8_Data[7] == ECU_STATE_GPIO_CHECK){
        st_Monitoring.st_Monitoring_Control_1.u8_Auxiliary_ECU_State = ECU_STATE_GPIO_CHECK;
    }

    if(t_Monitoring_Msg.au8_Data[7] == ECU_STATE_SLEEP){
        st_Monitoring.st_Monitoring_Control_1.u8_Auxiliary_ECU_State = ECU_STATE_SLEEP;
    }


    if (System_get_State() != ECU_STATE_GPIO_CHECK) return;

    if ((t_Monitoring_Msg.au8_Data[0] & 0x01) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_BCU = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_BCU = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[0] & 0x02) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_INV = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_INV = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[0]& 0x04) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_Screen = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_Screen = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[0]& 0x08) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_Digi = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_Digi = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[0]& 0x10) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_Steering = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_Steering = DS_ON;
    }


    if ((t_Monitoring_Msg.au8_Data[0]& 0x20) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_BTMS = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_BTMS = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[0]& 0x40) == 0x00){
        st_GPIO.un_Outs_T15.bits.b2_T15_Buzzer = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_T15.bits.b2_T15_Buzzer = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[0]& 0x80) == 0x00){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Discharge = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Discharge = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[1]& 0x01) == 0x00){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Precharge = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Precharge = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[1]& 0x02) == 0x00){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Main = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Main = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[1]& 0x04) == 0x00){
       st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_AC = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_AC = DS_ON;
    }

/*      if ((t_Monitoring_Msg.au8_Data[1]& 0x08) == 0x00){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Steering = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Steering = DS_ON;
    }  */

    if ((t_Monitoring_Msg.au8_Data[1]& 0x10) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_HVIL = DS_OFF;
    }
    else{
        HVIL_Activate();
        st_GPIO.un_Outs_Gen.bits.b2_HVIL = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[1]& 0x20) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_TPDU_Compresor = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_TPDU_Compresor = DS_ON;
    }


    if ((t_Monitoring_Msg.au8_Data[1]& 0x40) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_TPDU_AC = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_TPDU_AC = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[1]& 0x80) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_Dapig_Override = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_Dapig_Override = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[2]& 0x01) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_Cooling_Tank = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_Cooling_Tank = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[2]& 0x02) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_Heating_Tank = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_Heating_Tank = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[2]& 0x04) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_Steering_Tank = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_Steering_Tank = DS_ON;
    }

    if ((t_Monitoring_Msg.au8_Data[2]& 0x08) == 0x00){
        st_GPIO.un_Outs_Gen.bits.b2_Steering_Contactor = DS_OFF;
    }
    else{
        st_GPIO.un_Outs_Gen.bits.b2_Steering_Contactor = DS_ON;
    }

}
void MON_Tx_Service_Air(void){
    T_x_can_msg t_Msg;
    Service_Air_Monitoring(&t_Msg);
    t_Msg.t_Id.u32_Id = 0x18168814;
    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);

}


void MON_Tx_AC_Comp(void){
    T_x_can_msg t_Msg;
    AC_Comp_Monitoring(&t_Msg);
    t_Msg.t_Id.u32_Id = 0x18178814;
    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);

}
// New
// ---------------- BTMS Manual Control PC -----------------
void MON_PC_Command_BTMS_Rx(void)
{
    if(t_Monitoring_Msg.t_Id.u32_Id != 0x1C021488){
        return;
    }

    st_Monitoring.st_Monitoring_BTMS.u8_BTMS_OnOff = t_Monitoring_Msg.au8_Data[0];
    st_Monitoring.st_Monitoring_BTMS.u8_BTMS_Work_Status = t_Monitoring_Msg.au8_Data[1];
    st_Monitoring.st_Monitoring_BTMS.u8_BTMS_Ref_Temp = t_Monitoring_Msg.au8_Data[2];
    st_Monitoring.st_Monitoring_BTMS.u8_BTMS_Manual_Control = t_Monitoring_Msg.au8_Data[3] & 0x1;

    if(x_os_mutex_get(&t_Mutex_Monitoring_Msg,10) != C_NO_ERR){
        return;
    }

    x_os_mutex_release(&t_Mutex_Monitoring_Msg);
}


void Monitoring_Recived(void){
    sint16 s16_Error;
    sint16 s16_Error_Can;
    s16_Error = x_os_mutex_get(&t_Mutex_Monitoring_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Monitoring, &t_Monitoring_Msg);
    while(s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR){
        switch(t_Monitoring_Msg.t_Id.u32_Id){
            case 0x1C011488u: MON_RX_Odometer(); break;
            case 0x1C001488u: MON_RX_GPIO_Command(); break;
            case 0x1C021488u: MON_PC_Command_BTMS_Rx(); break; //New
            default:
                x_os_mutex_release(&t_Mutex_Monitoring_Msg);
        }
        s16_Error = x_os_mutex_get(&t_Mutex_Monitoring_Msg,10);    //Trato de obtener el mutex denuevo
        if(s16_Error != C_NO_ERR){
            break;                                              //No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Monitoring, &t_Monitoring_Msg);
    }
    x_os_mutex_release(&t_Mutex_Monitoring_Msg);
}

void Monitoring_Send(void){
    uint8 u8_Turn;
    uint8 u8_Turn_Limit;
    u8_Turn = 0;
    u8_Turn_Limit = st_Monitoring.st_Monitoring_Control_1.u8_Number_Turns_per_Cycle;
    if(st_Monitoring.st_Monitoring_Control_1.u8_Fix_Message_Flag){
        u8_Turn_Limit = 1;
        st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn= st_Monitoring.st_Monitoring_Control_1.u16_Fixed_PNG;
    }
    MON_Tx_Service_Air();
    MON_Tx_AC_Comp();

    while(u8_Turn < u8_Turn_Limit){
        switch(st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn){
        case 0x0800:
            MON_TX_GPIO_State();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0A00;
            break;
        case 0x0A00:
            MON_TX_PDU_Vars();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0B00;
            break;
        case 0x0B00:
            MON_TX_System_Inputs_Vars();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0C00;
            break;
        case 0x0C00:
            MON_TX_System_Status();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0D00;
            break;
        case 0x0D00:
            //Now used by MaxiMark Debug
            MON_TX_Debug();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0E00;
            break;
        case 0x0E00:
            MON_TX_Throttle();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x1000;
            break;
         case 0x1000:
            MON_TX_HVIL();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x1200;
            break;
        case 0x1200:
            MON_TX_Steering();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x1300;
            break;
        case 0x1300:
            MON_TX_BTMS();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x1400;
            break;
        case 0x1400:
            MON_TX_TPDU();
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x1500;
            break;
        case 0x1500:
            Compresor_Monitor(0x18158814u,mpv_MsgObjTx_Monitoring);
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0800;
            break;
        default:
            st_Monitoring.st_Monitoring_Control_1.u16_PNG_Send_Turn = 0x0800;
            st_Monitoring.st_Monitoring_Control_1.u8_Fix_Message_Flag = FALSE;
        }
        u8_Turn++;
    }

}
