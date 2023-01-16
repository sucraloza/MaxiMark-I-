#include "OS_Defs.h"

// MutEx
T_x_os_mutex t_Mutex_Tacho_Msg;

// CAN vars
void *mpv_MsgObjTx_Tachograph;
void *mpv_MsgObjRx_Tachograph;
void *mpv_MsgObjRx_TachographBug;

T_x_can_msg t_Tacho_Msg;

void Tachograph_TX_AT1T1(void) // Urea filter, controls panel lights
{
    uint8 u8_UreaLowLevel = UREA_OFF;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FE5600;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0x00;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = (uint8)((u8_UreaLowLevel << 5) & 0xFF);
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_CCVS1_1(void) // Cruise control, controls panel lights
{
    uint8 u8_PTO_Governor_state = 0;
    uint8 u8_Cruise_control_state = 0;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FEF100;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = (uint8)(((u8_Cruise_control_state << 5) | u8_PTO_Governor_state) & 0xFF);
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_DD(void) // Oil diff pressure, controls panel lights
{
    uint8 u8_Fuel_filter_dpressure = 234;
    uint8 u8_Oil_filter_dpressure = 45;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FEFC00;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = u8_Fuel_filter_dpressure & 0xFF;
    t_Msg.au8_Data[3] = u8_Oil_filter_dpressure & 0xFF;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_TCO1(void) // Tachograph velocity and warnings
{
    uint16 u16_Speed_kmh;
    uint8 u8_tacho_performance = 0;
    uint8 u8_tacho_system_event = 0;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x0CFE6CEE;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    u16_Speed_kmh = st_Tachograph.u8_Speed_kmh * 256;

    t_Msg.au8_Data[0] = 0x00;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = (uint8)((((u8_tacho_performance & 0x03) << 4) | (u8_tacho_system_event & 0x03)) & 0xFF);
    // no need to send RPM through this message
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = (uint8)(u16_Speed_kmh & 0x00FF);
    t_Msg.au8_Data[7] = (uint8)((u16_Speed_kmh >> 8) & 0x00FF);
    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_TRF2(void) // Tx overheat indicator
{
    uint8 u8_tx_overheat = 0;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FD9503;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8)((u8_tx_overheat << 4) & 0xFF);
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_WFI(void) // Tx overheat indicator
{
    uint8 u8_water_in_fuel = 0;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FEFF00;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8)((u8_water_in_fuel) & 0xFF);
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_EEC1(void) // Engine speed
{
    uint16 u16_rpm;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0xCF00400;          // source address for Agrale cluster is 0x00
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    u16_rpm = st_Tachograph.u16_Rpm * 8;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = (uint8)(u16_rpm & 0x00FF);
    t_Msg.au8_Data[4] = (uint8)((u16_rpm >> 8) & 0x00FF);
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_ET1(void)
{
    uint8 u8_coolant_temp;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FEEE00;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    u8_coolant_temp = (90 + 40); //Actual value to be parsed here

    t_Msg.au8_Data[0] = (uint8)(u8_coolant_temp & 0xFF);
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}


void Tachograph_TX_EBC1(void) // ABS and ASR signals, controls panel lights
{
    // Assuming signals as independent 8-bit type
    uint8 u8_ASR_engine = 0;
    uint8 u8_ASR_brake = 0;
    uint8 u8_EBS_redsignal = 0;
    uint8 u8_EBS_ambersignal = 0;
    uint8 u8_Engine_retarder_selection = 0;
    uint8 u8_ABS_operational = 0;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18F00100;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint8 temp_data5 =  ((u8_EBS_redsignal & 0x3) << 4) | \
                        ((u8_EBS_ambersignal & 0x3) << 2) | \
                        (u8_ABS_operational & 0x3);

    t_Msg.au8_Data[0] = (uint8)((u8_ASR_engine | (u8_ASR_brake << 2)) & 0xFF);
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = (uint8)(u8_Engine_retarder_selection & 0xFF);
    t_Msg.au8_Data[5] = (uint8)(temp_data5 & 0xFF);
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_EFL_P1_1(void) // Engine oil pressure
{
    uint16 u16_engine_oil_pressure = 1000;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FEEF00;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0x00;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = (uint8)((u16_engine_oil_pressure/4) & 0xFF);
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Tachograph_TX_ETC7(void)
{
    uint8 u8_tx_warning = 0;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FE4A03;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = 0x00;
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = (uint8)((u8_tx_warning << 2) & 0x0C);
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}

void Send_Tachograph(void){
    st_Tachograph.u8_msg_Counter100++;
    if(st_Tachograph.u8_msg_Counter100 >= TACHOGRAPH_COUNTER_LIMIT){     // 0.1sec
        st_Tachograph.u8_msg_Counter100 = 0;
        st_Tachograph.u8_msg_Counter500++;
        Tachograph_TX_TCO1();
        Tachograph_TX_EEC1();
        Tachograph_TX_EBC1();
        Tachograph_TX_CCVS1_1();
        Tachograph_TX_ETC7();
    }
    if(st_Tachograph.u8_msg_Counter500 >= TACHOGRAPH_COUNTER_SLOW_5){    // 0.5sec
        st_Tachograph.u8_msg_Counter500 = 0;
        st_Tachograph.u8_msg_Counter1k++;
        Tachograph_TX_EFL_P1_1();
    }
    if(st_Tachograph.u8_msg_Counter1k >= TACHOGRAPH_COUNTER_SLOW_10){   // 1sec
        st_Tachograph.u8_msg_Counter1k = 0;
        st_Tachograph.u8_msg_Counter10k++;
        Tachograph_TX_AT1T1();
        Tachograph_TX_ET1();
        Tachograph_TX_TRF2();
    }
    if(st_Tachograph.u8_msg_Counter10k >= TACHOGRAPH_COUNTER_SLOW_100){ // 10secs
        st_Tachograph.u8_msg_Counter10k = 0;
        Tachograph_TX_WFI();
    }
}

void Tachograph_RX_CCVS1(void)
{
    if (t_Tacho_Msg.t_Id.u32_Id != 0x18FEF117) return;
    static uint8 u8_maxibreak = 0;
    u8_maxibreak = (t_Tacho_Msg.au8_Data[0] >> 2) & 0x03;

    if(u8_maxibreak == 0){
        un_Tachograph_RxVars.bits.b1_Hand_Break = 0;
        return;
    }
    else if((u8_maxibreak == 0x02) || (u8_maxibreak == 0x03))
    {
        un_Tachograph_RxVars.bits.b1_Hand_Break = 1;
        //This is an error, TBD Fusi event
        return;
    }
    un_Tachograph_RxVars.bits.b1_Hand_Break = 1;
}

void Tachograph_RX_EFL_P1(void)
{
    if (t_Tacho_Msg.t_Id.u32_Id != 0x18FEEF17) return;
    static uint8 u8_engine_coolant = 0;
    u8_engine_coolant = t_Tacho_Msg.au8_Data[7];
    
    if(u8_engine_coolant == 0){
        un_Tachograph_RxVars.bits.b2_Coolant_Level = DS_OFF;
        return;
    }
    un_Tachograph_RxVars.bits.b2_Coolant_Level = DS_ON;
    return;
}

void Tacho_receive(void)
{
    sint16 s16_Error;
    sint16 s16_Error_Can;
    static uint8 u8_contadora = 0;

    s16_Error = x_os_mutex_get(&t_Mutex_Tacho_Msg, 10);
    if (s16_Error != C_NO_ERR)
    {
        Error_Beep();
        return;
    }

/*     s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_TachographBug, &t_Tacho_Msg);
    if (s16_Error_Can != C_NO_ERR)
    {
        Error_Beep();
        return;
    } */

    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Tachograph, &t_Tacho_Msg);

    static T_x_can_bus_status CAN3status;
    static sint16 s16_CAN3err;
    s16_CAN3err = x_can_bus_get_status(X_CAN_BUS_03, &CAN3status);

    static T_x_can_obj_status Rx_Tachograph;
    static sint16 s16_Rx_tachograph;
    s16_Rx_tachograph = x_can_obj_get_status(mpv_MsgObjRx_Tachograph, &Rx_Tachograph);
    if (Rx_Tachograph.u8_OverflowCount){u8_contadora += Rx_Tachograph.u8_OverflowCount;}

    while (s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR)
    {
        debuggeo.flag = 2;
        switch (t_Tacho_Msg.t_Id.u32_Id)
        {
            case 0x18FEF117:
                // x_os_mutex_release(&t_Mutex_Tacho_Msg);
                Tachograph_RX_CCVS1();
                break;
            case 0x18FEEF17:
                // x_os_mutex_release(&t_Mutex_Tacho_Msg);
                Tachograph_RX_EFL_P1();
                break;
            default: x_os_mutex_release(&t_Mutex_Tacho_Msg); break;
        }

        s16_Error = x_os_mutex_get(&t_Mutex_Tacho_Msg, 10); // Trato de obtener el mutex denuevo
        if (s16_Error != C_NO_ERR)
        {
            break; // No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Tachograph, &t_Tacho_Msg);
    }
    x_os_mutex_release(&t_Mutex_Tacho_Msg);
    return;
}

void Tacho_eval(void){
/*     T_x_can_bus_status CAN4status;          sint16 s16_CAN4err;
    s16_CAN4err = x_can_bus_get_status(X_CAN_BUS_04, &CAN4status);
    T_x_can_obj_status Rx_Tachograph;       sint16 s16_Error;
    s16_Error = x_can_obj_get_status(mpv_MsgObjRx_TachographBug, &Rx_Tachograph); */

    sint32 s32_Aux_Var;
    sint16 s16_Error;
    s32_Aux_Var = 0;
    s16_Error = x_in_get_value(GPIO_IN_HTANK,&s32_Aux_Var);

    if(s16_Error || s32_Aux_Var){
        Error_Beepers();
    }
    return;
}

/* void Tachograph_TX_DD(void)
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x018FEFCEE;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint8 u8_SOC = Battery_get_SOC() * 2.5f;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = u8_SOC;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;


    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
} */

/* void Tachograph_TX_VD(void)
{
    uint32 u32_Odometer;
    float32 f32_Odometer_km;
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x018FEE0EE;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    Odometer_Get_Km(&f32_Odometer_km);
    u32_Odometer = f32_Odometer_km * 8.0f;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = (uint8)((u32_Odometer) & 0x00FF);
    t_Msg.au8_Data[5] = (uint8)((u32_Odometer >> 8) & 0x00FF);
    t_Msg.au8_Data[6] = (uint8)((u32_Odometer >> 16) & 0x00FF);
    t_Msg.au8_Data[7] = (uint8)((u32_Odometer >> 24) & 0x00FF);

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
} */

/* void Tachograph_TX_ETC2(void)
{
    uint16 u8_trns_actual;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18F00500;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    u8_trns_actual = (0 - (-125)); //Actual value to be parsed here

    t_Msg.au8_Data[0] = (uint8)(u8_trns_actual & 0xFF);
    t_Msg.au8_Data[1] = 0x00;
    t_Msg.au8_Data[2] = 0x00;
    t_Msg.au8_Data[3] = 0x00;
    t_Msg.au8_Data[4] = 0x00;
    t_Msg.au8_Data[5] = 0x00;
    t_Msg.au8_Data[6] = 0x00;
    t_Msg.au8_Data[7] = 0x00;

    x_can_obj_send_msg(mpv_MsgObjTx_Tachograph, &t_Msg);
}
*/
