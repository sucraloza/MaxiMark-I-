#include "buzzer.h"

#define SPEED_OFF_THRESHOLD ((uint8) 16)
#define SPEED_ON_THRESHOLD ((uint8) 14)

uint32 u32_time_buzzer_on;
uint32 u32_timeout_buzzer_on;
uint32 u32_time_buzzer_off;
uint32 u32_timeout_buzzer_off;
uint8 u8_Buzzer_state;

void Buzzer_Init(void)
{
    u32_time_buzzer_on = 300;
    u32_time_buzzer_off = 1600;

    u32_timeout_buzzer_on = Timeout_Set_ms(u32_time_buzzer_on);
    u32_timeout_buzzer_off = Timeout_Set_ms(u32_time_buzzer_off);

}

inline void Buzzer_Activate(void)
{
    if (Timeout_Check(u32_timeout_buzzer_on))
    {
        if (Timeout_Check(u32_timeout_buzzer_off))
        {
            u32_timeout_buzzer_on = Timeout_Set_ms(u32_time_buzzer_on);
            u32_timeout_buzzer_off = Timeout_Set_ms(u32_time_buzzer_off);
        }
        else
        {
            GPIO_set_T15_Buzzer(DS_OFF);
        }
    }

    else
    {
        GPIO_set_T15_Buzzer(DS_ON);
    }

}

inline void Buzzer_Deactivate(void)
{
    GPIO_set_T15_Buzzer(DS_OFF);
}

void Buzzer_Set_Speed(uint8 u8_kmh)
{
    Compressor_Run();
    if (System_get_State() == ECU_STATE_GPIO_CHECK)
    {
        return;
    }


    if(System_get_State() != ECU_STATE_DRIVE){
        Buzzer_Deactivate();
        return;
    }

    if(Gear_get_State() == GEAR_PARK){
        Buzzer_Deactivate();
        return;
    }

    if(Gear_get_State() == GEAR_REVERSE){
        Buzzer_Deactivate();
        return;
    }

    MON_TX_Var16(GPIO_get_T15_Buzzer());

    if (u8_kmh < SPEED_ON_THRESHOLD)
    {
        u8_Buzzer_state = BUZZER_ACTIVATE;
    }
    else if (u8_kmh > SPEED_OFF_THRESHOLD)
    {
        u8_Buzzer_state = BUZZER_DEACTIVATE;
    }

    if ( u8_Buzzer_state == BUZZER_ACTIVATE)
    {
        Buzzer_Activate();
    }

    else if( u8_Buzzer_state == BUZZER_DEACTIVATE)
    {
        Buzzer_Deactivate();
    }
}
