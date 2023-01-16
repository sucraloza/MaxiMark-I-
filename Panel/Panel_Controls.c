#include "OS_Defs.h"

void Panel_Set_Rpm(sint16 s16_Rpm){
    if(s16_Rpm >= 0)
        st_Panel.u16_Rpm = s16_Rpm;
    else
        st_Panel.u16_Rpm = -s16_Rpm;

    st_Panel.u8_Speed_kmh = SPEED_FACTOR * st_Panel.u16_Rpm;
}

void Panel_Set_Buzzer(uint8 u8_Buzzer){
    st_Panel.u8_Buzzer = u8_Buzzer;
}
