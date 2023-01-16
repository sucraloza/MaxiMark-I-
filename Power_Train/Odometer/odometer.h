#ifndef ODOMETER_H_INCLUDED
#define ODOMETER_H_INCLUDED

#include "OS_Defs.h"

extern sint16 Odometer_Save(void);
extern sint16 Odometer_Get_Km(float32 *pf32_odometer);
extern void Odometer_Set_Speed(uint8 u8_kmh);
extern void Odometer_Set_Km(float32 f32_km);

#endif // ODOMETER_H_INCLUDED
