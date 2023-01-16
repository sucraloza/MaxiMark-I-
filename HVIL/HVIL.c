#include "Os_Defs.h"
#include "HVIL.h"

t_HVIL_Vars t_HVIL;

void HVIL_Init(void){
    t_HVIL.u8_State = HVIL_OPERATION_OFF;
    t_HVIL.u8_Feedback = HVIL_OPEN;
    t_HVIL.u8_Counter = 0;
    Filter_Init(&t_HVIL.t_Filter,HVIL_FILTER_GAIN_A0,HVIL_FILTER_GAIN_A1,HVIL_FILTER_GAIN_B1);
}

void HVIL_Activate(void){
    t_HVIL.u8_State = HVIL_OPERATION_WAIT;
    t_HVIL.u8_Counter = 0;
}

void HVIL_Deactivate(void){
    t_HVIL.u8_State = HVIL_OPERATION_OFF;
    t_HVIL.u8_Feedback = HVIL_OPEN;
    t_HVIL.u8_Counter = 0;
}

uint8 HVIL_get_State(void){
    return t_HVIL.u8_State;
}

uint8 HVIL_get_Feedback(void){
    if (EEPROM_get_HVIL_Enable() != EEPROM_HVIL_ENABLE)
    {
         t_HVIL.u8_Feedback = HVIL_CLOSE;
    }
    return t_HVIL.u8_Feedback;
}

uint8 HVIL_is_Close(void){
    return (t_HVIL.u8_Feedback == HVIL_CLOSE);
}

uint8 HVIL_get_ref_GPIO(void){
    return (t_HVIL.u8_State != HVIL_OPERATION_OFF);
}

void HVIL_set_Feedback(uint8 u8_HVIL_Input){
    if(t_HVIL.u8_State == HVIL_OPERATION_WAIT){
        t_HVIL.u8_Counter++;
        if(t_HVIL.u8_Counter >= HVIL_WAIT_COUNTER_LIMIT){
            t_HVIL.u8_Counter = HVIL_WAIT_COUNTER_LIMIT;
            t_HVIL.u8_State = HVIL_OPERATION_ON;
        }
    }

    float32 f32_Feed_pu  = Filter(&(t_HVIL.t_Filter), u8_HVIL_Input * 1.0f);
    if(u8_HVIL_Input == 1){
        t_HVIL.t_Filter.f64_Y1 = 1.0f;
    }

    if(f32_Feed_pu < 0.25f)
        t_HVIL.u8_Feedback = HVIL_OPEN;
    if(f32_Feed_pu > 0.75f)
        t_HVIL.u8_Feedback = HVIL_CLOSE;

}
//void HVIL_Save(void)
//{
//    EEPROM_Write(HVIL_EEPROM_ADDRESS, HVIL_DATA_SIZE, t_HVIL.u8_State);
//}
