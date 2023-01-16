#ifndef BATTERIES_CONTROL_H_INCLUDED
#define BATTERIES_CONTROL_H_INCLUDED

extern void Battery_Machine_State(void);

extern void Battery_Update_Life_Signal(void);
extern void Battery_Set_Power_On(void);
extern void Battery_Set_Power_Off(void);
extern void Baterry_Set_Pos_Contator(uint8 u8_status);
extern void Baterry_Set_Aux_Contator(uint8 u8_status);
extern void Baterry_Set_Speed_RPM(sint16 s16_speed_RPM);
extern uint8 Battery_Is_Ready(void);
extern uint8 Battery_Is_Open(void);
extern uint8 Battery_get_Emergency_Off(void);
extern sint16 Battery_get_Pack_Current(void);
extern uint16 Battery_get_Pack_Voltage_V(void);
extern uint16 Battery_get_ImpulseDischarge_Current(void);
extern uint16 Battery_get_ImpulseCharge_Current(void);
extern uint16 Battery_get_Charge_Current(void);
extern uint8 Battery_get_SOC(void);
extern uint8 Battery_get_Gun_Connection(void);
extern uint8 Battery_get_Not_Charging(void);
extern uint8 Battery_get_Charging(void);
extern uint8 Battery_get_Charge_Finished(void);
extern uint8 Battery_get_Charging_Error(void);
extern sint8 Battery_get_Temperature(void);
extern uint8 Battery_get_Heating(void);
extern uint8 Battery_get_Cooling(void);
extern uint8 Battery_get_Failure(void);
extern uint8 Battery_get_Catastrofic_Failure(void);
extern uint16 Battery_get_Isolation_kohm(void);
extern uint16 Battery_get_Isolation_Pos_kohm(void);
extern uint16 Battery_get_Isolation_Neg_kohm(void);
extern uint8 Battery_get_BTMS_Error(void);
extern uint32 Battery_get_Error_Flags_1(void);
extern uint64 Battery_get_Error_Flags_2(void);
extern uint8 Battery_get_BMS_HV_State(void);

extern void Battery_Check_CAN(void);
extern void Battery_Reset_CAN_Counter(void);
extern uint8 Battery_Get_CAN_OK(void);


#endif // INTERRUPTS_H_INCLUDED
