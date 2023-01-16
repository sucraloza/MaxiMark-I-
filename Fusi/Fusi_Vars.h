#ifndef FUSI_VARS_H_INCLUDED
#define FUSI_VARS_H_INCLUDED

typedef struct{
    uint64 b2_CAN_1:2;
    uint64 b2_CAN_2:2;
    uint64 b2_CAN_3:2;
    uint64 b2_CAN_4:2;
    uint64 b2_Auxiliary_ECU_CAN:2;
    uint64 b2_Auxiliary_Battery_Low:2;
    uint64 b2_EEprom_Error:2;
    uint64 b2_Emergency_Stop:2;
    uint64 b2_Change_State_Conditions:2;
    uint64 b2_Key_Off_on_Drive:2;
    uint64 b2_Power_Off_Inverter_Timout:2;
    uint64 b2_Power_Off_Current_Timeout:2;
    uint64 b2_Power_Off_PDU_Open_Timeout:2;
    uint64 b2_Power_Off_Battery_Open_Timeout:2;
    uint64 b2_Vision_No_Respoce:2;
    uint64 b2_Power_On_PDU_Circuits:2;
    uint64 b2_Power_On_Protocol_Inverter:2;
    uint64 b2_Power_On_Battery_CAN:2;
    uint64 b2_Power_On_Battery_Ready:2;
    uint64 b2_Power_On_PDU_Close:2;
    uint64 b2_Power_On_HVIL:2;
    uint64 b2_HVIL_Fault_No_Speed:2;
    uint64 b2_HVIL_Fault_With_Speed:2;
    uint64 rsv:18;
}St_Fusi_System_Faults;

typedef union{
    uint64 all;
    St_Fusi_System_Faults bits;
}Un_Fusi_System_Faults;

typedef struct{
    uint32 b2_CAN:2;
    uint32 b2_Failure:2;
    uint32 b2_Catastrofic_Failure:2;
    uint32 b2_OverTemp:2;
    uint32 b2_Charge_Failure:2;
    uint32 b2_Low_Coolant:2;
    uint32 b2_BTMS_Failure:2;
    uint32 b2_Over_SOC_Warning:2;
    uint32 b2_Under_SOC_Warning:2;
    uint32 b2_Under_SOC_Critical:2;
    uint32 b2_Isolation_Neg_Warning:2;
    uint32 b2_Isolation_Pos_Warning:2;
    uint32 b2_Isolation_Neg_Fault:2;
    uint32 b2_Isolation_Pos_Fault:2;
    uint32 rsv:4;
}St_Fusi_System_Battery;

typedef union{
    uint32 all;
    St_Fusi_System_Battery bits;
}Un_Fusi_System_Battery;

typedef struct{
    uint16 b2_HVIL_Open:2;
    uint16 b2_Low_Voltage:2;
    uint16 b2_Precharge_Failure:2;
    uint16 b2_Precharge_Cant_Close:2;
    uint16 b2_Contactor_Cant_Close:2;
    uint16 b2_Contactor_Welded:2;
    uint16 b2_Discharge_Failure:2;
    uint16 rsv:2;
}St_Fusi_PDU_Faults;

typedef union{
    uint16 all;
    St_Fusi_PDU_Faults bits;
}Un_Fusi_PDU_Faults;

typedef struct{
    uint16 b2_CAN:2;
    uint16 b2_Failure:2;
    uint16 b2_Pump_CAN:2;
    uint16 b2_Pump_Failure:2;
    uint16 b2_Coolant_Low:2;
    uint16 b2_Regen_Limitation:2;
    uint16 rsv:4;
}St_Fusi_Motor_Faults;

typedef union{
    uint16 all;
    St_Fusi_Motor_Faults bits;
}Un_Fusi_Motor_Faults;

typedef struct{
    uint16 b2_CAN:2;
    uint16 b2_Failure:2;
    uint16 b2_OverTemp:2;
    uint16 b2_PSU_Not_Working:2;
    uint16 b2_PSU_Low_Voltage:2;
    uint16 b2_PSU_Fault:2;
    uint16 b2_DCU_Fault:2;
    uint16 b2_SCU_HV_Fuse:2;
}St_Fusi_AuxInv_Faults;

typedef union{
    uint16 all;
    St_Fusi_AuxInv_Faults bits;
}Un_Fusi_AuxInv_Faults;

typedef struct{
    uint8 b2_CAN:2;
    uint8 b2_Failure:2;
    uint8 rsv:4;
}St_Fusi_Steering_Faults;

typedef union{
    uint8 all;
    St_Fusi_Steering_Faults bits;
}Un_Fusi_Steering_Faults;

typedef struct{
    uint16 b2_Leak:2;
    uint16 b2_OverTemp:2;
    uint16 b2_Sensor_Error:2;
    uint16 b2_Low_Pressure:2;
    uint16 b2_No_Pressure:2;
    uint16 b2_No_Pressure_Vision:2;
    uint16 rsv:4;
}St_Fusi_Air_Faults;

typedef union{
    uint16 all;
    St_Fusi_Air_Faults bits;
}Un_Fusi_Air_Faults;

typedef struct{
    uint16 b2_Pump_CAN:2;
    uint16 b2_Pump_Failure:2;
    uint16 b2_Low_Coolant:2;
    uint16 b2_PTC_1_CAN:2;
    uint16 b2_PTC_1_Failure:2;
    uint16 b2_PTC_2_CAN:2;
    uint16 b2_PTC_2_Failure:2;
    uint16 rsv:2;
}St_Fusi_Heating_Faults;

typedef union{
    uint16 all;
    St_Fusi_Heating_Faults bits;
}Un_Fusi_Heating_Faults;

typedef struct{
    uint16 b2_Chek_Engine:2;
    uint16 b2_Buzzer:2;
    uint16 b2_Block_Power_On:2;
    uint16 b2_Limit_Speed:2;
    uint16 b2_Park_Request:2;
    uint16 b2_Motor_Off:2;
    uint16 b2_Power_Off_Park:2;
    uint16 b2_Power_Off_Now:2;
}St_Fusi_Actions_Fusi;

typedef union{
    uint16 all;
    St_Fusi_Actions_Fusi bits;
}Un_Fusi_Actions_Fusi;

typedef struct{
    uint32 b2_Sys_CAN_3:2;
    uint32 b2_Batt_CAN:2;
    uint32 b2_Batt_Failure:2;
    uint32 b2_Batt_Catastrofic_Failure:2;
    uint32 b2_Batt_Isolation_Neg_Fault:2;
    uint32 b2_Batt_Isolation_Pos_Fault:2;
    uint32 b2_PDU_Low_Voltage:2;
    uint32 b2_PDU_HVIL_Open:2;
    uint32 b2_PDU_Contactor_Cant_Close:2;
    uint32 b2_PDU_Contactor_Welded:2;
    uint32 rsv:12;
}St_Fusi_Eeprom_Vars_Fusi;

typedef union{
    uint32 all;
    St_Fusi_Eeprom_Vars_Fusi bits;
}Un_Fusi_Eeprom_Vars_Fusi;

typedef struct{
    uint8 u8_State;
    uint16 u16_Activation_DTC;
    uint32 u32_timeout;
}St_Fusi_Buzzer_Timer;

#define INIT_FUSI_BUZZER_TIMER(v)     \
    v.u8_State = 0;                   \
    v.u16_Activation_DTC = 0;         \
    v.u32_timeout = 0;

typedef struct {
    Un_Fusi_System_Faults un_System;
    Un_Fusi_System_Battery un_Battery;
    Un_Fusi_PDU_Faults un_PDU;
    Un_Fusi_Motor_Faults un_Motor;
    Un_Fusi_AuxInv_Faults un_AuxInv;
    Un_Fusi_Steering_Faults un_Steering;
    Un_Fusi_Air_Faults un_Air;
    Un_Fusi_Heating_Faults un_Heating;
    Un_Fusi_Actions_Fusi un_Actions;
    Un_Fusi_Eeprom_Vars_Fusi un_Eeprom;
    St_Fusi_Buzzer_Timer t_Buzzer_Timer;
    uint8 u8_DTC_Level;
    uint16 u16_DTC;
    uint16 u16_DTCs[4];
}St_Fusi_Vars;

#define INIT_FUSI(v)                            \
    v.un_System.all = 0;                        \
    v.un_Battery.all = 0;                       \
    v.un_PDU.all = 0;                           \
    v.un_Motor.all = 0;                         \
    v.un_AuxInv.all = 0;                        \
    v.un_Steering.all = 0;                      \
    v.un_Air.all = 0;                           \
    v.un_Heating.all = 0;                       \
    v.un_Actions.all = 0;                       \
    v.un_Eeprom.all = 0;                        \
    INIT_FUSI_BUZZER_TIMER(v.t_Buzzer_Timer)    \
    v.u8_DTC_Level = 0;                         \
    v.u16_DTC = 0;

typedef struct{
    uint8 u8_Fast_Counter;
    uint8 u8_Normal_Counter;
    uint8 u8_Slow_Counter;
    uint16 u16_Air_Start_Counter;
    uint32 u32_Vision_No_Responce_Time_Limit;
    uint16 u16_Battery_Isolation_Start_Counter;
} St_Counters_Fusi;

#define INIT_FUSI_COUNTERS(v)                       \
    v.u8_Fast_Counter = 0;                          \
    v.u8_Normal_Counter = 0;                        \
    v.u8_Slow_Counter = 0;                          \
    v.u16_Air_Start_Counter = 0;                    \
    v.u32_Vision_No_Responce_Time_Limit = 18000;    \
    v.u16_Battery_Isolation_Start_Counter = 0;

extern St_Fusi_Vars st_Fusi;
extern St_Counters_Fusi st_Fusi_Counters;

extern void *mpv_MsgObjTx_Fusi; // module global defined TX object for Fusi
extern void *mpv_MsgObjTx_Fusi4; // module global defined TX object for Fusi
extern void *mpv_MsgObjRx_Fusi; // module global defined TX object for Fusi

extern T_x_can_msg t_Fusi_Msg;  // Message Holder RX object
extern T_x_os_mutex t_Mutex_Fusi_Msg;


#endif // FUSI_VARS_H_INCLUDED
