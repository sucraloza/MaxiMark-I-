#ifndef AUXILIARY_FUNTIONS_H_INCLUDED
#define AUXILIARY_FUNTIONS_H_INCLUDED

extern uint16 System_Get_Aux_Current(void);
extern void System_Set_Inv_Limits(void);
extern void System_Detect_Drive_Block(void);
extern uint8 System_get_Is_Regen(void);

extern void Update_Vision(void);
extern void Update_Vision_Batteries(void);
extern void Update_Vision_Converter(void);
extern void Update_Vision_System(void);
extern void Update_Vision_System_Critical(void);
extern void Update_Vision_Versions(void);
extern void Update_Vision_Fusi(void);

extern uint16 System_Get_Isolation(void);
extern float32 System_get_PDU_Current(void);

extern void Battery_Thermal_Management(void);
extern void Update_Buzzer(void);

extern uint8 get_EEPROM_State(void);
#endif // AUXILIARY_FUNTIONS_H_INCLUDED
