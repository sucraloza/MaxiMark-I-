#ifndef SCU_H_INCLUDED
#define SCU_H_INCLUDED

#define COMPRESOR_TEMPERATURE_RX_ID 0x18FF04F7
#define CONVERTER_PSU_VALUES_RX_ID 0x18FF03F7
#define CONVERTER_DCU_VALUES_RX_ID 0x18FF02F7
#define CONVERTER_STATUS_RX_ID 0x18FF00F7

#define CONVERTER_MASTER_CONTROL_TX_ID 0x18FF1080
#define CONVERTER_PSU_SETPOINT_TX_ID 0x18FFE380
#define CONVERTER_DCU_SETPOINT_TX_ID 0x18FFE280

#define CONVERTER_TX_MASK 0x00000080u
#define CONVERTER_RX_MASK 0x000000F7u

#define CONVERTER_INITIALIZATION_COMPLETE 0x0
#define CONVERTER_INITIALIZATION_ACTIVE 0x1
#define CONVERTER_RSV 0x2
#define CONVERTER_NO_DATA 0x3

#define CONVERTER_ERROR_LED_OFF 0x0
#define CONVERTER_ERROR_LED_ON 0x1

#define CONVERTER_NO_T15_SIGNAL 0x0
#define CONVERTER_T15_SIGNAL 0x1

#define CONVERTER_PRECHARGE_ACTIVE 0x0
#define CONVERTER_PRECHARGE_COMPLETED 0x1

#define CONVERTER_INVERTER_POWER_INHIBITED 0x0
#define CONVERTER_INVERTER_POWER_ENABLED 0x1

#define CONVERTER_INVERTER_NOT_READY 0x0
#define CONVERTER_INVERTER_READY 0x1

#define CONVERTER_NO_ERROR 0x0
#define CONVERTER_ERROR 0x1
#define CONVERTER_WARNING 0x2

#define CONVERTER_MASTER_CONTROL_DISABLE 0x0
#define CONVERTER_MASTER_CONTROL_ENABLE 0x1

#define CONVERTER_SWITCH_OFF 0x0
#define CONVERTER_SWITCH_ON 0x1

#define CONVERTER_OFF 0b000
#define CONVERTER_SLEEP 0b001
#define CONVERTER_ON 0b101

#define CONVERTER_MASTERCONTROL 0x0
#define CONVERTER_DCU 0x1
#define CONVERTER_PSU 0x2

#define CONVERTER_PSU_POWER_THRESHOLD_KW 0.1f
#define CONVERTER_PSU_AUX_VOLTAGE_THRESHOLD_V 23

#define MIN_DC_VOLTAGE 580
#define DCLINK_VOLTAGE 620

#define DCLINK_TOLERANCE 4


#define PSU_UNDER_VOLTAGE 22
#define PSU_VOLTAGE 26
#define PSU_VOLTAGE_DRIVE 28
#define PSU_MAXCURRENT 150

#define CONVERTER_MAX_SPEED_RPM 3000
#define DCU_SPEED_SETPOINT 3000
#define DCU_TORQUE_SETPOINT 0

#define PSU_VOLTAGE_FILTER_A0   0.20f
#define PSU_VOLTAGE_FILTER_A1   0.0f
#define PSU_VOLTAGE_FILTER_B1   -0.80f

#define PSU_CURRENT_FILTER_A0   0.20f
#define PSU_CURRENT_FILTER_A1   0.0f
#define PSU_CURRENT_FILTER_B1   -0.80f


#include "Filter/Filter.h"

typedef struct
{
    uint8 u8_InverterStatus; // agregar errores
    uint8 u8_InverterReady;
    uint8 u8_ErrorStatus;
    float32 f32_Voltage_V;
    float32 f32_Current_A;
    float32 f32_Power_kW;
    sint8 s8_Temperature_C;
} T_PSU_Variables;

typedef struct
{
    uint8 u8_InverterStatus;
    uint8 u8_InverterReady;
    uint8 u8_ErrorStatus;
    sint16 s16_MotorSpeed_rpm;
    float32 f32_MotorPower_kW;
} T_DCU_Variables;

typedef struct
{
    uint16 b_Init_State:2;
    uint16 b_Lamp_State:2;
    uint16 b_Dev_Num:4;
    uint16 b_Clamp_T15_State:2;
    uint16 b_Precharge_State:2;
    uint16 rsv:4;
} T_Converter_Flags;

typedef union
{
    uint16 all;
    T_Converter_Flags bits;
} U_Converter_Flags;

typedef struct
{
    U_Converter_Flags u_Flags;
    uint16 u16_ErrorCode;
    uint16 u16_VoltageDC_V;
    float32 f32_PowerDC_kW;
    sint8 s8_DeviceTemperature_C;
} T_Converter_Status;

typedef struct
{
    uint8 u8_PSU_Max_Current_A;
    uint8 u8_PSU_Voltage_V;
    sint16 s16_DCU_Speed_rpm;
    uint8 u8_Enable_DCU;
    uint8 u8_Enable_PSU;

} T_Converter_Setpoints;

typedef struct
{
    uint8 b2_CAN_Send:2;
    uint8 b2_CAN_Recived:2;
    uint8 rsv:4;
} T_Converter_Errors;

typedef union
{
    uint8 all;
    T_Converter_Errors bits;
} U_Converter_Errors;

typedef struct
{
    T_DCU_Variables t_DCU;
    T_PSU_Variables t_PSU;
    T_Converter_Status t_Status;
    T_Converter_Setpoints t_Setpoints;
    U_Converter_Errors u_Errors;
    T_Filter_Vars t_PSU_Voltage_Filter;
    T_Filter_Vars t_PSU_Current_Filter;
} T_SCU_Variables;

//Funciones Privadas
sint8 SCU_Master_Control_Tx(void);
sint16 SCU_DCU_Tx(void);
sint16 SCU_PSU_Tx(void);
void SCU_Status_Rx(void);
void SCU_DCU_Rx(void);
void SCU_PSU_Rx(void);


//Funciones Publicas
extern void SCU_Init(void);

extern void SCU_Send_Msg(void);
extern uint8 SCU_Recived_Msg(void);

extern void SCU_set_PSU_Enable(uint8 u8_ref);
extern void SCU_set_PSU_Voltage_V(uint8 u8_ref);
extern void SCU_set_PSU_Max_Current_A(uint8 u8_ref);
extern void SCU_set_DCU_Enable(uint8 u8_ref);
extern void SCU_set_DCU_Speed_rpm(sint16 s16_ref);

extern float32 SCU_get_PSU_Current_A(void);
extern float32 SCU_get_PSU_Voltage_V(void);
extern float32 SCU_get_PSU_Filtered_Current_A(void);
extern float32 SCU_get_PSU_Filtered_Voltage_V(void);
extern float32 SCU_get_PSU_Power_kW(void);
extern sint8 SCU_get_PSU_Temperature_C(void);
extern uint8 SCU_get_PSU_Power_Status(void);
extern uint8 SCU_get_PSU_Ready(void);
extern uint8 SCU_get_PSU_Error(void);
extern uint8 SCU_get_PSU_in_Error(void);
extern uint8 SCU_get_PSU_in_Warning(void);
extern uint8 SCU_get_PSU_in_Working(void);
extern uint8 SCU_get_PSU_in_Under_Voltage(void);

extern sint16 SCU_get_DCU_Speed_rpm(void);
extern sint16 SCU_get_DCU_Torque_Nm(void);
extern float32 SCU_get_DCU_Power_kW(void);
extern sint8 SCU_get_DCU_Temperature_C(void);
extern uint8 SCU_get_DCU_Power_Status(void);
extern uint8 SCU_get_DCU_Ready(void);
extern uint8 SCU_get_DCU_Error(void);
extern uint8 SCU_get_DCU_in_Error(void);
extern uint8 SCU_get_DCU_in_Warning(void);

extern uint16 SCU_get_Voltage_V(void);
extern float32 SCU_get_Power_kW(void);
extern sint8 SCU_get_Temperature_C(void);
extern uint8 SCU_get_Clamp_T15(void);
extern uint8 SCU_get_Init_Status(void);
extern uint8 SCU_get_Precharge_Status(void);
extern uint8 SCU_get_Error_Lamp_Status(void);
extern uint16 SCU_get_DTC(void);
extern uint8 SCU_get_Dev_Num(void);
extern uint8 SCU_get_Send_Error(void);
extern uint8 SCU_get_Recived_Error(void);

#endif /* SCU_H_INCLUDED */
