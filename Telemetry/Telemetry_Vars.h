#ifndef TELEMETRY_VARS_H_INCLUDED
#define TELEMETRY_VARS_H_INCLUDED

typedef struct{
    uint8 b1_Battery_Heating:1;
    uint8 b1_BTMS_State:1;
    uint8 b1_Gun_Connection:1;
    uint8 b1_Compressor_Use:1;
    uint8 b1_PTC1_State:1;
    uint8 b1_PTC2_State:1;
    uint8 b2_BMS_HV_State:2;
} St_Telemetry_Tracking_Flags;

typedef union{
    uint8 all;
    St_Telemetry_Tracking_Flags bits;
} Un_Telemetry_Tracking_Flags;


typedef struct{
    uint8 u8_ECU_State;
    uint8 u8_Key_State;
    uint8 u8_HVIL_Reference_State;
    uint8 u8_HVIL_Feedback_State;
    Un_Telemetry_Tracking_Flags un_Flags;
} St_Telemetry_Tracking;

#define INIT_TELEMETRY_TRACKING(v)          \
    v.u8_ECU_State = 0;                     \
    v.u8_Key_State = 0;                     \
    v.u8_HVIL_Reference_State = 0           \
    v.u8_HVIL_Feedback_State = 0;           \
    v.un_Flags.all = 0;

typedef struct{
    uint16 u16_Preveous_DTC;
    uint8 u8_Fusi_Message_Turn;
    uint8 u8_Tracking_Triggered;
    St_Telemetry_Tracking st_Tracking;
} St_Telemetry_Vars;

#define TELEMETRY_INIT(v)                   \
    v.u16_Preveous_DTC = 0;                 \
    v.u8_Fusi_Message_Turn = 0;             \
    v.u8_Tracking_Triggered = 0;            \
    INIT_TELEMETRY_TRACKING(v.st_Tracking)

typedef struct{
    uint8 u8_Fusi_Counter;
    uint16 u16_5min_Counter;
    uint16 u16_30sec_Counter;
} St_Counters_Telemetry;

#define INIT_TELEMETRY_COUNTERS(v)      \
    v.u8_Fusi_Counter = 0;              \
    v.u16_5min_Counter = 0;            \
    v.u16_30sec_Counter = 0;

extern St_Telemetry_Vars st_Telemetry;
extern St_Counters_Telemetry st_Telemetry_Counters;

extern void *mpv_MsgObjTx_Telemetry; // module global defined TX object for Telemetry

#endif // TELEMETRY_VARS_H_INCLUDED
