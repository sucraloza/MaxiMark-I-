#include "OS_Defs.h"

uint16 Isometer_Get_Recistance_kOhm(void){
    return st_Isometer_Vars.u16_Isulation_Resistance_kOhm;
}
uint16 Isometer_Get_Voltage_V(void){
    return st_Isometer_Vars.u16_Voltage_V;
}
uint8 Isometer_Get_State(void){
    return st_Isometer_Vars.u8_State;
}
uint8 Isometer_Get_Alarm(void){
    return st_Isometer_Vars.u8_Alarm;
}

uint8 Isometer_Get_Warking_State(void){
        return System_get_T15_Isometer() == OUTPUT_ON;
}

uint8 Isometer_Get_Isolation_Fault(void){
    return (st_Isometer_Vars.u8_State == ISOMETER_STATUS_FAULT);
}

