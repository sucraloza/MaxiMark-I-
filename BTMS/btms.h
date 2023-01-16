#ifndef BTMS_H_INCLUDED
#define BTMS_H_INCLUDED

#include "OS_Defs.h"

#define BTMS_RX_MASK 0x0000003A
#define BTMS_TX_MASK 0x000000F4

#define BTMS_ID_RX_FEEDBACK ((uint32) 0x18FFC13A)
#define BTMS_ID_TX_CONTROL ((uint32) 0x18FF45F4)
#define BTMS_VOLTAGE_INVALID 0xFFFF


typedef enum
{
    BTMS_STATE_SLEEP = 0x0,
    BTMS_STATE_T15_ON = 0x3,
    BTMS_STATE_PDU_CLOSE = 0x5,
    BTMS_STATE_PRECHARGE = 0x6,
    BTMS_STATE_OPERATION = 0x9,
    BTMS_STATE_WAIT_FOR_SHUTDOWN = 0xA,
    BTMS_STATE_POWER_OFF = 0xC,
    BTMS_STATE_PDU_OPEN = 0xF,
} E_BTMS_MQ_States;

typedef enum
{
    BTMS_MQ_ERROR_NORMAL = 0x00,
    BTMS_MQ_ERROR_T15 = 0x01,
    BTMS_MQ_ERROR_PRECHARGE = 0x02,
    BTMS_MQ_ERROR_BTMS_FAULT = 0x04,
    BTMS_WAIT_SHUTDOWN_TIMEOUT = 0x08,
    BTMS_POWER_OFF_TIMEOUT = 0x10,
    BTMS_MQ_ERROR_CAN_SEND = 0x20,
} E_BTMS_MQ_Errors;

typedef enum
{
    T2B_NO_FAULT = 0x00,
    T2B_BUS_UNDER_VOLTAGE_L1 = 0x01,
    T2B_BUS_OVER_VOLTAGE_L1 = 0x02,
    T2B_OVER_CURRENT_L2 = 0x03,
    T2B_INNER_COMMUNICATION_ABNORMAL_L1 = 0x04,
    T2B_LOW_SPEED_L2 = 0x05,
    T2B_ABNORMAL_L2 = 0x06,
    T2B_SYSTEM_PRESSURE_PROTECT_L2 = 0x07,
    T2B_PREVENT_FREEZING_PROTECT_L2 = 0x08,
    T2B_OUTLET_TEMP_SENSOR_ABNORMAL_L2 = 0x09,
    T2B_INLET_TEMP_SENSOR_ABNORMAL_L2 = 0x0A,
    T2B_PREVENT_FREEZING_TEMP_SENSOR_ABNORMAL_L2 = 0x0B,
    T2B_WATER_PUMP_FAULT_L1 = 0x0C,
    T2B_PTC_FAULT_L1 = 0x0D,
    T2B_CAN_COMMUNICATION_ABNORMAL_L1 = 0x0E,
    T2B_VENT_TEMP_PROTECT_L2 = 0x0F,
    T2B_VENT_TEMP_SENSOR_ABNORMAL_L2 = 0x10,
    T2B_ENVIRONMENT_TEMP_SENSOR_ABNORMAL_L2 = 0x11,
    T2B_INVALID_FAULT_CODE = 0x12
} E_T2B_FaultCode;

typedef enum
{
    T2B_NORMAL = 0x0,
    T2B_L1_LIGHT_FAULT = 0x1,
    T2B_L2_MORE_SEVERE_FAULT = 0x2,
    T2B_L3_MOST_SEVERE_FAULT = 0x3,
} E_T2B_FaultLevel;

typedef enum
{
    BTMS_RELAY_OPEN = 0x0,
    BTMS_RELAY_CLOSE = 0x1,
    BTMS_RELAY_INVALID_1 = 0x2,
} E_BTMS_HVRelay_Status;

typedef enum
{
    BTMS_WORK_SHUTDOWN = 0x0,
    BTMS_WORK_COOLING = 0x1,
    BTMS_WORK_HEAT = 0x2,
    BTMS_WORK_SELF_LOOP = 0x3,
    BTMS_WORK_INVALID = 0x4,
} E_BTMS_WorkStatus;

typedef enum
{
    BTMS_NOT_CHARGING = 0x0,
    BTMS_CHARGING = 0x1,
    BTMS_CHARGING_INVALID_1 = 0x2,
    BTMS_CHARGING_INVALID_2 = 0x3,
} E_BTMS_ChargingState;

typedef enum
{
    BTMS_REQUEST_HV_ON = 0x0,
    BTMS_REQUEST_HV_OFF = 0x1,
    BTMS_REQUEST_HV_INVALID_1 = 0x2,
} E_BTMS_RequestHV;

typedef struct
{
    E_BTMS_WorkStatus e_WorkStatus;
    E_BTMS_RequestHV e_Request_HV;
    E_BTMS_ChargingState e_ChargingState;
    E_BTMS_HVRelay_Status e_RelayStatus;
    uint16 u16_HighVoltage_V;
    sint8 s8_Temperature_C;
    uint8 u8_RollingCounter;
} T_BTMS_Setpoints;

typedef struct
{
    E_T2B_FaultCode e_FaultCode;
    E_T2B_FaultLevel e_FaultLevel;
    sint16 s16_InletTemperature_C;
    sint16 s16_OutletTemperature_C;
    float32 f32_RequestedPower_kW;
    E_BTMS_HVRelay_Status e_RelayStatus;
    E_BTMS_WorkStatus e_WorkStatus;
} T_BTMS_Feedback;


typedef struct
{
    E_BTMS_MQ_States e_MQ_State;
    E_BTMS_MQ_States e_MQ_State_backup;
    uint8 u8_MQ_Fault_Codes;
    uint8 u8_On;
    uint16 u16_HighVoltage_V;
    sint8 s8_Temperature_C;
    uint8 u8_BTMS_Msg_Counter;
    E_BTMS_WorkStatus e_WorkStatus;
    E_BTMS_ChargingState e_Charging_Satus;
    T_BTMS_Setpoints t_Setpoints;
    T_BTMS_Feedback t_Feedback;
    uint32 u32_Timeout;
} T_BTMS;

extern sint8 BTMS_Init(void);
extern sint16 BTMS_Recived(void);
extern void BTMS_Send(void);
extern void BTMS_Get_Monitoring_Data(T_x_can_msg *t_msg);
extern void BTMS_Run_Machine_State(void);

/** Getters */
extern E_T2B_FaultCode BTMS_Get_FaultCode(void);
extern E_T2B_FaultLevel BTMS_Get_FaultLevel(void);
extern sint16 BTMS_Get_InletTemperature(void);
extern sint16 BTMS_Get_OutletTemperature(void);
extern float32 BTMS_Get_RequestedPower(void);
extern E_BTMS_HVRelay_Status BTMS_Get_RelayStatus(void);
extern E_BTMS_WorkStatus BTMS_Get_WorkStatus(void);
extern E_BTMS_MQ_States BTMS_get_Mq_State(void);
extern uint8 BTMS_get_Mq_State_Errors(void);

extern void BTMS_Get_Setpoints(T_BTMS_Setpoints *pt_setpoints);


/** Setters */
extern sint16 BTMS_Set_WorkMode(E_BTMS_WorkStatus e_ref);
extern sint16 BTMS_Set_ChargingState(E_BTMS_ChargingState e_ref);
extern void BTMS_Set_HVVoltage_V(uint16 u16_voltage_V);
extern void BTMS_Set_Ref_Temperature_C(sint8 s8_Temperature);
extern void BTMS_Set_OnOff(uint8 u8_ref);

//new
extern void BTMS_Set_Ref_WorkStatus(uint8 u8_WorkStatus);
extern void BTMS_Set_Ref_Temperature(sint8 u8_Temperature);

#endif /* BTMS_H_INCLUDED */
