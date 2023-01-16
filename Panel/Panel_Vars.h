#ifndef PANEL_VARS_H_INCLUDED
#define PANEL_VARS_H_INCLUDED

typedef struct{
    uint16 u16_Rpm;
    uint8 u8_Speed_kmh;
    uint8 u8_Buzzer;
    uint8 u8_Temp;
    uint8 u8_Check_Engine;
    uint8 u8_msg_Counter_Fast;
    uint8 u8_msg_Counter_Slow;
} St_Panels_Vars;

#define INIT_PANEL(v)               \
    v.u16_Rpm = 0;                  \
    v.u8_Speed_kmh = 0;            \
    v.u8_Buzzer = 0;                \
    v.u8_Temp = 0;                  \
    v.u8_Check_Engine = 0;          \
    v.u8_msg_Counter_Fast = 0;      \
    v.u8_msg_Counter_Slow = 0;

extern St_Panels_Vars st_Panel;

extern void *mpv_MsgObjTx_Panel; // module global defined TX object for Monitoring


#endif // PANEL_VARS_H_INCLUDED
