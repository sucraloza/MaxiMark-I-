#include "OS_Defs.h"

void Throttle_Conversion(uint16 IN_A,uint16 IN_B)
{
    uint32 u32_Throttle_Value=0;
    float32 f32_Throttle_Output_pu;
    float32 f32_Throttle_Range;
    sint32 s32_Value_A = 0;
    sint32 s32_Value_B = 0;

    x_in_get_value(IN_A,&s32_Value_A);
    x_in_get_value(IN_B,&s32_Value_B);

    debuggeo.u8_debug4 = (s32_Value_A & 0xFF);
    debuggeo.u8_debug5 = ((s32_Value_A >> 8) & 0xFF);

    if(x_os_mutex_get(&t_Mutex_Throttle,10) != C_NO_ERR){
        return;
    }

    uint32 u32_thro_corr = (uint32)(s32_Value_A*0.5 - s32_Value_B);
    if (u32_thro_corr/s32_Value_B > 0.02)//Establecer la correlacion del pedal
    {
        //error
        st_Throttle.f32_Raw_Value_pu  = 0.0f;
        st_Throttle.un_Errors.bits.b2_Redudndancy_Check_Out_of_Range = TRUE;
        x_os_mutex_release(&t_Mutex_Throttle);
        return;
    }

    if ((s32_Value_A < 1000 || s32_Value_A > 4600) || (s32_Value_B < 500 || s32_Value_B > 2300))//
    {
       //error
       st_Throttle.f32_Raw_Value_pu = 0.0f;
       st_Throttle.un_Errors.bits.b2_Frecuency_A_Out_of_Range = TRUE;
       x_os_mutex_release(&t_Mutex_Throttle);
       return;
    }

    u32_Throttle_Value = (s32_Value_A + s32_Value_B*2)/2; //Promedio entre las dos senales.
    //Maximum and minimum throttle
    if (u32_Throttle_Value < st_Throttle.st_Conversion.u32_Min_Throttle)
    {
        st_Throttle.st_Conversion.u32_Min_Throttle = u32_Throttle_Value;
        if (st_Throttle.st_Conversion.u32_Min_Throttle < st_Throttle.st_Conversion.u32_Min_Limit){
            st_Throttle.st_Conversion.u32_Min_Throttle = st_Throttle.st_Conversion.u32_Min_Limit;
        }
    }
    if (u32_Throttle_Value > st_Throttle.st_Conversion.u32_Max_Throttle)
    {
        st_Throttle.st_Conversion.u32_Max_Throttle = u32_Throttle_Value;
        if (st_Throttle.st_Conversion.u32_Max_Throttle > st_Throttle.st_Conversion.u32_Max_Limit){
            st_Throttle.st_Conversion.u32_Max_Throttle = st_Throttle.st_Conversion.u32_Max_Limit;
        }
    }
    debuggeo.u8_debug6 = (u32_Throttle_Value & 0xFF);
    debuggeo.u8_debug7 = ((u32_Throttle_Value >> 8) & 0xFF);
    
    //Output scale
    f32_Throttle_Range = (float32)(st_Throttle.st_Conversion.u32_Max_Throttle-st_Throttle.st_Conversion.u32_Min_Throttle);
    uint16 temp = (st_Throttle.st_Conversion.u32_Max_Throttle - st_Throttle.st_Conversion.u32_Min_Throttle);
    debuggeo.u8_debug1 = (temp & 0xFF);
    debuggeo.u8_debug2 = ((temp >> 8) & 0xFF);
    f32_Throttle_Output_pu = u32_Throttle_Value - st_Throttle.st_Conversion.u32_Min_Throttle;
    f32_Throttle_Output_pu = f32_Throttle_Output_pu / f32_Throttle_Range;
    if(f32_Throttle_Output_pu < 0){
        f32_Throttle_Output_pu = 0;}
    if(f32_Throttle_Output_pu > 1.0f){
        f32_Throttle_Output_pu = 1.0f;}
    st_Throttle.f32_Raw_Value_pu = f32_Throttle_Output_pu;
    x_os_mutex_release(&t_Mutex_Throttle);
}

float64 Throttle_Filter(float32 f32_X0){
    float64 f64_Y0;

    if(x_os_mutex_get(&t_Mutex_Throttle,10) != C_NO_ERR){
        st_Throttle.f64_Filtered_Value_pu = 0;
        return 0;
    }

    st_Throttle.st_Low_Pass_Filter.f64_GX = st_Throttle.st_Low_Pass_Filter.f32_Gain_A0 * f32_X0 + st_Throttle.st_Low_Pass_Filter.f32_Gain_A1 * st_Throttle.st_Low_Pass_Filter.f32_X1;
    st_Throttle.st_Low_Pass_Filter.f64_GY =  - (st_Throttle.st_Low_Pass_Filter.f64_Gain_B1) * (st_Throttle.st_Low_Pass_Filter.f64_Y1);
    f64_Y0 = st_Throttle.st_Low_Pass_Filter.f64_GX + st_Throttle.st_Low_Pass_Filter.f64_GY ;
    //y0 = 0.2 x0 + 0.8 y1;


    st_Throttle.st_Low_Pass_Filter.f32_X1 = f32_X0;
    st_Throttle.st_Low_Pass_Filter.f64_Y1 = f64_Y0;

    st_Throttle.f64_Filtered_Value_pu = f64_Y0;
    x_os_mutex_release(&t_Mutex_Throttle);
    return f64_Y0;
}

float32 Throttle_Hysteresis(float f32_in){
    uint8 u8_Gear = Gear_get_State();

    if(x_os_mutex_get(&t_Mutex_Throttle,10) != C_NO_ERR){
        st_Throttle.f32_Post_Hysteresis_Value_pu = 0;
        return 0;
    }
    if(u8_Gear != GEAR_DIRECT && u8_Gear != GEAR_REVERSE){ // ECU en cambio distinto a directa y a reversa
        st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu = 0.0f;
        st_Throttle.f32_Post_Hysteresis_Value_pu = st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu;
        x_os_mutex_release(&t_Mutex_Throttle);
        return 0.0f;        // pedal a 0%;
    }
    if(f32_in < st_Throttle.st_Hysteresis.f32_Limit_Off){   // si el pedal esta bajo de un 2.5% se asume que est en 0%
        st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu = 0.0f;
        st_Throttle.f32_Post_Hysteresis_Value_pu = st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu;
        x_os_mutex_release(&t_Mutex_Throttle);
        return 0.0f;
    }
    if(f32_in > st_Throttle.st_Hysteresis.f32_Limit_On){
        st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu = f32_in;
        st_Throttle.f32_Post_Hysteresis_Value_pu = st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu;
        x_os_mutex_release(&t_Mutex_Throttle);
        return f32_in;
    }
    // Comment to appear
    st_Throttle.f32_Post_Hysteresis_Value_pu = st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu;
    x_os_mutex_release(&t_Mutex_Throttle);
    return st_Throttle.st_Hysteresis.f32_Preveous_Throttle_pu;
}

float32 Throttle_Ramp(float32 f32_in){

    if(x_os_mutex_get(&t_Mutex_Throttle,10) != C_NO_ERR){
        return f32_in;
    }

    if(f32_in > st_Throttle.st_Ramp.f32_Buffer){
        if((f32_in - st_Throttle.st_Ramp.f32_Buffer) > st_Throttle.st_Ramp.f32_Step_Up){
            st_Throttle.st_Ramp.u8_Counter++;
            if(st_Throttle.st_Ramp.u8_Counter >= st_Throttle.st_Ramp.u8_Limit_Counter){
                st_Throttle.st_Ramp.f32_Buffer =  st_Throttle.st_Ramp.f32_Buffer + st_Throttle.st_Ramp.f32_Step_Up;
                st_Throttle.st_Ramp.u8_Counter = 0;
            }
        }
        else{
            st_Throttle.st_Ramp.f32_Buffer = f32_in;
        }
    }

    if(f32_in < st_Throttle.st_Ramp.f32_Buffer){
        st_Throttle.st_Ramp.f32_Buffer = f32_in;
    }

    if(st_Throttle.st_Ramp.f32_Buffer > 1.0f ||  st_Throttle.st_Ramp.f32_Buffer < 0.0f){
        st_Throttle.st_Ramp.f32_Buffer = f32_in;
    }
    st_Throttle.f32_Post_Ramp_Value_pu = st_Throttle.st_Ramp.f32_Buffer;
    x_os_mutex_release(&t_Mutex_Throttle);
    return st_Throttle.st_Ramp.f32_Buffer;
}

float32 Throttle_Regen(float32 f32_in_Throttle_pu){
//float32 Regen_Pedal(float32 f32_Pedal_entrada, uint8 u8_Estado_Cambio){
    float32 f32_Filtered_Speed_rpm;           // Velocidad del motor filtrada en rpm
    float64 f64_Regen_Present_pu;                 // Porcentaje de Regeneracion
    float64 f64_Derrating_Regen_Limit;           // Limite de Regeneracion derateado por velocidad
    float32 f32_Speed_Derrating_Present_pu;    // Porcentaje de derateo por velocidad del motor
    uint8 u8_Gear = Gear_get_State();

    if(x_os_mutex_get(&t_Mutex_Throttle,10) != C_NO_ERR){  //Se obtine el mutex del pedal
        st_Throttle.f32_Post_Regen_Value_pu = 0.0f;
        return 0.0f; // si no se puede pedal a 0
    }

    if(u8_Gear != GEAR_DIRECT){        // Si no estamos en marcha directa
        st_Throttle.f32_Post_Regen_Value_pu = f32_in_Throttle_pu;
        x_os_mutex_release(&t_Mutex_Throttle);
        return f32_in_Throttle_pu;                 //  retornamos el pedal de entrada
    }

    //Chequeo de parametros
    if(st_Throttle.st_Regen.f32_Max_Regen < 0.0f) // La regeneracion maxima no puede ser un numero negativo
        st_Throttle.st_Regen.f32_Max_Regen = 0.0f;

    if(st_Throttle.st_Regen.f32_Max_Regen > 0.5f) // La regeneracion maxima no puede ser mayor a 50%
        st_Throttle.st_Regen.f32_Max_Regen = 0.5f;

    if(st_Throttle.st_Regen.f32_Disipation_Present_Start> 2.0f) // el inicio de dicipacion de limit regen no puede ser mas de 2 veces limit regen
        st_Throttle.st_Regen.f32_Disipation_Present_Start = 2.0f;

    if(st_Throttle.st_Regen.f32_Disipation_Present_Start < 1.0f) // el inicio de dicipacion de limit regen no puede ser menor de que limit regen
        st_Throttle.st_Regen.f32_Disipation_Present_Start = 1.0f;

    //No ocupe un mutex por que es una seccion critica del codigo y solo estoy extrallendo la variable y no modificandola
    //TODO INVERTER GET FILTERED SPEED
    f32_Filtered_Speed_rpm = Inverter_Get_Filtered_Speed_rpm(); // se obtien la velocidad filtrada del motor en rpm
    //f32_Filtered_Speed_rpm = 0;
    //Si el pedalde entrada esta en cero y la velocidad es mas de 300rpm entonces cominso a incrementar la regeneracion limite segun el step configurado
    if(f32_in_Throttle_pu <0.005f){
        st_Throttle.st_Regen.f32_Limit_Regen = st_Throttle.st_Regen.f32_Limit_Regen + st_Throttle.st_Regen.f32_Stocking_Regen_Step;
    }

    // Si el pedal de entrada supera 1.33 el limite de regeneracion entonces comienso a disminuir el limite de regeneracion segun el step
    if(f32_in_Throttle_pu >= st_Throttle.st_Regen.f32_Disipation_Present_Start * st_Throttle.st_Regen.f32_Limit_Regen){
        st_Throttle.st_Regen.f32_Limit_Regen = st_Throttle.st_Regen.f32_Limit_Regen - st_Throttle.st_Regen.f32_Disipation_Regen_Step;
    }
    //Si el limite de regeneracion es mayor a el maximo que se puede regenerar saturo en el maximo configurado
    if(st_Throttle.st_Regen.f32_Limit_Regen > st_Throttle.st_Regen.f32_Max_Regen)
        st_Throttle.st_Regen.f32_Limit_Regen = st_Throttle.st_Regen.f32_Max_Regen;

    //Si el limite de regeneracion es inferior a cero saturo en cero
    if(st_Throttle.st_Regen.f32_Limit_Regen < 0)
        st_Throttle.st_Regen.f32_Limit_Regen = 0;


    //Calcular el derrateo por velocidad
    f32_Speed_Derrating_Present_pu =(f32_Filtered_Speed_rpm-st_Throttle.st_Regen.f32_Minimun_Speed_rpm)/(st_Throttle.st_Regen.f32_Derrating_Speed_rpm-st_Throttle.st_Regen.f32_Minimun_Speed_rpm);

    //Si el porcentaje de derrateo es mayor a 1 saturo en 1
    if(f32_Speed_Derrating_Present_pu > 1.0f)
        f32_Speed_Derrating_Present_pu = 1.0f;

    //Si el porcentaje de derrateo es menor a 0 saturo en 0
    if(f32_Speed_Derrating_Present_pu < 0.10f)
        f32_Speed_Derrating_Present_pu = 0.10f;

    // si la velocidad del bus es menor a la velocidad minima de regeneracion entonces el limite de regeneracion es 0
    // la trancision es suebe ya que anteriormente el limite se comenso a deratear
    if(f32_Filtered_Speed_rpm < st_Throttle.st_Regen.f32_Minimun_Speed_rpm)
        st_Throttle.st_Regen.f32_Limit_Regen = 0;

    //Derratear limite de regeneracion en caso de que la velocidad sea baja
    f64_Derrating_Regen_Limit = f32_Speed_Derrating_Present_pu * st_Throttle.st_Regen.f32_Limit_Regen;

    //Calculo de ganancia de regeneracion segun prametro de regeneracion maxima actual
    f64_Regen_Present_pu = (f32_in_Throttle_pu*(1+f64_Derrating_Regen_Limit))-f64_Derrating_Regen_Limit;
    //MON_TX_Throttle_1(f32_in_Throttle_pu,st_Throttle.st_Regen.f32_Limit_Regen,f64_Derrating_Regen_Limit,f64_Regen_Present_pu);
    st_Throttle.f32_Post_Regen_Value_pu = f64_Regen_Present_pu;
    x_os_mutex_release(&t_Mutex_Throttle); // livero el mutex del pedal
    return f64_Regen_Present_pu; // retorno el porcentaje de regeneracion calculado
}

float32 Throttle_Down_Ramp(float32 f32_in){
    uint8 u8_Gear = Gear_get_State();

    if(x_os_mutex_get(&t_Mutex_Throttle,10) != C_NO_ERR){
        st_Throttle.f32_Final_Value_pu = 0.0f;
        return 0.0f;
    }

    if(u8_Gear != GEAR_DIRECT){   // Si no estamos en marcha directa
        st_Throttle.st_Down_Ramp.f32_Buffer = 0;           // Buffer debe reiniciarce
        st_Throttle.f32_Final_Value_pu = f32_in;
        x_os_mutex_release(&t_Mutex_Throttle);
        return f32_in;                                      //  retornamos el pedal de entrada
    }

    if(f32_in >= 0){                                        // Si el torque es positivo
        st_Throttle.st_Down_Ramp.f32_Buffer = 0;            // Buffer debe reiniciarce
        st_Throttle.f32_Final_Value_pu = f32_in;
        x_os_mutex_release(&t_Mutex_Throttle);
        return f32_in;                                      //  retornamos el pedal de entrada
    }

    //Chequeo de parametros
    if(st_Throttle.st_Down_Ramp.f32_Max_Regen < 0.0f) // La regeneracion maxima no puede ser un numero negativo
        st_Throttle.st_Down_Ramp.f32_Max_Regen = 0.0f;

    if(st_Throttle.st_Down_Ramp.f32_Max_Regen > 0.5f) // La regeneracion maxima no puede ser mayor a 50%
        st_Throttle.st_Down_Ramp.f32_Max_Regen = 0.5f;

    if(st_Throttle.st_Down_Ramp.f32_Buffer > f32_in){
        if(st_Throttle.st_Down_Ramp.f32_Buffer - f32_in < st_Throttle.st_Down_Ramp.f32_Stocking_Step_Regen){
            st_Throttle.st_Down_Ramp.f32_Buffer = f32_in;
        }else{
            st_Throttle.st_Down_Ramp.f32_Buffer = st_Throttle.st_Down_Ramp.f32_Buffer - st_Throttle.st_Down_Ramp.f32_Stocking_Step_Regen;
        }
    }
    else{
        st_Throttle.st_Down_Ramp.f32_Buffer = f32_in;
    }

    if(st_Throttle.st_Down_Ramp.f32_Buffer < -st_Throttle.st_Down_Ramp.f32_Max_Regen)
        st_Throttle.st_Down_Ramp.f32_Buffer = -st_Throttle.st_Down_Ramp.f32_Max_Regen;

    if(st_Throttle.st_Down_Ramp.f32_Buffer > 1.0f)
        st_Throttle.st_Down_Ramp.f32_Buffer = 1.0f;

    st_Throttle.f32_Final_Value_pu = st_Throttle.st_Down_Ramp.f32_Buffer;
    x_os_mutex_release(&t_Mutex_Throttle);
    return st_Throttle.st_Down_Ramp.f32_Buffer;
}

void Get_Drive_Throttle(void){
    Throttle_Conversion(GPIO_THROTTLE_A,GPIO_THROTTLE_B);
    Throttle_Filter(st_Throttle.f32_Raw_Value_pu);
    Throttle_Hysteresis(st_Throttle.f64_Filtered_Value_pu);
    Throttle_Ramp(st_Throttle.f32_Post_Hysteresis_Value_pu);
    Throttle_Regen(st_Throttle.f32_Post_Ramp_Value_pu);
    Throttle_Down_Ramp(st_Throttle.f32_Post_Regen_Value_pu);
    if(st_Throttle.f32_Final_Value_pu < 0.0f)
        Jarvis_Set_Is_Regen(TRUE);
    else
        Jarvis_Set_Is_Regen(FALSE);
}
