#include "OS_Defs.h"

void Jarvis_Set_Is_Regen(uint8 u8_Enable)
{
    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }
    st_Jarvis.st_Command.u8_Is_Regen = u8_Enable;
    x_os_mutex_release(&t_Mutex_Jarvis);
}

void Jarvis_Set_Rpm(float f32_rpm)
{
    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }
    //TODO CHANGE_FACTOR
    uint8 u8_Speed_kmh = SPEED_FACTOR * f32_rpm;

    st_Jarvis.st_Command.u8_Speed_kmh = u8_Speed_kmh;
    x_os_mutex_release(&t_Mutex_Jarvis);
}

sint8 Jarvis_get_Aux_Current(void)
{
    return 20;
}

uint16 Jarvis_get_Pressure_mBar(void)
{
    if (st_Jarvis.st_GPIO.u16_Pressure1_mBar > 13000)
        return 0;
    return st_Jarvis.st_GPIO.u16_Pressure1_mBar;
}

uint8 Jarvis_get_Button_Charge(void)
{
    return st_Jarvis.st_GPIO.un_Flags.bits.b2_Charge_Button;
}
uint8 Jarvis_get_Button_Regen(void)
{
    return st_Jarvis.st_GPIO.un_Flags.bits.b2_Regen_Button;
}
uint8 Jarvis_get_Button_Heating(void)
{
    return st_Jarvis.st_GPIO.un_Flags.bits.b2_Heater_Button;
}
uint8 Jarvis_get_Button_Pedal_Break(void)
{
    return st_Jarvis.st_GPIO.un_Flags.bits.b1_Pedal_Break;
}
uint8 Jarvis_get_Button_Hand_Break(void)
{
    // return st_Jarvis.st_GPIO.un_Flags.bits.b1_Hand_Break;
    return un_Tachograph_RxVars.bits.b1_Hand_Break;
}
uint8 Jarvis_get_PTC_Operation(void)
{
    return st_Jarvis.st_GPIO.un_Flags.bits.b1_PTC_Operation;
}

uint8 Jarvis_get_Major_Version(void)
{
    return st_Jarvis.st_Version.u8_Version_Major;
}
uint8 Jarvis_get_Minor_Version(void)
{
    return st_Jarvis.st_Version.u8_Version_Minor;
}
uint8 Jarvis_get_Pach_Version(void)
{
    return st_Jarvis.st_Version.u8_Version_Patch;
}
En_VersionStatus Jarvis_get_Status_Version(void)
{
    return st_Jarvis.st_Version.en_Version_Status;
}

uint8 Jarvis_get_Compresor_Run(void)
{
    return st_Jarvis.un_System.bits.b1_Compresor_Startup_Run;
}

uint8 Jarvis_get_Compresor_Ok(void)
{
    return st_Jarvis.un_System.bits.b1_Compresor_Operation_Ok;
}

uint8 Jarvis_get_Compresor_State(void){
    return st_Jarvis.st_Aux_Converter.u8_Compresor_State;
}

uint8 Jarvis_get_PTC1_State(void){
    return st_Jarvis.st_PTC.u8_PTC1_State;
}
uint8 Jarvis_get_PTC2_State(void){
    return st_Jarvis.st_PTC.u8_PTC2_State;
}
uint16 Jarvis_get_PTC1_Power_W(void){
    return st_Jarvis.st_PTC.u16_PTC1_Power_W;
}
uint16 Jarvis_get_PTC2_Power_W(void){
    return st_Jarvis.st_PTC.u16_PTC2_Power_W;
}
