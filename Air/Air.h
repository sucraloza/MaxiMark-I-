#ifndef AIR_H_INCLUDED
#define AIR_H_INCLUDED

#define AIR_OFF_SPEED_RPM   250

typedef struct{
    uint32 u32_Start_Run;
    uint32 u32_Start_Blast;
    uint32 u32_Start_Heat;
    uint8 u8_Start_Ok;
    uint16 u16_Air_Pressure_mbar;
    uint8 u8_Charge_Air;
} T_Air_Vars;


extern void Air_Init(void);
extern void Air_Disable(void);
extern void Air_Run(void);

extern uint8 Air_get_Compresor_Ok(void);

#endif /* AIR_H_INCLUDED */
