#ifndef PDU_VARS_H_INCLUDED
#define PDU_VARS_H_INCLUDED

typedef struct{
    uint8 u8_Circuit_State;
    uint8 u8_Circuit_State_Reference;
    uint32 u32_Initial_Time_ms;
    uint32 u32_Current_Time_ms;
    uint32 u32_Timeout_Precharge_ms;
    uint32 u32_Timeout_Bypass_ms;
    uint32 u32_Timeout_Disengage_ms;
    uint32 u32_Timeout_Discharge_ms;
    uint32 u32_Timeout_Discharge_Weld_ms;
    uint16 u16_Contactor_Engage_Voltage_V;
    uint16 u16_Operational_Voltage_V;
    uint16 u16_Safe_Voltage_V;
    uint8 u8_Precharge_Circuit_Ok;
    uint16 u16_Precharge_Circuit_Ok_Voltage_V;
} St_PDU_Circuit;

#define INIT_PDU_CIRCUIT(v)                                                     \
    v.u8_Circuit_State = 0;                                                     \
    v.u8_Circuit_State_Reference = 0;                                           \
    v.u32_Initial_Time_ms = 0;                                                  \
    v.u32_Current_Time_ms = 0;                                                  \
    v.u32_Timeout_Precharge_ms = 3000;                                          \
    v.u32_Timeout_Bypass_ms = 1000;                                             \
    v.u32_Timeout_Disengage_ms = 100;                                           \
    v.u32_Timeout_Discharge_ms= 3000;                                           \
    v.u32_Timeout_Discharge_Weld_ms = 4000;                                     \
    v.u16_Contactor_Engage_Voltage_V = PDU_MIN_OPERATIONAL_VOLTAGE_V - 20;      \
    v.u16_Operational_Voltage_V = PDU_MIN_OPERATIONAL_VOLTAGE_V;                \
    v.u16_Safe_Voltage_V = 60;                                                  \
    v.u8_Precharge_Circuit_Ok = 0;                                              \
    v.u16_Precharge_Circuit_Ok_Voltage_V = 100;

typedef struct{
    uint16 b2_Precharge:2;
    uint16 b2_No_Voltage:2;
    uint16 b2_Bypass:2;
    uint16 b2_Welded:2;
    uint16 b2_Discharge:2;
    uint16 rsv:6;
} St_PDU_Faults_Flags;

typedef union{
    uint16 all;
    St_PDU_Faults_Flags bits;
}Un_PDU_Faults;

typedef struct{
    St_PDU_Circuit PDU_Circuit;
    Un_PDU_Faults Errors;
} St_PDU_Variables;

#define INIT_PDU(v)                         \
    INIT_PDU_CIRCUIT(v.PDU_Circuit)         \
    v.Errors.all = 0;

extern St_PDU_Variables st_PDU;
extern T_x_os_mutex t_Mutex_PDU;

#endif // PDU_VARS_H_INCLUDED
