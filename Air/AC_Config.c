#include "OS_Defs.h"
#include "AC_Config.h"

T_AC_Config_Vars t_AC_Config;

void AC_Config_Init(void){
    Compressor_Init();
    t_AC_Config.e_Type = (E_AC_Config_States) EEPROM_get_AC_Type();
    if(t_AC_Config.e_Type == AC_CONFIG_TPDU){
        Air_Init();
    }
    else{
        AC_Compressor_Init(X_CAN_BUS_02);
        Service_Air_Init();
    }
}

void AC_Config_Run(void){
    Compressor_Run();
    if(t_AC_Config.e_Type == AC_CONFIG_TPDU){
        Air_Run();
    }
    else{

        if(System_get_DCLink_State()){
             AC_Comp_Set_Enable(!GPIO_get_AC_Front());
        }
        else{
             AC_Comp_Set_Enable(FALSE);
        }
        AC_Comp_Run();

        Service_Air_Set_Ref(Compressor_get_Charge_Air());
        if(SCU_get_DCU_Error() == 1){
            Service_Air_set_Feedback(SCU_get_DCU_Speed_rpm(),SERVICE_AIR_ERROR);
        }
        else{
            Service_Air_set_Feedback(SCU_get_DCU_Speed_rpm(),Service_Air_Get_Enable());
        }
        Service_Air_Run();
        SCU_set_DCU_Speed_rpm(Service_Air_Get_Speed());
        SCU_set_DCU_Enable(Service_Air_Get_Enable());
    }
}
