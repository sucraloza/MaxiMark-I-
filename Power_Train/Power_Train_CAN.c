#include "OS_Defs.h"

//Configuracion protocolo de Comunicacion
void Inverter_Tx_Protocol(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = INV_CAN_BASEADDR_LOW + 4;
    t_Msg.t_Id.u8_Xtd = FALSE;

    t_Msg.u8_Dlc = 2;
    t_Msg.au8_Data[0]= 4;
    t_Msg.au8_Data[1]= 4;
    Inverter_Safe_Send(x_can_obj_send_msg(mpv_MsgObjTx_Inverter, &t_Msg));
}

void Inverter_Tx_VmuPullingVmuResp(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = INV_CAN_BASEADDR_LOW + 6;
    t_Msg.t_Id.u8_Xtd = FALSE;

    t_Msg.u8_Dlc = 8;
    t_Msg.au8_Data[0]= 0;
    t_Msg.au8_Data[1]= (uint8)  (st_Inverter.st_Coms.u16_VmuBootNumber & 0x00FF);
    t_Msg.au8_Data[2]= (uint8) ((st_Inverter.st_Coms.u16_VmuBootNumber & 0xFF00)>> 8);
    t_Msg.au8_Data[3]= 0;
    t_Msg.au8_Data[4]= 0;
    t_Msg.au8_Data[5]= 0;
    t_Msg.au8_Data[6]= 0;
    t_Msg.au8_Data[7]= 0;
    Inverter_Safe_Send(x_can_obj_send_msg(mpv_MsgObjTx_Inverter, &t_Msg));
}

void Inverter_Rx_McuPullingVmu(void){
}

void Inverter_Rx_McuPullingMcuResp(void){
    sint16 s16_error;
    uint8 u8_Aux_Ack;
    uint8 u8_Aux_RequestId;

    s16_error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_LOW + 8){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    u8_Aux_RequestId = t_Inverter_Msg.au8_Data[0] & 0x7F;
    u8_Aux_Ack = t_Inverter_Msg.au8_Data[0] & 0x80;

    if(u8_Aux_RequestId!=0 || u8_Aux_Ack != 0x80){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    s16_error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_error != C_NO_ERR){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    st_Inverter.st_Coms.u8_Mayor_Protocol = t_Inverter_Msg.au8_Data[1];
    st_Inverter.st_Coms.u8_Minor_Protocol = t_Inverter_Msg.au8_Data[2];
    st_Inverter.st_Coms.u8_Mayor_Extension = t_Inverter_Msg.au8_Data[3];
    st_Inverter.st_Coms.u8_Minor_Extension = t_Inverter_Msg.au8_Data[4];
    st_Inverter.un_State.bits.b1_Protocol_Set = TRUE;

    x_os_mutex_release(&t_Mutex_Inverter);
    x_os_mutex_release(&t_Mutex_Inverter_Msg);

}

//Mensajes periodicos que se le debe enviar al Inv
void Inverter_Tx_VmuCommand1(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = INV_CAN_BASEADDR_HIGH + 0;
    t_Msg.t_Id.u8_Xtd = FALSE;

    t_Msg.u8_Dlc = 8;
    t_Msg.au8_Data[0] = (uint8) st_Inverter.st_Limits.u16_Max_Charge_Current_A & 0x00FF;

    t_Msg.au8_Data[1] = ((uint8) ((st_Inverter.st_Limits.u16_Max_Charge_Current_A & 0x0F00)>>8));
    t_Msg.au8_Data[1]|=  ((st_Inverter.st_Refs.un_Flags.bits.b2_Operation_State & 0x07)<<5);

    t_Msg.au8_Data[2] = (uint8) st_Inverter.st_Limits.u16_Max_Discharge_Current_A & 0x00FF;
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.st_Limits.u16_Max_Discharge_Current_A & 0x0F00)>>8);
    t_Msg.au8_Data[4] = (uint8) st_Inverter.st_Limits.u16_Max_Battery_Voltage_V & 0x00FF;

    t_Msg.au8_Data[5] = ((uint8) ((st_Inverter.st_Limits.u16_Max_Battery_Voltage_V & 0x0F00)>>8));
    t_Msg.au8_Data[5] |=  ((st_Inverter.st_Limits.un_Flags.all & 0x0F)<<4);

    t_Msg.au8_Data[6] = (uint8) st_Inverter.st_Limits.u16_Min_Battery_Voltage_V & 0x00FF;

    t_Msg.au8_Data[7] = ((uint8) ((st_Inverter.st_Limits.u16_Min_Battery_Voltage_V & 0x0F00)>>8));
    t_Msg.au8_Data[7] |=  ((st_Inverter.st_Coms.u8_VmuCommand1_RollCnt & 0x0F)<<4);

    Inverter_Safe_Send(x_can_obj_send_msg(mpv_MsgObjTx_Inverter, &t_Msg));

    st_Inverter.st_Coms.u8_VmuCommand1_RollCnt++;
    if(st_Inverter.st_Coms.u8_VmuCommand1_RollCnt >= 8)
        st_Inverter.st_Coms.u8_VmuCommand1_RollCnt = 0;
}

void Inverter_Tx_VmuCommand2(void){
    uint16 u16_Torque_Req_nm;
    uint16 u16_Speed_Req_rpm;
    uint8 u8_Torque_ok;
    uint8 u8_Speed_ok;

    u8_Torque_ok = st_Inverter.st_Refs.un_Flags.bits.b1_Torque_Valid;
    u8_Speed_ok = st_Inverter.st_Refs.un_Flags.bits.b1_Speed_Valid;

    u16_Torque_Req_nm = (uint16) floor((st_Inverter.st_Refs.f32_Torque_nm + 4096)*8);

    if(st_Inverter.st_Refs.f32_Torque_nm > 4090 || st_Inverter.st_Refs.f32_Torque_nm < -4090){
        u8_Torque_ok = FALSE;
        u16_Torque_Req_nm = 32768;      //TODO Revisar
    }

    if(u16_Torque_Req_nm <= 47){
        u16_Torque_Req_nm = 48;
    }
    if(u16_Torque_Req_nm >= 65528){
        u16_Torque_Req_nm = 65527;
    }

    if(st_Inverter.st_Refs.s16_Speed_rpm > 32760 || st_Inverter.st_Refs.s16_Speed_rpm < -32760){
        u8_Speed_ok = FALSE;
        u16_Speed_Req_rpm = 32768;      //TODO Revisar logica
    }
    if(st_Inverter.st_Refs.s16_Speed_rpm<0)
        u16_Speed_Req_rpm = (uint16)(st_Inverter.st_Refs.s16_Speed_rpm + 32767);
    else
        u16_Speed_Req_rpm = ((uint16)st_Inverter.st_Refs.s16_Speed_rpm) + 32767;

    if(u16_Speed_Req_rpm <= 6){
        u16_Speed_Req_rpm = 7;
    }
    if(u16_Speed_Req_rpm >= 65528){
        u16_Speed_Req_rpm = 65527;
    }

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = INV_CAN_BASEADDR_HIGH + 1;
    t_Msg.t_Id.u8_Xtd = FALSE;

    t_Msg.u8_Dlc = 6;

    t_Msg.au8_Data[0] = (st_Inverter.st_Refs.un_Flags.bits.b2_Operation_Mode & 0x07);
    t_Msg.au8_Data[0] |= ((st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode & 0x03)<<3);
    t_Msg.au8_Data[0] |= ((u8_Speed_ok & 0x01)<<5);
    t_Msg.au8_Data[0] |= ((u8_Torque_ok & 0x01)<<6);

    t_Msg.au8_Data[1]= (uint8)((u16_Torque_Req_nm & 0x00FF));
    t_Msg.au8_Data[2]= (uint8)((u16_Torque_Req_nm & 0xFF00)>>8);
    t_Msg.au8_Data[3]= (uint8)((u16_Speed_Req_rpm & 0x00FF));
    t_Msg.au8_Data[4]= (uint8)((u16_Speed_Req_rpm & 0xFF00)>>8);
    t_Msg.au8_Data[5]= (st_Inverter.st_Coms.u8_VmuCommand2_RollCnt & 0x0F)<<4;

    Inverter_Safe_Send(x_can_obj_send_msg(mpv_MsgObjTx_Inverter, &t_Msg));

    st_Inverter.st_Coms.u8_VmuCommand2_RollCnt++;
    if(st_Inverter.st_Coms.u8_VmuCommand2_RollCnt >= 8){
        st_Inverter.st_Coms.u8_VmuCommand2_RollCnt = 0;
    }
}

void Inverter_Tx_VmuCommandSafety(void){
    uint16 u16_Torque_Req_nm;
    uint16 u16_Speed_Req_rpm;
    uint8 u8_Torque_ok;
    uint8 u8_Speed_ok;

    u8_Torque_ok = st_Inverter.st_Refs.un_Flags.bits.b1_Torque_Valid;
    u8_Speed_ok = st_Inverter.st_Refs.un_Flags.bits.b1_Speed_Valid;

    u16_Torque_Req_nm = (uint16) floor((st_Inverter.st_Refs.f32_Safe_Torque_nm + 4096)*8);

    if(st_Inverter.st_Refs.f32_Safe_Torque_nm > 4090 || st_Inverter.st_Refs.f32_Safe_Torque_nm < -4090){
        u8_Torque_ok = FALSE;
        u16_Torque_Req_nm = 32768;      //TODO Revisar
    }

    if(u16_Torque_Req_nm <= 47){
        u16_Torque_Req_nm = 48;
    }
    if(u16_Torque_Req_nm >= 65528){
        u16_Torque_Req_nm = 65527;
    }

    if(st_Inverter.st_Refs.s16_Safe_Speed_rpm > 32760 || st_Inverter.st_Refs.s16_Safe_Speed_rpm < -32760){
        u8_Speed_ok = FALSE;
        u16_Speed_Req_rpm = 32768;      //TODO Revisar logica
    }
    if(st_Inverter.st_Refs.s16_Safe_Speed_rpm<0)
        u16_Speed_Req_rpm = (uint16)(st_Inverter.st_Refs.s16_Safe_Speed_rpm+ 32767);
    else
        u16_Speed_Req_rpm = ((uint16)st_Inverter.st_Refs.s16_Safe_Speed_rpm) + 32767;

    if(u16_Speed_Req_rpm <= 6){
        u16_Speed_Req_rpm = 7;
    }
    if(u16_Speed_Req_rpm >= 65528){
        u16_Speed_Req_rpm = 65527;
    }

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = INV_CAN_BASEADDR_HIGH + 2;
    t_Msg.t_Id.u8_Xtd = FALSE;

    t_Msg.u8_Dlc = 5;
    t_Msg.au8_Data[0]= (uint8) (u16_Torque_Req_nm & 0x00FF);
    t_Msg.au8_Data[1]= (uint8) ((u16_Torque_Req_nm & 0xFF00)>>8);
    t_Msg.au8_Data[2]= (uint8) (u16_Speed_Req_rpm & 0x00FF);
    t_Msg.au8_Data[3]= (uint8) ((u16_Speed_Req_rpm & 0xFF00)>>8);
    t_Msg.au8_Data[4]= (st_Inverter.st_Refs.un_Flags.bits.b2_Command_Mode & 0x03) | ((st_Inverter.st_Coms.u8_VmuCommandSafety_RollCnt & 0x0F)<<4);

    Inverter_Safe_Send(x_can_obj_send_msg(mpv_MsgObjTx_Inverter, &t_Msg));
    st_Inverter.st_Coms.u8_VmuCommandSafety_RollCnt++;
    if(st_Inverter.st_Coms.u8_VmuCommandSafety_RollCnt >= 8)
        st_Inverter.st_Coms.u8_VmuCommandSafety_RollCnt = 0;
}

//Mensajes que llegan a la ECU desde el Inv
void Inverter_Rx_McuCommand1Resp(void){
    sint16 s16_Error;
    uint8 u8_Operational_State;
    uint8 u8_Operational_Mode;
    uint8 u8_Command_Mode;
    uint8 u8_Regen_Derrating;
    uint8 u8_Drive_Derrating;
    uint16 u16_Max_Foward_Speed;
    uint16 u16_Max_Reverse_Speed;
    uint8 u8_Foward_Speed_Valid;
    uint8 u8_Reverse_Speed_Valid;
    uint8 u8_Regen_Derrating_Valid;
    uint8 u8_Drive_Derrating_Valid;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_HIGH + 3){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    u8_Operational_State = t_Inverter_Msg.au8_Data[0] & 0x0F;
    u8_Operational_Mode = (t_Inverter_Msg.au8_Data[0] & 0x70)>>4;
    u8_Regen_Derrating_Valid = (t_Inverter_Msg.au8_Data[0] & 0x80)>>7;
    u8_Regen_Derrating = t_Inverter_Msg.au8_Data[1] & 0x0F;
    u8_Drive_Derrating = (t_Inverter_Msg.au8_Data[1] & 0xF0)>>4;

    u8_Foward_Speed_Valid = (t_Inverter_Msg.au8_Data[5] & 0x02)>>1;
    u8_Reverse_Speed_Valid = (t_Inverter_Msg.au8_Data[5] & 0x04)>>2;
    u8_Drive_Derrating_Valid = (t_Inverter_Msg.au8_Data[5] & 0x08)>>3;

    u16_Max_Foward_Speed = (((uint16)(t_Inverter_Msg.au8_Data[3] & 0x01))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[2]));
    if(u16_Max_Foward_Speed >= 329){
       u16_Max_Foward_Speed = 328;
       u8_Foward_Speed_Valid = FALSE;
    }
    u16_Max_Foward_Speed = u16_Max_Foward_Speed*100;

    u8_Command_Mode =  (t_Inverter_Msg.au8_Data[3] >> 1) & 0x07;

    u16_Max_Reverse_Speed = (((uint16)(t_Inverter_Msg.au8_Data[5] & 0x01))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[4]));
    if(u16_Max_Reverse_Speed >= 329){
       u16_Max_Reverse_Speed = 328;
       u8_Reverse_Speed_Valid = FALSE;
    }
    u16_Max_Reverse_Speed = u16_Max_Reverse_Speed*100;

    x_os_mutex_release(&t_Mutex_Inverter_Msg);

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    st_Inverter.un_State.bits.b4_Operational_State = u8_Operational_State;
    st_Inverter.un_State.bits.b3_Operational_Mode = u8_Operational_Mode;
    st_Inverter.un_State.bits.b2_Command_Mode  = u8_Command_Mode;

    if(u8_Regen_Derrating_Valid == 0)
        st_Inverter.un_State.bits.b4_Regen_Derraiting = u8_Regen_Derrating;
    if(u8_Drive_Derrating_Valid == 0)
        st_Inverter.un_State.bits.b4_Traction_Derraiting = u8_Drive_Derrating;


    if(u8_Foward_Speed_Valid == 0)
        st_Inverter.st_Mechanical.u16_Max_Foward_Speed_rpm = u16_Max_Foward_Speed;
    if(u8_Reverse_Speed_Valid == 0)
        st_Inverter.st_Mechanical.u16_Max_Reverse_Speed_rpm = u16_Max_Reverse_Speed;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Rx_McuCommand2Resp(void){
    sint16 s16_Error;
    sint16 s16_Motor_Speed;
    uint16 u16_Motor_Angle;
    uint8 u8_Emergency_Stop;
    uint8 u8_Motor_Speed_Valid;
    uint8 u8_Motor_Angle_Valid;
    float32 f32_Speed_kmh;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_HIGH + 4){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }
    //Esto Funciona
    s16_Motor_Speed =(((((sint16)(t_Inverter_Msg.au8_Data[1]))<<8) | ((sint16)(t_Inverter_Msg.au8_Data[0]))) - 32767);
    u16_Motor_Angle = ((((uint16)(t_Inverter_Msg.au8_Data[3]))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[2])));

    u8_Motor_Angle_Valid = t_Inverter_Msg.au8_Data[4] & 0x01;
    u8_Motor_Speed_Valid = (t_Inverter_Msg.au8_Data[4] & 0x06 )>>1;
    u8_Emergency_Stop = (t_Inverter_Msg.au8_Data[4] & 0x08 )>>3;

    x_os_mutex_release(&t_Mutex_Inverter_Msg);

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    f32_Speed_kmh = s16_Motor_Speed * SPEED_FACTOR;

    if(u8_Motor_Angle_Valid == 0)
        st_Inverter.st_Mechanical.u16_Motor_Angle = u16_Motor_Angle;
    if(u8_Motor_Speed_Valid == 0){
        st_Inverter.st_Mechanical.s16_Speed_rpm = s16_Motor_Speed;
        st_Inverter.st_Mechanical.f32_Avg_Speed_kmh = 0.9934f * st_Inverter.st_Mechanical.f32_Avg_Speed_kmh + 0.0066f * f32_Speed_kmh;
        if(f32_Speed_kmh > st_Inverter.st_Mechanical.s8_Max_Speed_kmh){
            st_Inverter.st_Mechanical.s8_Max_Speed_kmh = f32_Speed_kmh;
        }

    }

    st_Inverter.st_Fauts.un_Flags.bits.b1_Emergency_Stop = u8_Emergency_Stop;

    x_os_mutex_release(&t_Mutex_Inverter);

    if(u8_Motor_Speed_Valid == 0){
        Inverter_Filter_Speed();
    }
}

void Inverter_Rx_McuInfo1(void){
    sint16 s16_Error;
    float32 f32_Power_Current;
    uint16 u16_Power_Voltage;
    float32 f32_Auxiliary_Voltage;
    uint8 u8_Power_Current_Valid;
    uint8 u8_Power_Voltage_Valid;
    uint8 u8_Auxiliary_Voltage_Valid;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_LOW + 0){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    f32_Auxiliary_Voltage = t_Inverter_Msg.au8_Data[0] * 0.25f;
    u16_Power_Voltage = (((uint16)(t_Inverter_Msg.au8_Data[2] & 0x07))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[1]));
    u8_Power_Current_Valid = (t_Inverter_Msg.au8_Data[2] & 0x08) >> 3;
    u8_Auxiliary_Voltage_Valid = (t_Inverter_Msg.au8_Data[2] & 0x10) >> 4;
    u8_Power_Voltage_Valid = (t_Inverter_Msg.au8_Data[2] & 0x20) >> 5;

    f32_Power_Current = (((((uint16)(t_Inverter_Msg.au8_Data[4]&0x3F))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[3]))) * 0.25) - 2048.0f;

    x_os_mutex_release(&t_Mutex_Inverter_Msg);

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(u8_Power_Current_Valid == 0 && f32_Power_Current >= -2046.0f && f32_Power_Current <= 2046.0f){
        st_Inverter.st_Electrical.f32_Power_Current_A = f32_Power_Current;
        st_Inverter.st_Electrical.f32_Avg_Current_A = 0.0066f * f32_Power_Current + 0.9934f *st_Inverter.st_Electrical.f32_Avg_Current_A;
    }
    if(u8_Auxiliary_Voltage_Valid == 0)
        st_Inverter.st_Electrical.f32_Auxiliary_Voltage_V = f32_Auxiliary_Voltage;
    if(u8_Power_Voltage_Valid == 0 && u16_Power_Voltage <= 2046)
        st_Inverter.st_Electrical.u16_Power_Voltage_V = u16_Power_Voltage;


    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Rx_McuInfo2(void){
    sint16 s16_Error;
    uint16 u16_Estimated_Torque;
    uint16 u16_Avalible_Drive_Torque;
    uint16 u16_Avalible_Regen_Torque;
    uint8 u8_Estimated_Torque_Valid;
    uint8 u8_Avalible_Drive_Torque_Valid;
    uint8 u8_Avalible_Regen_Torque_Valid;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_LOW + 1){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    u8_Estimated_Torque_Valid = (t_Inverter_Msg.au8_Data[6] & 0x02) >> 1;
    u8_Avalible_Drive_Torque_Valid = (t_Inverter_Msg.au8_Data[6] & 0x04) >> 2;
    u8_Avalible_Regen_Torque_Valid = (t_Inverter_Msg.au8_Data[6] & 0x08) >> 3;

    u16_Estimated_Torque = (((uint16)(t_Inverter_Msg.au8_Data[1]))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[0]));
    u16_Avalible_Drive_Torque = ((((uint16)(t_Inverter_Msg.au8_Data[3] & 0x7F))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[2])))>>3;
    u16_Avalible_Regen_Torque = ((((uint16)(t_Inverter_Msg.au8_Data[5] & 0x7F))<<8) | ((uint16)(t_Inverter_Msg.au8_Data[4])))>>3;

    x_os_mutex_release(&t_Mutex_Inverter_Msg);

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(u8_Estimated_Torque_Valid == 0 ){
        st_Inverter.st_Mechanical.f32_Estimated_Torque_nm = (u16_Estimated_Torque * 0.125f)- 4096;
        st_Inverter.st_Mechanical.f32_Avg_Torque_nm = 0.0066f * st_Inverter.st_Mechanical.f32_Estimated_Torque_nm  + 0.9934f *st_Inverter.st_Mechanical.f32_Avg_Torque_nm;
        if(st_Inverter.st_Mechanical.f32_Estimated_Torque_nm > st_Inverter.st_Mechanical.f32_Max_Meesured_Torque_nm){
            st_Inverter.st_Mechanical.f32_Max_Meesured_Torque_nm = st_Inverter.st_Mechanical.f32_Estimated_Torque_nm;
        }
        if(st_Inverter.st_Mechanical.f32_Estimated_Torque_nm < st_Inverter.st_Mechanical.f32_Min_Meesured_Torque_nm){
            st_Inverter.st_Mechanical.f32_Min_Meesured_Torque_nm = st_Inverter.st_Mechanical.f32_Estimated_Torque_nm;
        }
    }
    if(u8_Avalible_Drive_Torque_Valid == 0 && u16_Avalible_Drive_Torque <= 4090)
        st_Inverter.st_Mechanical.u16_Drive_Torque_nm = u16_Avalible_Drive_Torque;
    if(u8_Avalible_Regen_Torque_Valid == 0 && u16_Avalible_Regen_Torque <= 4090)
        st_Inverter.st_Mechanical.u16_Regen_Torque_nm = u16_Avalible_Regen_Torque;

    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Rx_McuEventInfo1(void){
    sint16 s16_Error;
    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_LOW + 2){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    if((t_Inverter_Msg.au8_Data[0] & 0x10) == 0x00){
        st_Inverter.st_Fauts.un_Flags.bits.b1_Isolation = t_Inverter_Msg.au8_Data[0] & 0x01;
    }

    if((t_Inverter_Msg.au8_Data[0] & 0x20) == 0x00){
        st_Inverter.st_Fauts.un_Flags.bits.b1_Battery_Interlock = (t_Inverter_Msg.au8_Data[0] & 0x02) >>1;
    }

    if((t_Inverter_Msg.au8_Data[0] & 0x40) == 0x00){
        st_Inverter.st_Fauts.un_Flags.bits.b1_Motor_Interlock = (t_Inverter_Msg.au8_Data[0] & 0x04) >>2;
    }

    if((t_Inverter_Msg.au8_Data[0] & 0x80) == 0x00){
        st_Inverter.st_Fauts.un_Flags.bits.b1_Cover_Open = (t_Inverter_Msg.au8_Data[0] & 0x08) >>3;
    }

    x_os_mutex_release(&t_Mutex_Inverter_Msg);
    x_os_mutex_release(&t_Mutex_Inverter);
}

void Inverter_Rx_McuOnEventInfo1(void){
    sint16 s16_Error;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_LOW + 3){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    st_Inverter.st_Fauts.u16_Inv_Event_Id = (((uint16) t_Inverter_Msg.au8_Data[1])<<8) | ((uint16) t_Inverter_Msg.au8_Data[0]);

    x_os_mutex_release(&t_Mutex_Inverter);
    x_os_mutex_release(&t_Mutex_Inverter_Msg);
}

void Inverter_Rx_McuThermal1(void){
    sint16 s16_Error;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }

    if(t_Inverter_Msg.t_Id.u32_Id != INV_CAN_BASEADDR_LOW + 9){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter,10);
    if(s16_Error != C_NO_ERR){
        x_os_mutex_release(&t_Mutex_Inverter_Msg);
        return;
    }

    if((t_Inverter_Msg.au8_Data[3] & 0x01) == 0x00){
        if(t_Inverter_Msg.au8_Data[0] < 129){
            st_Inverter.st_Cooling.f32_Inv_Required_Flow_pu = t_Inverter_Msg.au8_Data[0] * 0.78125f * 0.01;
        }
    }

    if((t_Inverter_Msg.au8_Data[3] & 0x02) == 0x00){
        if(t_Inverter_Msg.au8_Data[1] < 129){
            st_Inverter.st_Cooling.f32_Motor_Required_Flow_pu = t_Inverter_Msg.au8_Data[1] * 0.78125f * 0.01;
        }
    }

    if((t_Inverter_Msg.au8_Data[3] & 0x04) == 0x00){
        if(t_Inverter_Msg.au8_Data[2] < 129){
            st_Inverter.st_Cooling.u8_Thermal_Indicator_pu =  (uint8) floor(t_Inverter_Msg.au8_Data[2] * 0.78125f);
        }
    }

    x_os_mutex_release(&t_Mutex_Inverter);
    x_os_mutex_release(&t_Mutex_Inverter_Msg);
}

void Inverter_Received(void){
    sint16 s16_Error;
    sint16 s16_Error_Can;

    s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);
    if(s16_Error != C_NO_ERR){
        return;
    }
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Inverter, &t_Inverter_Msg);
    while(s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR){
        switch(t_Inverter_Msg.t_Id.u32_Id){
            case 0x248:
                AC_Comp_Feedback_Interface(&t_Inverter_Msg);
                break;
            case INV_CAN_BASEADDR_HIGH + 3:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuCommand1Resp();
                break;
            case INV_CAN_BASEADDR_HIGH + 4:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuCommand2Resp();
                break;
            case INV_CAN_BASEADDR_LOW + 0:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuInfo1();
                break;
            case INV_CAN_BASEADDR_LOW + 1:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuInfo2();
                break;
            case INV_CAN_BASEADDR_LOW + 2:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuEventInfo1();
                break;
            case INV_CAN_BASEADDR_LOW + 3:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuOnEventInfo1();
                break;
            case INV_CAN_BASEADDR_LOW + 8:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuPullingMcuResp();
                break;
            case INV_CAN_BASEADDR_LOW + 9:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
                Inverter_Rx_McuThermal1();
                break;
            default:
                x_os_mutex_release(&t_Mutex_Inverter_Msg);
        }
        s16_Error = x_os_mutex_get(&t_Mutex_Inverter_Msg,10);    //Trato de obtener el mutex denuevo
        if(s16_Error != C_NO_ERR){
            break;                                              //No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Inverter, &t_Inverter_Msg);
    }
    x_os_mutex_release(&t_Mutex_Inverter_Msg);
}

void Inverter_Send(void){
    if(st_Inverter.un_State.bits.b1_Protocol_Set == TRUE){
        Inverter_Tx_VmuCommand2();
        switch(st_Inverter.st_Coms.u8_Send_Turn){
        case 0:
            Inverter_Tx_VmuCommand1();
            st_Inverter.st_Coms.u8_Send_Turn = 1;
            break;
        case 1:
            Inverter_Tx_VmuCommandSafety();
            st_Inverter.st_Coms.u8_Send_Turn = 0;
            break;
        default:
            st_Inverter.st_Coms.u8_Send_Turn = 0;
        }
    }

}
