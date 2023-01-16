#ifndef TACHOGRAPH_VARS_H_INCLUDED
#define TACHOGRAPH_VARS_H_INCLUDED

// Gen vars
typedef struct{
    uint16 u16_Rpm;
    uint8 u8_Speed_kmh;
    uint8 u8_msg_Counter100;
    uint8 u8_msg_Counter500;
    uint8 u8_msg_Counter1k;
    uint8 u8_msg_Counter10k;
} St_Tachograph_Vars;

#define INIT_TACHOGRAPH(v)          \
    v.u16_Rpm = 0;                  \
    v.u8_Speed_kmh = 0;             \
    v.u8_msg_Counter100 = 0;        \
    v.u8_msg_Counter500 = 0;        \
    v.u8_msg_Counter1k = 0;         \
    v.u8_msg_Counter10k = 0;        

typedef struct{
    uint8 b1_Hand_Break : 1;
    uint8 b2_Coolant_Level : 2;
} St_Tachograph_RxVars;

typedef union{
    uint8 all;
    St_Tachograph_RxVars bits;
} Un_Tachograph_RxVars;

#define INIT_TACHOGRAPH_RXVARS(v)          \
    v.bits.b1_Hand_Break = 0;                  \
    v.bits.b2_Coolant_Level = 0;        

extern St_Tachograph_Vars st_Tachograph;
extern Un_Tachograph_RxVars un_Tachograph_RxVars;

#endif // TACHOGRAPH_VARS_H_INCLUDED
