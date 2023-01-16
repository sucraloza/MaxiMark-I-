#ifndef THROTTLE_CONTROL_H_INCLUDED
#define THROTTLE_CONTROL_H_INCLUDED


extern void Throttle_Conversion(uint16 IN_A,uint16 IN_B);
extern float64 Throttle_Filter(float32 f32_X0);
extern float32 Throttle_Hysteresis(float f32_in);
extern float32 Throttle_Ramp(float32 f32_in);
extern float32 Throttle_Regen(float32 f32_in);
extern float32 Throttle_Down_Ramp(float32 f32_in);

extern void Get_Drive_Throttle(void);

#endif // THROTTLE_CONTROL_H_INCLUDED
