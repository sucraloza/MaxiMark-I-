#ifndef POWER_TRAIN_CONTROL_H_INCLUDED
#define POWER_TRAIN_CONTROL_H_INCLUDED

void Inverter_Safe_Send(sint16 s16_Error_Inv);

extern uint8 Inverter_Get_Potocol_Flag(void);
extern uint8 Inverter_Get_Operational_State(void);
extern uint8 Inverter_Get_Operational_Mode(void);
extern uint8 Inverter_Get_Command_Mode(void);
extern uint8 Inverter_Get_Thermal_Indicator(void);
extern float32 Inverter_Get_Power_Current_A(void);
extern uint16 Inverter_Get_Power_Voltage_V(void);
extern sint16 Inverter_Get_Speed_rpm(void);
extern sint16 Inverter_Get_Filtered_Speed_rpm(void);
extern sint16 Inverter_get_Speed_kmh(void);
extern uint16 Inverter_Get_Available_Drive_Torque_Nm(void);
extern uint16 Inverter_Get_Available_Regen_Torque_Nm(void);
extern uint8 Inverter_Get_EmergencyStop(void);
extern uint8 Inverter_Get_Derrating(void);
extern float32 Inverter_get_Reverse_Speed_derrating_pu(void);
extern uint8 Inverter_Get_Error_Flags(void);
extern uint16 Inverter_Get_Error_Code(void);
extern float Inverter_Get_Auxiliary_Voltage_V(void);
extern uint8 Inverter_Is_Vehicle_Stop(void);

extern void Inverter_Set_Potocol_Flag_False(void);
extern void Inverter_Set_Operation_State(uint8 u8_Ref_State);
extern void Inverter_Set_Operation_Mode(uint8 u8_Ref_Mode);
extern void Inverter_Set_Command_Mode(uint8 u8_Ref_Mode);
extern void Inverter_Set_Torque_nm(float32 f32_Ref_Torque_nm);
extern void Inverter_Set_Discharge_Current_A(uint16 u16_Ref_Discharge_Current_A);
extern void Inverter_Set_Charge_Current_A(uint16 u16_Ref_Charge_Current_A);
extern void Inverter_Set_Max_Voltage_V(uint16 u8_Ref_Max_Voltage_V);
extern void Inverter_Set_Min_Voltage_V(uint16 u8_Ref_Min_Voltage);

extern void Inverter_Filter_Speed(void);
extern void Inverter_Calculate_Torque(void);

float Inverter_Get_Avg_Current_A(void);

extern float Inverter_Get_Avg_Torque_nm(void);
extern float Inverter_Get_Max_Torque_nm(void);
extern float Inverter_Get_Min_Torque_nm(void);
extern sint8 Inverter_Get_Avg_Speed_kmh(void);
extern sint8 Inverter_Get_Max_Speed_kmh(void);
extern void Inverter_Reset_Max_Torque_nm(void);
extern void Inverter_Reset_Min_Torque_nm(void);
extern void Inverter_Reset_Max_Speed_kmh(void);



#endif // INTERRUPTS_H_INCLUDED
