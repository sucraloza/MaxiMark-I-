#include "OS_Defs.h"

void Tachograph_Set_Rpm(sint16 s16_Rpm){
    if(s16_Rpm >= 0)
        st_Tachograph.u16_Rpm = s16_Rpm;
    else
        st_Tachograph.u16_Rpm = -s16_Rpm;

    st_Tachograph.u8_Speed_kmh = SPEED_FACTOR * st_Tachograph.u16_Rpm;
}
