#ifndef VISION_H_INCLUDED
#define VISION_H_INCLUDED

#include "../MarkVersion.h"

#define VISION_CRITICAL_COUNTER_LIMIT 5
#define VISION_FAST_COUNTER_LIMIT 25
#define VISION_NORMAL_COUNTER_LIMIT 50
#define VISION_SLOW_COUNTER_LIMIT 100

#define VISION_BATTERIES_TX_ID 0x18030988
#define VISION_VEHICLE_TX_ID 0x18010988
#define VISION_VERSION_TX_ID 0x18040988
#define VISION_FUSI_TX_ID 0x18050988
#define VISION_ODOMETER_TX_ID 0x18060988

#define VISION_PERIODIC_RX_ID 0x18078809

#define VISION_VERSION_ACU 1
#define VISION_VERSION_VCU 2

#define VISION_VEHICLE_STATE_CONTACT    0
#define VISION_VEHICLE_STATE_READY      1
#define VISION_VEHICLE_STATE_CHARGING   2
#define VISION_VEHICLE_STATE_POWER_OFF  3
#define VISION_VEHICLE_STATE_ERROR      4
#define VISION_VEHICLE_STATE_INIT       5
#define VISION_VEHICLE_STATE_UNKNOWN    6

typedef struct
{
    uint8 b1_Battery_Charging : 1;
    uint8 b1_Check_Engine : 1;
    uint8 b1_Emergency_Button : 1;
    uint8 b1_Engine_Limitation : 1;
    uint8 b1_Gun_Connected : 1;
    uint8 b1_Handbreak : 1;
    uint8 b1_Stop_Required : 1;
    uint8 b1_rsv : 1;
} T_Vision_Indicator_Flags;

typedef union
{
    uint8 all;
    T_Vision_Indicator_Flags bits;
} U_Vision_Indicators;

typedef struct
{
    En_VersionStatus en_Jarvis_VersionStatus : 3;
    En_VersionStatus en_Mark_VersionStatus : 3;
    uint8 u8_Vision_Status : 1;
    uint8 u8_Jarvis_VersionMajor;
    uint8 u8_Jarvis_VersionMinor;
    uint8 u8_Jarvis_VersionPatch;
    uint8 u8_Mark_VersionMajor;
    uint8 u8_Mark_VersionMinor;
    uint8 u8_Mark_VersionPatch;
} T_Vision_Version;

typedef struct{
    uint8 u8_Critical_Counter;
    uint8 u8_Fast_Counter;
    uint8 u8_Normal_Counter;
    uint8 u8_Slow_Counter;
    uint8 u8_Gear;
    uint8 u8_Air_Charging;
    uint8 u8_Waiting;
    uint8 u8_Pressure_dBar;
    uint8 u8_SOC;
    uint8 u8_DTC_Level;
    uint8 u8_Rolling_Counter;
    uint8 u8_Vehicle_State;
    uint8 u8_Change_Page;
    sint8 s8_Engine_Temperature_C;
    sint8 s8_Batteries_Temperature_C;
    uint16 u16_Isolation_kohm;
    uint16 u16_DTC;
    uint32 u32_Odometer_100m;
    uint32 u32_Timeout;
    float32 f32_Voltage_DC_V;
    U_Vision_Indicators u_Indicators;
    T_Vision_Version t_Version;
} T_Vision_Vars;

extern void Vision_Init(void);

extern void Vision_Set_Gear(uint8 u8_Gear);
extern void Vision_Set_Air_Charging(uint8 u8_Air);
extern void Vision_Set_Waiting(uint8 u8_Wait);
extern void Vision_Set_Air_Pressure_mBar(uint16 u16_Pressure_mBar);
extern void Vision_Set_SOC(uint8 u8_SOC);
extern void Vision_Set_Fusi_DTC_Level(uint8 u8_Level);
extern void Vision_Set_Engine_Temperature_C(sint8 s8_Temperature);
extern void Vision_Set_Batteries_Temperature_C(sint8 s8_Temperature);
extern void Vision_Set_Isolation(uint16 u16_Isolation);
extern void Vision_Set_Fusi_DTC(uint16 u16_Level);
extern void Vision_Set_Odometer_km(float32 f32_km);
extern void Vision_Set_Voltage_DC_V(float32 f32_VoltageDC_V);
extern void Vision_Set_Vehicle_State(uint8 u8_Vehicle_State);
extern void Vision_Set_Change_Page(uint8 u8_Change_Page);

extern void Vision_Set_Is_Charging(uint8 u8_Is_Charging);
extern void Vision_Set_Check_Engine(uint8 u8_Check_Engine);
extern void Vision_Set_Emergency_Button(uint8 u8_Emergency_Button);
extern void Vision_Set_Engine_Limitation(uint8 u8_Engine_Limitation);
extern void Vision_Set_Gun_Connected(uint8 u8_Gun);
extern void Vision_Set_Handbreak(uint8 u8_Handbreak);
extern void Vision_Set_Stop_Required(uint8 u8_StopRequired);

extern void Vision_Set_Jarvis_Major_Version(uint8 u8_ver);
extern void Vision_Set_Jarvis_Minor_Version(uint8 u8_ver);
extern void Vision_Set_Jarvis_Pach_Version(uint8 u8_ver);
extern void Vision_Set_Jarvis_Status_Version(En_VersionStatus en_status);

extern void Vision_Update(void);
extern uint8 Vision_Receive(void);

extern void Vision_Set_Timeout(void);
extern void Vision_off(uint16 u16_DTC, uint8 u8_key);

#endif // VISION_H_INCLUDED
