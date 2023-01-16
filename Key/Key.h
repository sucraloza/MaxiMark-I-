#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "Filter/Filter.h"

//Filter Gains
#define KEY_FILTER_A0  0.02f
#define KEY_FILTER_A1  0.0f
#define KEY_FILTER_B1  -0.98f

//KEY STATES
#define KEY_OFF        0x00
#define KEY_ACCESORY   0x03
#define KEY_START      0x05

typedef struct{
    uint8 u8_State;
    uint8 u8_Enable;
    uint8 u8_Acc;
    uint8 u8_Start;
    T_Filter_Vars t_Acc_Filter;
    T_Filter_Vars t_Start_Filter;
} t_Key_State;

extern void Key_Init(void);
extern void Key_set_Enable(uint8 u8_ref);
extern void Key_Update(uint8 u8_Acc_raw, uint8 u8_Start_raw);
extern uint8 Key_get_State(void);


#endif /* KEY_H_INCLUDED */
