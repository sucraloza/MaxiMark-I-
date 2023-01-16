#include "OS_Defs.h"
#include "Air.h"

T_Air_Vars t_Air;

void Air_Init(void){
    t_Air.u32_Start_Run = 0;
    t_Air.u32_Start_Blast = 0;
    t_Air.u32_Start_Heat = 0;
    t_Air.u16_Air_Pressure_mbar = 0;
    t_Air.u8_Start_Ok = 0;
    t_Air.u8_Charge_Air = 0;
}

void Air_Disable(void){
    GPIO_set_TPDU_Compressor(DS_ON);
    GPIO_set_TPDU_AC(DS_ON);
    SCU_set_DCU_Enable(FALSE);
    SCU_set_DCU_Speed_rpm(0);
}

void Air_Run(void){
    t_Air.u8_Charge_Air = Compressor_get_Charge_Air();

    T_TPDU_Inputs t_inputs;
    t_inputs.u8_Compressor = t_Air.u8_Charge_Air;

    /** TODO: La funcion GPIO_get_AC_Front deberia contener todo tipo de logica,
     * y evitar hacer procesamiento por fuera (en este caso, negar el resultado)
     */

    if(GPIO_get_Steering_Tank_Ref()){
        t_inputs.u8_AC = !GPIO_get_AC_Front();
    }
    else{
        t_inputs.u8_AC = FALSE;
    }

    float32 f32_power = SCU_get_DCU_Power_kW();
    t_inputs.u8_DisengageEnabled = (f32_power < TPDU_DISENGAGE_MAX_POWER_KW);

    if(System_get_State() != ECU_STATE_DRIVE){
        t_inputs.u8_AC = FALSE;
    }

    if((System_get_State() != ECU_STATE_DRIVE) && (System_get_State() != ECU_STATE_CHARGE)){
        t_inputs.u8_Compressor = FALSE;
    }

    if(!System_get_DCLink_State()){
        SCU_set_DCU_Enable(FALSE);
    }

    T_TPDU_Outputs t_outputs;
    TPDU_Run_StateMachine(t_inputs, &t_outputs);
    if (!t_outputs.u8_Timeout)
    {
        return;
    }

    if (t_outputs.u8_LowerSpeed)
    {
        SCU_set_DCU_Speed_rpm(0);
        if(SCU_get_DCU_Speed_rpm() < AIR_OFF_SPEED_RPM){
            SCU_set_DCU_Enable(FALSE);
        }
        //TODO REMOVER ESTO DE AQUI E IMPLEMENTARLO BIEN
        Telemetry_Track_Compresor_Use(FALSE);
        return;
    }



    E_TPDU_State e_TPDU_State = TPDU_Get_State();
    if (e_TPDU_State == TPDU_AC_ON)
    {
        SCU_set_DCU_Speed_rpm(2900);
        SCU_set_DCU_Enable(TRUE);
    }
    else if (e_TPDU_State == TPDU_COMP_ON)
    {
        SCU_set_DCU_Speed_rpm(3000);
        SCU_set_DCU_Enable(TRUE);
        //TODO REMOVER ESTO DE AQUI E IMPLEMENTARLO BIEN
        Telemetry_Track_Compresor_Use(TRUE);
    }
}

uint8 Air_get_Compresor_Ok(void){
    return Compressor_get_Ok();
}
