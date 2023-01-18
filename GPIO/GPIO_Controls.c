#include "OS_Defs.h"
#include "GPIO_Defs.h"
#include "GPIO_Controls.h"

St_GPIO_Variables st_GPIO;
T_x_os_mutex t_Mutex_GPIO;

void GPIO_Init(){
    sint16 s16_Error;
    x_sys_set_all_relays(X_ON);   // enable power stage

    //Sensor Supply
    x_sys_set_sensor_supply(GPIO_SUPPLY_COMP,GPIO_SUPPLY_COMP_VOLTAGE_mV);

    //GPIOS KS1
    s16_Error = x_out_init(GPIO_PDU_DISCHARGE,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_PDU_PRECHARGE,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_PDU_INVERTER,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_PDU_BTMS,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_PDU_AC,X_OUT_TYPE_DIGITAL,1,0,NULL);

    s16_Error |= x_out_init(GPIO_HVIL_OUT,X_OUT_TYPE_DIGITAL,1,0,NULL);
    // s16_Error |= x_out_init(GPIO_STEERING_CONT,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_DAPIG,X_OUT_TYPE_DIGITAL,1,0,NULL);

    s16_Error |= x_in_init(GPIO_HVIL_IN,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    s16_Error |= x_in_init(GPIO_BATTERY_VOLTAGE,X_IN_TYPE_VOLTAGE_40V,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    /* s16_Error |= x_in_init(GPIO_THROTTLE_A, X_IN_TYPE_FREQUENCY, 0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_UP), 0, NULL);//PIN data, frecuencia, rising edge.
    s16_Error |= x_in_init(GPIO_THROTTLE_B, X_IN_TYPE_FREQUENCY, 0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_UP), 0, NULL);//PIN data, frecuencia, rising edge. */

    s16_Error |= x_in_init(GPIO_THROTTLE_A, X_IN_TYPE_VOLTAGE_5V, 0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN), 0, NULL);// TO BE INITIATED
    s16_Error |= x_in_init(GPIO_THROTTLE_B, X_IN_TYPE_VOLTAGE_5V, 0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN), 0, NULL);// TO BE INITIATED
    // s16_Error |= x_in_init(GPIO_AC_FRONT,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    s16_Error |= x_in_init(GPIO_AC_REAR,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);

    //GPIOS KS2
    s16_Error |= x_out_init(GPIO_T15_BCU,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_T15_INV,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_T15_SCREEN,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_T15_DIGI,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_T15_STEERING,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_T15_BTMS,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_T15_BUZZER,X_OUT_TYPE_DIGITAL,1,0,NULL);

    s16_Error |= x_in_init(GPIO_IN_TEMP_COMP,X_IN_TYPE_CURRENT,0,0,0,NULL);

    s16_Error |= x_out_init(GPIO_TPDU_AC,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_TPDU_COMP,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_BUZZER_SPEED,X_OUT_TYPE_PWM,1,0,NULL);
    s16_Error |= x_out_init(GPIO_OUT_CTANK,X_OUT_TYPE_DIGITAL,1,0,NULL);
    // s16_Error |= x_out_init(GPIO_OUT_HTANK,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_out_init(GPIO_STEERING_CONT,X_OUT_TYPE_DIGITAL,1,0,NULL);
    // s16_Error |= x_out_init(GPIO_OUT_STANK,X_OUT_TYPE_DIGITAL,1,0,NULL);
    s16_Error |= x_in_init(GPIO_EMERGENCY,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    // s16_Error |= x_in_init(GPIO_INERTIA,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    s16_Error |= x_in_init(GPIO_KEY_ACCESSORIES,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    s16_Error |= x_in_init(GPIO_KEY_START,X_IN_TYPE_VOLTAGE_40V,0,0,0,NULL);
    s16_Error |= x_in_init(GPIO_CHARGE_WAKEUP,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    // s16_Error |= x_in_init(GPIO_IN_STANK,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    s16_Error |= x_in_init(GPIO_IN_HTANK,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    s16_Error |= x_in_init(GPIO_IN_CTANK,X_IN_TYPE_DIGITAL,0,(X_IN_CIRCUIT_ACTIVE_HIGH|X_IN_CIRCUIT_PULL_DOWN),0,NULL);
    if(s16_Error != C_NO_ERR) Error_Beep();

    GPIO_INIT(st_GPIO)
    s16_Error = x_os_mutex_init(&t_Mutex_GPIO);
    if(s16_Error != C_NO_ERR) Error_Beep();
}

//Inputs
void GPIO_Update_Inputs(void){
    sint32 s32_Aux_Var;
    sint16 s16_Error;
    s32_Aux_Var = 0;

    if(x_os_mutex_get(&t_Mutex_GPIO,10) != C_NO_ERR){
        return;
    }
    x_os_mutex_release(&t_Mutex_GPIO);
    /*
    s16_Error = x_in_get_value(GPIO_INERTIA,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Inertia = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Inertia = DS_ON;
     */
    s16_Error = x_in_get_value(GPIO_EMERGENCY,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Emergency = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Emergency = DS_ON;

    s16_Error = x_in_get_value(GPIO_KEY_ACCESSORIES,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Key_Accesory = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Key_Accesory = DS_ON;

    s16_Error = x_in_get_value(GPIO_KEY_START,&s32_Aux_Var);
    if(s32_Aux_Var < 7000)
        st_GPIO.un_Ins_Digital.bits.b2_Key_Start = DS_OFF;
    if(s32_Aux_Var > 15000)
        st_GPIO.un_Ins_Digital.bits.b2_Key_Start = DS_ON;

    s16_Error = x_in_get_value(GPIO_CHARGE_WAKEUP,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Charge_Wakeup = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Charge_Wakeup = DS_ON;

    st_GPIO.un_Ins_Digital.bits.b2_Heating_Tank = DS_ON;
    /* s16_Error = x_in_get_value(GPIO_IN_HTANK,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Heating_Tank = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Heating_Tank = DS_ON; */

    // Reading is now performed by Tachograph and received through Tachograph_RX_EFL_P1()
    // To be commented
    s16_Error = x_in_get_value(GPIO_IN_CTANK,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Cooling_Tank = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Cooling_Tank = DS_ON;

    st_GPIO.un_Ins_Digital.bits.b2_Steering_Tank = DS_ON;
    // s16_Error = x_in_get_value(GPIO_IN_STANK,&s32_Aux_Var);
    /* if(s32_Aux_Var ==  FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_Steering_Tank = DS_OFF;
    if(s32_Aux_Var ==  TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_Steering_Tank = DS_ON; */


    s16_Error = x_in_get_value(GPIO_HVIL_IN,&s32_Aux_Var);
    if(s32_Aux_Var == FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_HVIL = DS_OFF;
    if(s32_Aux_Var == TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_HVIL = DS_ON;


    st_GPIO.un_Ins_Digital.bits.b2_AC_Front = DS_OFF;
/*     s16_Error = x_in_get_value(GPIO_AC_FRONT,&s32_Aux_Var);
    if(s32_Aux_Var ==  FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_AC_Front = DS_OFF;
    if(s32_Aux_Var ==  TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_AC_Front = DS_ON; */

    s16_Error = x_in_get_value(GPIO_AC_REAR,&s32_Aux_Var);
    if(s32_Aux_Var ==  FALSE)
        st_GPIO.un_Ins_Digital.bits.b2_AC_Rear = DS_OFF;
    if(s32_Aux_Var ==  TRUE)
        st_GPIO.un_Ins_Digital.bits.b2_AC_Rear = DS_ON;

    x_in_get_value(GPIO_IN_TEMP_COMP,&s32_Aux_Var);
    st_GPIO.s32_Compresor_Current_uA = s32_Aux_Var;
}

uint32 GPIO_get_Digital_Inputs(void){ return st_GPIO.un_Ins_Digital.all; }
//uint8 GPIO_get_Inertia(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Inertia == DS_ON); }
uint8 GPIO_get_Inertia(void){ return FALSE; }
uint8 GPIO_get_Emergency(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Emergency == DS_ON); }
uint8 GPIO_get_Key_Acc(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Key_Accesory == DS_ON); }
uint8 GPIO_get_Key_Start(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Key_Start == DS_ON); }
uint8 GPIO_get_Charge_Wakeup(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Charge_Wakeup == DS_ON); }
uint8 GPIO_get_Cooling_Tank(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Cooling_Tank == DS_ON); } // 
// uint8 GPIO_get_Cooling_Tank(void){ return (un_Tachograph_RxVars.bits.b2_Coolant_Level == DS_ON); }
uint8 GPIO_get_Heating_Tank(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Heating_Tank == DS_ON); }
// uint8 GPIO_get_Heating_Tank(void){ return (TRUE); }
uint8 GPIO_get_Steering_Tank(void){ return (st_GPIO.un_Ins_Digital.bits.b2_Steering_Tank == DS_ON); }
uint8 GPIO_get_HVIL_Feedback(void){ return (st_GPIO.un_Ins_Digital.bits.b2_HVIL == DS_ON); }
uint8 GPIO_get_AC_Front(void){ return (st_GPIO.un_Ins_Digital.bits.b2_AC_Front == DS_ON); }
uint8 GPIO_get_AC_Rear(void){ return (st_GPIO.un_Ins_Digital.bits.b2_AC_Rear == DS_ON); }
sint32 GPIO_get_Compresor_Current_uA(void){ return st_GPIO.s32_Compresor_Current_uA; }

//Outputs
void GPIO_Update_Outputs(void){
    GPIO_Update_T15_Outputs();
    GPIO_Update_PDU_Outputs();
    GPIO_Update_Generic_Outputs();
    GPIO_Update_Buzzer();
}

void GPIO_Update_T15_Outputs(void){
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_BCU){
        case DS_OFF: x_out_set_value(GPIO_T15_BCU,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_BCU,X_ON); break;
    }
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_INV){
        case DS_OFF: x_out_set_value(GPIO_T15_INV,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_INV,X_ON); break;
    }
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_Steering){
        case DS_OFF: x_out_set_value(GPIO_T15_STEERING,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_STEERING,X_ON); break;
    }
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_Screen){
        case DS_OFF: x_out_set_value(GPIO_T15_SCREEN,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_SCREEN,X_ON); break;
    }
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_Digi){
        case DS_OFF: x_out_set_value(GPIO_T15_DIGI,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_DIGI,X_ON); break;
    }
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_BTMS){
        case DS_OFF: x_out_set_value(GPIO_T15_BTMS,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_BTMS,X_ON); break;
    }
    switch (st_GPIO.un_Outs_T15.bits.b2_T15_Buzzer){
        case DS_OFF: x_out_set_value(GPIO_T15_BUZZER,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_T15_BUZZER,X_ON); break;
    }

}
void GPIO_Update_PDU_Outputs(void){
    switch (st_GPIO.un_Outs_PDU.bits.b2_PDU_Discharge){
        case DS_OFF: x_out_set_value(GPIO_PDU_DISCHARGE,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_PDU_DISCHARGE,X_ON); break;
    }
    switch (st_GPIO.un_Outs_PDU.bits.b2_PDU_Precharge){
        case DS_OFF: x_out_set_value(GPIO_PDU_PRECHARGE,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_PDU_PRECHARGE,X_ON); break;
    }
    switch (st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Main){
        case DS_OFF: x_out_set_value(GPIO_PDU_INVERTER,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_PDU_INVERTER,X_ON); break;
    }
    switch (st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_BTMS){
        case DS_OFF: x_out_set_value(GPIO_PDU_BTMS,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_PDU_BTMS,X_ON); break;
    }
    switch (st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_AC){
        case DS_OFF: x_out_set_value(GPIO_PDU_AC,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_PDU_AC,X_ON); break;
    }
}

void GPIO_Update_Generic_Outputs(void){
    switch (st_GPIO.un_Outs_Gen.bits.b2_HVIL){
        case DS_OFF: x_out_set_value(GPIO_HVIL_OUT,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_HVIL_OUT,X_ON); break;
    }
    switch (st_GPIO.un_Outs_Gen.bits.b2_TPDU_Compresor){
        case DS_OFF: x_out_set_value(GPIO_TPDU_COMP,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_TPDU_COMP,X_ON); break;
    }
    switch (st_GPIO.un_Outs_Gen.bits.b2_TPDU_AC){
        case DS_OFF: x_out_set_value(GPIO_TPDU_AC,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_TPDU_AC,X_ON); break;
    }
    switch (st_GPIO.un_Outs_Gen.bits.b2_Cooling_Tank){
        case DS_OFF: x_out_set_value(GPIO_OUT_CTANK,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_OUT_CTANK,X_ON); break;
    }
    switch (st_GPIO.un_Outs_Gen.bits.b2_Heating_Tank){
        default: break;
        /*
        case DS_OFF: x_out_set_value(GPIO_OUT_HTANK,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_OUT_HTANK,X_ON); break;
        */
    }
    switch (st_GPIO.un_Outs_Gen.bits.b2_Steering_Tank){
        /* case DS_OFF: x_out_set_value(GPIO_OUT_STANK,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_OUT_STANK,X_ON); break; */
        default: break;
    }
    switch (st_GPIO.un_Outs_Gen.bits.b2_Steering_Contactor){
        case DS_OFF: x_out_set_value(GPIO_STEERING_CONT,X_OFF); break;
        case DS_ON: x_out_set_value(GPIO_STEERING_CONT,X_ON); break;
    }
}

void GPIO_Update_Buzzer(void){
    sint32 s32_ref_duty = st_GPIO.u16_Buzzer_Duty_10mpu;
    x_out_set_value(GPIO_BUZZER_SPEED,s32_ref_duty);
    x_out_set_frequency(GPIO_BUZZER_SPEED,st_GPIO.u32_Buzzer_Freq_mHz);
}

uint8 GPIO_get_T15_BCU(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_BCU; }
uint8 GPIO_get_T15_Inv(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_INV; }
uint8 GPIO_get_T15_Steering(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_Steering; }
uint8 GPIO_get_T15_Screen(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_Screen; }
uint8 GPIO_get_T15_Digi(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_Digi; }
uint8 GPIO_get_T15_BTMS(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_BTMS; }
uint8 GPIO_get_T15_Buzzer(void){ return st_GPIO.un_Outs_T15.bits.b2_T15_Buzzer; }

uint8 GPIO_get_PDU_Discharge(void){ return st_GPIO.un_Outs_PDU.bits.b2_PDU_Discharge; }
uint8 GPIO_get_PDU_Precharge(void){ return st_GPIO.un_Outs_PDU.bits.b2_PDU_Precharge; }
uint8 GPIO_get_PDU_Contactor_Main(void){ return st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Main; }
uint8 GPIO_get_PDU_Contactor_BTMS(void){ return st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_BTMS; }
uint8 GPIO_get_PDU_Contactor_AC(void){ return st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_AC; }

uint8 GPIO_get_HVIL_Ref(void){ return st_GPIO.un_Outs_Gen.bits.b2_HVIL; }
uint8 GPIO_get_TPSU_Compressor(void){ return st_GPIO.un_Outs_Gen.bits.b2_TPDU_Compresor; }
uint8 GPIO_get_TPDU_AC(void){ return st_GPIO.un_Outs_Gen.bits.b2_TPDU_AC; }
uint8 GPIO_get_Dapig_Override(void){ return st_GPIO.un_Outs_Gen.bits.b2_Dapig_Override; }
uint8 GPIO_get_Cooling_Tank_Ref(void){ return st_GPIO.un_Outs_Gen.bits.b2_Cooling_Tank; }
uint8 GPIO_get_Heating_Tank_Ref(void){ return st_GPIO.un_Outs_Gen.bits.b2_Heating_Tank; }
uint8 GPIO_get_Steering_Tank_Ref(void){ return st_GPIO.un_Outs_Gen.bits.b2_Steering_Tank; }
uint8 GPIO_get_Steering_Contactor(void){ return st_GPIO.un_Outs_Gen.bits.b2_Steering_Contactor; }


uint16 GPIO_get_T15_Refs(void){ return st_GPIO.un_Outs_T15.all; }
uint16 GPIO_get_PDU_Refs(void){ return st_GPIO.un_Outs_PDU.all; }
uint16 GPIO_get_Gen_Refs(void){ return st_GPIO.un_Outs_Gen.all; }

void GPIO_set_T15_BCU(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_T15.bits.b2_T15_BCU = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_T15_Inv(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_T15.bits.b2_T15_INV = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_T15_Steering(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_T15.bits.b2_T15_Steering = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_T15_Screen(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR) {
        st_GPIO.un_Outs_T15.bits.b2_T15_Screen = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_T15_Digi(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_T15.bits.b2_T15_Digi = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_T15_BTMS(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_T15.bits.b2_T15_BTMS = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_T15_Buzzer(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_T15.bits.b2_T15_Buzzer = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}

void GPIO_set_PDU_Discharge(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Discharge = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_PDU_Precharge(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Precharge = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_PDU_Contactor_Main(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_Main = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_PDU_Contactor_BTMS(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_BTMS = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }

}
void GPIO_set_PDU_Contactor_AC(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_PDU.bits.b2_PDU_Contactor_AC = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}

void GPIO_set_HVIL_Ref(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_HVIL = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_TPDU_Compressor(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_TPDU_Compresor = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_TPDU_AC(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_TPDU_AC = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_Dapig_Override(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_Dapig_Override = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}

void GPIO_set_Cooling_Tank(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_Cooling_Tank = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_Heating_Tank(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_Heating_Tank = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_Steering_Tank(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_Steering_Tank = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_Steering_Contactor(uint8 u8_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        st_GPIO.un_Outs_Gen.bits.b2_Steering_Contactor = (u8_Out_Ref & 0x3);
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}


void GPIO_set_Buzzer_Frequency_mHz(uint32 u32_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        if(u32_Out_Ref > 20000000u)
            u32_Out_Ref = 20000000u;
        if(u32_Out_Ref < 5000u)
            u32_Out_Ref = 5000u;
        st_GPIO.u32_Buzzer_Freq_mHz = u32_Out_Ref;
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}
void GPIO_set_Buzzer_Duty_10mpu(uint16 u16_Out_Ref){
    if(x_os_mutex_get(&t_Mutex_GPIO,10) == C_NO_ERR){
        if(u16_Out_Ref > 10000u)
            u16_Out_Ref = 10000u;
        st_GPIO.u16_Buzzer_Duty_10mpu = u16_Out_Ref;
        x_os_mutex_release(&t_Mutex_GPIO);
    }
}


