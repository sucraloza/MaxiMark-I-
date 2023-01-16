#ifndef JARVIS_FUNCTIONS_H_INCLUDED
#define JARVIS_FUNCTIONS_H_INCLUDED

extern void Jarvis_Set_Is_Regen(uint8 u8_Enable);
extern void Jarvis_Set_Rpm(float f32_rpm);

extern sint8 Jarvis_get_Aux_Current(void);
extern uint16 Jarvis_get_Pressure_mBar(void);
extern uint8 Jarvis_get_Button_Charge(void);
extern uint8 Jarvis_get_Button_Regen(void);
extern uint8 Jarvis_get_Button_Heating(void);
extern uint8 Jarvis_get_Button_Pedal_Break(void);
extern uint8 Jarvis_get_Button_Hand_Break(void);
extern uint8 Jarvis_get_PTC_Operation(void);

extern uint8 Jarvis_get_PTC1_State(void);
extern uint8 Jarvis_get_PTC2_State(void);
extern uint16 Jarvis_get_PTC1_Power_W(void);
extern uint16 Jarvis_get_PTC2_Power_W(void);

//Vesion Functions
extern uint8 Jarvis_get_Major_Version(void);
extern uint8 Jarvis_get_Minor_Version(void);
extern uint8 Jarvis_get_Pach_Version(void);
extern En_VersionStatus Jarvis_get_Status_Version(void);

#endif // JARVIS_FUNCTIONS_H_INCLUDED
