#ifndef JARVIS_VARS_H_INCLUDED
#define JARVIS_VARS_H_INCLUDED

#include "MarkVersion.h"

typedef struct
{
    uint8 u8_Is_Regen;
    uint8 u8_Speed_kmh;
    uint8 u8_Gear;
    uint8 u8_Ignition;
} St_Jarvis_Command;

#define INIT_JARVIS_COMMAND(v) \
    v.u8_Is_Regen = 0;         \
    v.u8_Speed_kmh = 0;        \
    v.u8_Gear = GEAR_NEUTRAL;  \
    v.u8_Ignition = KEY_OFF;

typedef struct
{
    uint8 b2_DCU_Error : 2;
    uint8 b2_DCU_Status : 2;
    uint8 b2_PSU_Error : 2;
    uint8 b2_PSU_Status : 2;
} St_Jarvis_Converter_Flags;

typedef union
{
    uint8 all;
    St_Jarvis_Converter_Flags bits;
} Un_Jarvis_Converter_Flags;

typedef struct
{
    sint8 s8_PSU_Temperature_C;
    sint8 s8_Device_Temperature_C;
    uint16 u16_DC_Voltage_V;
    float32 f32_DC_Power_kW;
    float32 f32_Auxiliary_Voltage_V;
    Un_Jarvis_Converter_Flags un_Flags;
    uint8 u8_Charging_Current_A;
    uint8 u8_Avg_Charging_Current_A;
    uint8 u8_Compresor_State;
} St_Jarvis_Aux_Converter;

typedef struct
{
    uint16 b2_Charge_Button : 2;
    uint16 b1_Hand_Break : 1;
    uint16 b1_Pedal_Break : 1;
    uint16 b2_Regen_Button : 2;
    uint16 b2_Heater_Button : 2;
    uint16 b1_Heater_Module : 1;
    uint16 b1_Defroster : 1;
    uint16 b1_PTC_Operation : 1;
    uint16 b2_Tast_Overrun_Error : 1;
    uint16 rsv : 4;
} St_Jarvis_GPIO_Flags;

typedef union
{
    uint16 all;
    St_Jarvis_GPIO_Flags bits;
} Un_Jarvis_GPIO_Flags;

typedef struct
{
    uint16 u16_Pressure1_mBar;
    uint16 u16_Pressure2_mBar;
    Un_Jarvis_GPIO_Flags un_Flags;
} St_Jarvis_GPIO;

#define INIT_AUX_CONVERTER(v)           \
    v.s8_PSU_Temperature_C = 0;         \
    v.s8_Device_Temperature_C = 0;      \
    v.u16_DC_Voltage_V = 0;             \
    v.f32_DC_Power_kW = 0;              \
    v.f32_Auxiliary_Voltage_V = 0;      \
    v.un_Flags.all = 0;                 \
    v.u8_Charging_Current_A = 0;        \
    v.u8_Avg_Charging_Current_A = 0;    \
    v.u8_Compresor_State = 0;

#define INIT_JARVIS_GPIO(v)   \
    v.u16_Pressure1_mBar = 0; \
    v.u16_Pressure2_mBar = 0; \
    v.un_Flags.all = 0;

typedef struct
{
    uint8 u8_Version_Major;
    uint8 u8_Version_Minor;
    uint8 u8_Version_Patch;
    En_VersionStatus en_Version_Status;
} St_Jarvis_Version;

#define INIT_JARVIS_VERSION(v) \
    v.u8_Version_Major = 0; \
    v.u8_Version_Minor = 0; \
    v.u8_Version_Patch = 0; \
    v.en_Version_Status = RESERVED;

typedef struct
{
    uint16 b1_WaterPump_Enable : 1;
    uint16 b1_PTC1_Enable : 1;
    uint16 b1_PTC2_Enable : 1;
    uint16 b2_Active_Fans : 2;
    uint16 b1_Converter_DCU_Enable : 1;
    uint16 b1_Converter_PSU_Enable : 1;
    uint16 b1_OverSOC_Protection : 1;
    uint16 b1_OverSOC_Protection_Enable : 1;
    uint16 b1_get_Operation_State : 1;
    uint16 b1_task_overload : 1;
    uint16 b1_Compresor_Startup_Run : 1;
    uint16 b1_Compresor_Operation_Ok : 1;
    uint16 rsv : 2;
} St_Jarvis_System_Flags;

typedef union
{
    uint16 all;
    St_Jarvis_System_Flags bits;
} Un_Jarvis_System_Flags;

typedef struct{
    uint8  u8_PTC1_State;
    sint8  s8_PTC1_Temperature_C;
    uint16 u16_PTC1_Power_W;
    uint8  u8_PTC2_State;
    sint8  s8_PTC2_Temperature_C;
    uint16 u16_PTC2_Power_W;
} st_Jarvis_PTC;

#define INIT_JARVIS_PTC(v)              \
    v.u8_PTC1_State = 0;                \
    v.s8_PTC1_Temperature_C = 0;        \
    v.u16_PTC1_Power_W = 0;             \
    v.u8_PTC2_State = 0;                \
    v.s8_PTC2_Temperature_C = 0;        \
    v.u16_PTC2_Power_W = 0;

typedef struct
{
    St_Jarvis_Command st_Command;
    St_Jarvis_Aux_Converter st_Aux_Converter;
    St_Jarvis_GPIO st_GPIO;
    St_Jarvis_Version st_Version;
    Un_Jarvis_System_Flags un_System;
    st_Jarvis_PTC st_PTC;
} St_Jarvis_Vars;

#define INIT_JARVIS(v)                      \
    INIT_JARVIS_COMMAND(v.st_Command)       \
    INIT_AUX_CONVERTER(v.st_Aux_Converter)  \
    INIT_JARVIS_GPIO(v.st_GPIO)             \
    INIT_JARVIS_VERSION(v.st_Version)       \
    v.un_System.all = 0;                    \
    INIT_JARVIS_PTC(v.st_PTC)

extern St_Jarvis_Vars st_Jarvis;
extern T_x_os_mutex t_Mutex_Jarvis;

extern void *mpv_MsgObjTx_Jarvis; // module global defined TX object for Jarvis
extern void *mpv_MsgObjRx_Jarvis; // module global defined RX object for Jarvis

extern T_x_can_msg t_Jarvis_Msg; //Message Holder RX object
extern T_x_os_mutex t_Mutex_Jarvis_Msg;

#endif // JARVIS_VARS_H_INCLUDED
