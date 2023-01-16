#include "OS_Defs.h"
#include "Filter.h"

void Filter_Init(T_Filter_Vars *pFilter, float32 f32_Gain_A0, float32 f32_Gain_A1, float64 f64_Gain_B1){
    (*pFilter).f32_Gain_A0 = f32_Gain_A0;
    (*pFilter).f32_Gain_A1 = f32_Gain_A1;
    (*pFilter).f64_Gain_B1 = f64_Gain_B1;

    (*pFilter).f64_GX = 0;
    (*pFilter).f64_GY = 0;

    (*pFilter).f32_X1 = 0;
    (*pFilter).f64_Y1 = 0;
}

float32 Filter(T_Filter_Vars *pFilter, float32 f32_X0){
    float32 f64_Y0;

    (*pFilter).f64_GX = (*pFilter).f32_Gain_A0 * f32_X0 + (*pFilter).f32_Gain_A1 * (*pFilter).f32_X1;
    (*pFilter).f64_GY =  - ((*pFilter).f64_Gain_B1) * ((*pFilter).f64_Y1);
    f64_Y0 = (*pFilter).f64_GX + (*pFilter).f64_GY ;

    (*pFilter).f32_X1 = f32_X0;
    (*pFilter).f64_Y1 = f64_Y0;

    return f64_Y0;
}

float32 Filter_get_Output(T_Filter_Vars *pFilter){
    return (*pFilter).f64_Y1;
}

