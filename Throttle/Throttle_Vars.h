#ifndef THROTTLE_VARS_H_INCLUDED
#define THROTTLE_VARS_H_INCLUDED

typedef struct{
    uint8 b2_Frecuency_A_Out_of_Range:2;
    uint8 b2_Frecuency_B_Out_of_Range:2;
    uint8 b2_Redudndancy_Check_Out_of_Range:2;
    uint8 rsv:2;
}St_Throttle_Errors;

typedef union{
    uint8 all;
    St_Throttle_Errors bits;
}Un_Throttle_Errors;

typedef struct{
    uint32 u32_Max_Throttle;
    uint32 u32_Min_Throttle;
    uint32 u32_Max_Limit;
    uint32 u32_Min_Limit;
}St_Throttle_Convesion;

#define THROTTLE_CONVESION_INIT(v)      \
    v.u32_Max_Throttle = 4200;         \
    v.u32_Min_Throttle = 1100;         \
    v.u32_Max_Limit = 4600;            \
    v.u32_Min_Limit = 1000;

typedef struct{
    float32 f32_Limit_On;                   //Umbral de actrivacion del pedal
    float32 f32_Limit_Off;                  //Umbral de desactivacion del Pedal
    float32 f32_Preveous_Throttle_pu;       //Valor del pedal en la iteracion anterior
} St_Throttle_Hysteresis;

#define THROTTLE_HYSTERESIS_INIT(v)         \
    v.f32_Limit_On = 0.05;                  \
    v.f32_Limit_Off = 0.025;                \
    v.f32_Preveous_Throttle_pu = 0.0;

typedef struct{
    float32 f32_Step_Up;
    float32 f32_Step_Down;
    float32 f32_Buffer;
    uint8 u8_Counter;
    uint8 u8_Limit_Counter;
}St_Throttle_Ramp;

#define THROTTLE_RAMPA_INIT(v)         \
    v.f32_Step_Up = 0.004f;            \
    v.f32_Step_Down = 0.1f;            \
    v.f32_Buffer = 0.0f;               \
    v.u8_Counter = 0;                  \
    v.u8_Limit_Counter = 1;

typedef struct{
  float32 f32_Max_Regen;                // Maxima regeneracion que puede ser alcansada por el bus [0,0.5]
  float32 f32_Derrating_Speed_rpm;      // por debajo de esta velocidad el parametro la variable limit_Regen cominsa a ser derateada [rpm]
  float32 f32_Minimun_Speed_rpm;        // por debajo de esta velocidad el Limit_Regen se hace cero [rpm]
  float32 f32_Stocking_Regen_Step;      // tasa de cambio de la variable Limit_Regen durante el incremento de regeneracion 0,001 => 10% de regeneracion en 1 seg
  float32 f32_Disipation_Regen_Step;    // tasa de cambio de la variable Limit_Regen durante la disipacion de regeneracion 0,001 => 10% de regeneracion en 1 seg
  float32 f32_Limit_Regen;              // Limite de la regeneracion actual
  float32 f32_Disipation_Present_Start; // porcentaje del pedal que debe ser superado para que se inicie la dicipacion [1.0, 2.0] recomendado 1.33
} St_Throttle_Regen;

#define THROTTLE_REGEN_INIT(v)              \
  v.f32_Max_Regen = 0.3;                    \
  v.f32_Derrating_Speed_rpm = 300;          \
  v.f32_Minimun_Speed_rpm = 100;            \
  v.f32_Stocking_Regen_Step = 0.003;        \
  v.f32_Disipation_Regen_Step = 0.004;      \
  v.f32_Limit_Regen = 0;                    \
  v.f32_Disipation_Present_Start = 0;

typedef struct{
    float32 f32_Max_Regen;                    // Maxima regeneracion que puede ser alcansada por el bus [0,0.5]
    float32 f32_Stocking_Step_Regen;       // tasa de cambio de la variable Limit_Regen durante el incremento de regeneracion 0,001 => 10% de regeneracion en 1 seg
    float32 f32_Buffer;                       // Variable de integracion
}St_Throttle_Down_Ramp;

#define THROTTLE_DOWN_RAMP_INIT(v)            \
    v.f32_Max_Regen = 0.5;                    \
    v.f32_Stocking_Step_Regen = 0.003;        \
    v.f32_Buffer = 0;

typedef struct{
    Un_Throttle_Errors un_Errors;
    St_Throttle_Convesion st_Conversion;
    St_Filter st_Low_Pass_Filter;
    St_Throttle_Hysteresis st_Hysteresis;
    St_Throttle_Ramp st_Ramp;
    St_Throttle_Regen st_Regen;
    St_Throttle_Down_Ramp st_Down_Ramp;
    float32 f32_Raw_Value_pu;
    float32 f64_Filtered_Value_pu;
    float32 f32_Post_Hysteresis_Value_pu;
    float32 f32_Post_Ramp_Value_pu;
    float32 f32_Post_Regen_Value_pu;
    float32 f32_Final_Value_pu;
}St_Throttle_Component;

#define THROTTLE_FILTER_INIT(v)         \
    v.f32_Gain_A0 = 0.20f;              \
    v.f32_Gain_A1 = 0.0f;               \
    v.f64_Gain_B1 = -0.80f;             \
    v.f32_X1 = 0.0f;                    \
    v.f64_Y1 = 0.0f;                    \
    v.f64_GX = 0.0f;                    \
    v.f64_GY = 0.0f;

#define INIT_THROTTLE(v)                            \
    v.un_Errors.all = 0;                            \
    THROTTLE_CONVESION_INIT(v.st_Conversion)        \
    THROTTLE_FILTER_INIT(v.st_Low_Pass_Filter)      \
    THROTTLE_HYSTERESIS_INIT(v.st_Hysteresis)       \
    THROTTLE_RAMPA_INIT(v.st_Ramp)                  \
    THROTTLE_REGEN_INIT(v.st_Regen)                 \
    THROTTLE_DOWN_RAMP_INIT(v.st_Down_Ramp)         \
    v.f32_Raw_Value_pu = 0;                         \
    v.f64_Filtered_Value_pu = 0;                    \
    v.f32_Post_Hysteresis_Value_pu = 0;             \
    v.f32_Post_Ramp_Value_pu = 0;                   \
    v.f32_Post_Regen_Value_pu = 0;                  \
    v.f32_Final_Value_pu = 0;


extern St_Throttle_Component st_Throttle;
extern T_x_os_mutex t_Mutex_Throttle;

#endif // THROTTLE_VARS_H_INCLUDED
