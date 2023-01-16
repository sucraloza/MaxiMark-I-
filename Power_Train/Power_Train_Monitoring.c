#include "OS_Defs.h"

void Inverter_Monitoring_State(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C008844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = (uint8) (st_Inverter.un_State.all & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((st_Inverter.un_State.all >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Inverter.un_State.all >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.un_State.all >> 24) & 0xFF);
    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void Inverter_Monitoring_Refs_1(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C018844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 5;

    t_Msg.au8_Data[0] = st_Inverter.st_Refs.un_Flags.all;
    t_Msg.au8_Data[1] = (uint8) ((st_Inverter.st_Refs.s16_Speed_rpm >> 0) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Inverter.st_Refs.s16_Speed_rpm >> 8) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.st_Refs.s16_Safe_Speed_rpm >> 0) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Inverter.st_Refs.s16_Safe_Speed_rpm >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Refs_2(void){
    sint16 s16_Torque_nm;
    sint16 s16_Safe_Torque_nm;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C028844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    s16_Torque_nm = st_Inverter.st_Refs.f32_Torque_nm;
    s16_Safe_Torque_nm = st_Inverter.st_Refs.f32_Safe_Torque_nm;

    t_Msg.au8_Data[0] = (uint8) ((s16_Torque_nm >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((s16_Torque_nm >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((s16_Safe_Torque_nm >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((s16_Safe_Torque_nm >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Electrical(void){
    sint16 s16_Current_A;
    uint16 u16_Auxiliary_Voltage_V;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C038844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 6;

    s16_Current_A = st_Inverter.st_Electrical.f32_Power_Current_A*10;
    u16_Auxiliary_Voltage_V = st_Inverter.st_Electrical.f32_Auxiliary_Voltage_V*100;

    t_Msg.au8_Data[0] = (uint8) ((s16_Current_A >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((s16_Current_A >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Inverter.st_Electrical.u16_Power_Voltage_V >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.st_Electrical.u16_Power_Voltage_V >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((u16_Auxiliary_Voltage_V >> 0) & 0xFF);
    t_Msg.au8_Data[5] = (uint8) ((u16_Auxiliary_Voltage_V >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Mechanical_1(void){
    sint16 s16_Estimated_Torque_nm;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C048844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    s16_Estimated_Torque_nm = st_Inverter.st_Mechanical.f32_Estimated_Torque_nm;

    t_Msg.au8_Data[0] = (uint8) ((st_Inverter.st_Mechanical.s16_Speed_rpm >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((st_Inverter.st_Mechanical.s16_Speed_rpm >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((s16_Estimated_Torque_nm >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((s16_Estimated_Torque_nm >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Inverter.st_Mechanical.u16_Drive_Torque_nm >> 0) & 0xFF);
    t_Msg.au8_Data[5] = (uint8) ((st_Inverter.st_Mechanical.u16_Drive_Torque_nm >> 8) & 0xFF);
    t_Msg.au8_Data[6] = (uint8) ((st_Inverter.st_Mechanical.u16_Regen_Torque_nm >> 0) & 0xFF);
    t_Msg.au8_Data[7] = (uint8) ((st_Inverter.st_Mechanical.u16_Regen_Torque_nm >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Mechanical_2(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C058844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8) ((st_Inverter.st_Mechanical.u16_Max_Foward_Speed_rpm >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((st_Inverter.st_Mechanical.u16_Max_Foward_Speed_rpm >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Inverter.st_Mechanical.u16_Max_Reverse_Speed_rpm >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.st_Mechanical.u16_Max_Reverse_Speed_rpm >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Inverter.st_Mechanical.u16_Motor_Angle >> 0) & 0xFF);
    t_Msg.au8_Data[5] = (uint8) ((st_Inverter.st_Mechanical.u16_Motor_Angle >> 8) & 0xFF);
    t_Msg.au8_Data[6] = (uint8) ((st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm >> 0) & 0xFF);
    t_Msg.au8_Data[7] = (uint8) ((st_Inverter.st_Mechanical.s16_Filtered_Speed_rpm >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

void Inverter_Monitoring_Cooling(void){
    uint16 u16_Flow_Inv;
    uint16 u16_Flow_Motor;

    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C068844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 5;

    u16_Flow_Inv = st_Inverter.st_Cooling.f32_Inv_Required_Flow_pu*100;
    u16_Flow_Motor = st_Inverter.st_Cooling.f32_Motor_Required_Flow_pu*100;

    t_Msg.au8_Data[0] = (uint8) ((u16_Flow_Inv >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((u16_Flow_Inv >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((u16_Flow_Motor >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((u16_Flow_Motor >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Inverter.st_Cooling.u8_Thermal_Indicator_pu >> 0) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Faults(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C078844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 5;

    t_Msg.au8_Data[0] = (uint8) ((st_Inverter.st_Fauts.u16_Inv_Event_Id >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((st_Inverter.st_Fauts.u16_Inv_Event_Id >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Inverter.st_Fauts.u16_Cnt_Falla_Inv_CAN >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.st_Fauts.u16_Cnt_Falla_Inv_CAN >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Inverter.st_Fauts.un_Flags.all >> 0) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Limits(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C088844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8) ((st_Inverter.st_Limits.u16_Max_Charge_Current_A >> 0) & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((st_Inverter.st_Limits.u16_Max_Charge_Current_A >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((st_Inverter.st_Limits.u16_Max_Discharge_Current_A >> 0) & 0xFF);
    t_Msg.au8_Data[3] = (uint8) ((st_Inverter.st_Limits.u16_Max_Discharge_Current_A >> 8) & 0xFF);
    t_Msg.au8_Data[4] = (uint8) ((st_Inverter.st_Limits.u16_Max_Battery_Voltage_V >> 0) & 0xFF);
    t_Msg.au8_Data[5] = (uint8) ((st_Inverter.st_Limits.u16_Max_Battery_Voltage_V >> 8) & 0xFF);
    t_Msg.au8_Data[6] = (uint8) ((st_Inverter.st_Limits.u16_Min_Battery_Voltage_V >> 0) & 0xFF);
    t_Msg.au8_Data[7] = (uint8) ((st_Inverter.st_Limits.u16_Min_Battery_Voltage_V >> 8) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}
void Inverter_Monitoring_Coms(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1C098844;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = st_Inverter.st_Coms.u8_Mayor_Protocol;
    t_Msg.au8_Data[1] = st_Inverter.st_Coms.u8_Minor_Protocol;
    t_Msg.au8_Data[2] = st_Inverter.st_Coms.u8_Mayor_Extension;
    t_Msg.au8_Data[3] = st_Inverter.st_Coms.u8_Minor_Extension;

    x_can_obj_send_msg(mpv_MsgObjTx_Monitoring, &t_Msg);
}

uint8 INMON_Counter;

void Inverter_Monitoring_Send(){

  switch(INMON_Counter){
      case 0:
            Inverter_Monitoring_State();
            break;
      case 1:
            Inverter_Monitoring_Refs_1();
            break;
      case 2:
            Inverter_Monitoring_Refs_2();
            break;
      case 3:
            Inverter_Monitoring_Electrical();
            break;
      case 4:
            Inverter_Monitoring_Mechanical_1();
            break;
      case 5:
            Inverter_Monitoring_Mechanical_2();
            break;
      case 6:
            Inverter_Monitoring_Cooling();
            break;
      case 7:
            Inverter_Monitoring_Faults();
            break;
      case 8:
            Inverter_Monitoring_Limits();
            break;
      case 9:
            Inverter_Monitoring_Coms();
            break;
      default:
          INMON_Counter = 0;

  }
  INMON_Counter++;
  if(INMON_Counter > 9){
          INMON_Counter = 0;
  }
}

