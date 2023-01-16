#include "Os_defs.h"
#include "Gear.h"


St_Gear_Vars t_Gear;

void Gear_Init(void){
    t_Gear.u8_State = GEAR_DIRECT;
}

uint8 Gear_get_State(void){
    return t_Gear.u8_State;
}

void Gear_Update(void){
    switch (Buttons_Get_Gear())
    {
        case BUTTONS_GEAR_D:
            if((Throttle_Get_Thottle_pu() == 0) && (System_get_State() == ECU_STATE_DRIVE)){
                t_Gear.u8_State = GEAR_DIRECT;
                Buttons_Gear_Fix_Led();
            }
            else if(System_get_State() != ECU_STATE_DRIVE){
                if(Jarvis_get_Button_Hand_Break() && Inverter_Is_Vehicle_Stop()){
                    t_Gear.u8_State = GEAR_PARK;
                    Buttons_Clear_Gear();
                }
                else{
                    t_Gear.u8_State = GEAR_NEUTRAL;
                    Buttons_Set_Neutral();
                }
            }
            break;
        case BUTTONS_GEAR_N:
            if(Jarvis_get_Button_Hand_Break() && Inverter_Is_Vehicle_Stop()){
                t_Gear.u8_State = GEAR_PARK;
                Buttons_Clear_Gear();
            }
            else{
                t_Gear.u8_State = GEAR_NEUTRAL;
                Buttons_Set_Neutral();
            }
            break;
        case BUTTONS_GEAR_R:
            if((Throttle_Get_Thottle_pu() == 0) && (System_get_State() == ECU_STATE_DRIVE)){
                t_Gear.u8_State = GEAR_REVERSE;
                Buttons_Gear_Fix_Led();
            }
            else if(System_get_State() != ECU_STATE_DRIVE){
                if(Jarvis_get_Button_Hand_Break() && Inverter_Is_Vehicle_Stop()){
                    t_Gear.u8_State = GEAR_PARK;
                    Buttons_Clear_Gear();
                }
                else{
                    t_Gear.u8_State = GEAR_NEUTRAL;
                    Buttons_Set_Neutral();
                }
            }
            break;
        default:
            break;
    }
}


