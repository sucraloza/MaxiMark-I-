#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "OS_Defs.h"

#define BUZZER_DEACTIVATE ((uint8) 0)
#define BUZZER_ACTIVATE ((uint8) 3)


void Buzzer_Set_Speed(uint8 u8_kmh);

void Buzzer_Init(void);
#endif // _BUZZER_H_
