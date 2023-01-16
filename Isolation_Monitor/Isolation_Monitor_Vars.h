#ifndef INSULATION_MONITOR_VARS_H_INCLUDED
#define INSULATION_MONITOR_VARS_H_INCLUDED

typedef struct {
    uint16 u16_Isulation_Resistance_kOhm;
    uint16 u16_Voltage_V;
    uint8  u8_Life_Signal;
    uint8  u8_State;
    uint8  u8_Alarm;
} St_Isolation_Monitor_Vars;

#define INIT_ISOLATION_MONITOR(v)               \
    v.u16_Isulation_Resistance_kOhm = 0;        \
    v.u16_Voltage_V = 0;                        \
    v.u8_Life_Signal = 0;                       \
    v.u8_State = 0;                             \
    v.u8_Alarm = 0;

extern St_Isolation_Monitor_Vars st_Isometer_Vars;
extern T_x_os_mutex t_Mutex_Isometer;

extern void *mpv_MsgObjTx_Isometer; // module global defined TX object for Isometer
extern void *mpv_MsgObjRx_Isometer; // module global defined RX object for Isometer

extern T_x_can_msg t_Isometer_Msg;          //Message Holder RX object
extern T_x_os_mutex t_Mutex_Isometer_Msg;   //Message Isometer


#endif // INTERRUPTS_H_INCLUDED
