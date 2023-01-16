#include "OS_Defs.h"
#include "Timeout.h"

uint32 Timeout_Set_ms(uint32 u32_delta_ms){
    uint32 u32_current_time_ms = x_sys_get_time_ms();
    uint32 u32_timeout_ms = u32_current_time_ms + u32_delta_ms;
    if(u32_timeout_ms > u32_current_time_ms) return u32_timeout_ms;
    else return 0xFFFFFFFF;
}

inline uint8 Timeout_Check(uint32 u32_timeout_ms){
    return (x_sys_get_time_ms() >= u32_timeout_ms);
}
