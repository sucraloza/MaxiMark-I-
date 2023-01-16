#ifndef TELEMETRY_CONTROLS_H_INCLUDED
#define TELEMETRY_CONTROLS_H_INCLUDED

extern void Telemetry_Track_ECU_State(uint8 u8_New);
extern void Telemetry_Track_Key_State(uint8 u8_New);
extern void Telemetry_Track_Battery_Heating(uint8 u8_New);
extern void Telemetry_Track_BTMS(uint8 u8_New);
extern void Telemetry_Track_Gun_Connection(uint8 u8_New);
extern void Telemetry_Track_BMS_HV_State(uint8 u8_New);
extern void Telemetry_Track_Compresor_Use(uint8 u8_New);
extern void Telemetry_Track_PTC1(uint8 u8_New);
extern void Telemetry_Track_PTC2(uint8 u8_New);
extern void Telemetry_Track_HVIL_Reference(uint8 u8_New);
extern void Telemetry_Track_HVIL_Feedback(uint8 u8_New);

#endif // TELEMETRY_CONTROLS_H_INCLUDED
