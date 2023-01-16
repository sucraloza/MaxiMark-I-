#include "OS_Defs.h"
#include "Key.h"

t_Key_State t_Key;
T_x_os_mutex t_Mutex_Key;

void Key_Init(void){
    //Init Vars
    t_Key.u8_State = 0;
    t_Key.u8_Enable = TRUE;
    t_Key.u8_Acc = 0;
    t_Key.u8_Start = 0;

    //Filter Init
    Filter_Init(&t_Key.t_Acc_Filter,KEY_FILTER_A0,KEY_FILTER_A1,KEY_FILTER_B1);
    Filter_Init(&t_Key.t_Start_Filter,KEY_FILTER_A0,KEY_FILTER_A1,KEY_FILTER_B1);

    //Init Mutex
    sint16 s16_Error = x_os_mutex_init(&t_Mutex_Key);
    if(s16_Error != C_NO_ERR) Error_Beep();
}

void Key_set_Enable(uint8 u8_ref){
    if(x_os_mutex_get(&t_Mutex_Key,10) == C_NO_ERR){
        t_Key.u8_Enable = u8_ref;
        x_os_mutex_release(&t_Mutex_Key);
    }
}

uint8 Key_get_State(void){
    return t_Key.u8_State;
}

void Key_Update(uint8 u8_Acc_raw, uint8 u8_Start_raw){
    if(x_os_mutex_get(&t_Mutex_Key,10) != C_NO_ERR){
        //Todo Mutex Error
        return;
    }

    float32 f32_Acc_pu = Filter(&(t_Key.t_Acc_Filter),u8_Acc_raw * 1.0f);
    if(f32_Acc_pu < 0.25f)
        t_Key.u8_Acc = DS_OFF;
    if(f32_Acc_pu > 0.75f)
        t_Key.u8_Acc = DS_ON;
    if(f32_Acc_pu >  1.0f || f32_Acc_pu < 0.0f)
        t_Key.u8_Acc = DS_INVALID1;

    float32 f32_Start_pu = Filter(&(t_Key.t_Start_Filter),u8_Start_raw * 1.0f);
    if(f32_Start_pu < 0.25f)
        t_Key.u8_Start = DS_OFF;
    if(f32_Start_pu > 0.75f)
        t_Key.u8_Start = DS_ON;
    if(f32_Start_pu >  1.0f || f32_Acc_pu < 0.0f)
        t_Key.u8_Start = DS_INVALID1;

    if(t_Key.u8_Acc == DS_OFF && t_Key.u8_Start == DS_OFF)
    {
       if(t_Key.u8_Enable){
            t_Key.u8_State= KEY_OFF;
       }
    }

    if(t_Key.u8_Acc == DS_ON && t_Key.u8_Start == DS_OFF && t_Key.u8_State != KEY_START)
    {
        if(t_Key.u8_Enable){
            t_Key.u8_State = KEY_ACCESORY;
        }
    }

    if(t_Key.u8_Start == DS_ON)
    {
       if(t_Key.u8_Enable){
            t_Key.u8_State = KEY_START;
       }
    }

    x_os_mutex_release(&t_Mutex_Key);
}
