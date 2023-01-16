#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

typedef struct{
    float32 f32_Gain_A0;
    float32 f32_Gain_A1;
    float32 f64_Gain_B1;
    float64 f64_Y1;
    float32 f32_X1;
    float32 f64_GX;
    float64 f64_GY;
} T_Filter_Vars;


extern void Filter_Init(T_Filter_Vars *pFilter, float32 f32_Gain_A0, float32 f32_Gain_A1, float64 f64_Gain_B1);
extern float32 Filter(T_Filter_Vars *pFilter, float32 f32_X0);
extern float32 Filter_get_Output(T_Filter_Vars *pFilter);




#endif /* FILTER_H_INCLUDED */
