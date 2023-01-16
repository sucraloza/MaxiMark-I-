#ifndef FUSI_CONTROLS_H_INCLUDED
#define FUSI_CONTROLS_H_INCLUDED

extern void Fusi_Eep_Load(void);
extern void Fusi_Eep_Save(void);

extern void Fusi_from_Eeprom_to_Fusi(void);
extern void Fusi_from_Fusi_to_Eeprom(void);

extern void Fusi_Check_Start(void);

extern void Fusi_Check_DCLink(void);
extern void Fusi_Check_Emergency_Stop(void);
extern void Fusi_Check_Isolation(void);
extern void Fusi_Check_Battery_Failure(void);
extern void Fusi_Check_Battery_Catastrofic_Failure(void);
extern void Fusi_Check_CAN_3(void);
extern void Fusi_Check_CAN_1(void);
extern void Fusi_Check_Motor_Failure(void);
extern void Fusi_Check_Motor_CAN(void);
extern void Fusi_Check_CAN_2(void);
extern void Fusi_HVIL(void);
extern void Fusi_Check_Contactor_cant_Close(void);
extern void Fusi_Check_Contactor_is_Welded(void);
extern void Fusi_Check_Precharge_Failure(void);
extern void Fusi_Check_Discharge_Failure(void);
extern void Fusi_Charge_Failure(void);
extern void Fusi_ACU_CAN(void);
extern void Fusi_Low_Pressure_Critical(void);
extern void Fusi_Battery_OverTemp(void);
extern void Fusi_Steering_Failure(void);
extern void Fusi_Steering_CAN(void);
extern void Fusi_Low_SOC_Critical(void);
extern void Fusi_PSU_Not_Working(void);
extern void Fusi_PSU_Low_Voltage(void);
extern void Fusi_PSU_Fault(void);
extern void Fusi_DCU_Fault(void);
extern void Fusi_Ignition_Off_Drive(void);
extern void Fusi_Regen_Limit(void);
extern void Fusi_Check_Motor_Coolant_Level(void);
extern void Fusi_Check_Heating_Coolant_Level(void);
extern void Fusi_Reset_Vison_Responce_Limit(void);

extern void Fusi_Set_Battery_CAN(uint8 u8_ref);
extern void Fusi_Set_Power_Off_Inverter(uint8 u8_ref);
extern void Fusi_Set_Power_Off_Current(uint8 u8_ref);
extern void Fusi_Set_Power_Off_PDU(uint8 u8_ref);
extern void Fusi_Set_Power_Off_Battery(uint8 u8_ref);
extern void Fusi_Set_Power_On_PDU_Circuits(uint8 u8_ref);
extern void Fusi_Set_Power_On_Protocol_Inverter(uint8 u8_ref);
extern void Fusi_Set_Power_On_Battery_CAN(uint8 u8_ref);
extern void Fusi_Set_Power_On_Battery_Ready(uint8 u8_ref);
extern void Fusi_Set_Power_On_PDU_Close(uint8 u8_ref);
extern void Fusi_Set_Power_On_HVIL(uint8 u8_ref);

void Fusi_Get_Battery_CAN(void);
void Fusi_Get_Power_Off_Inverter(void);
void Fusi_Get_Power_Off_Current(void);
void Fusi_Get_Power_Off_PDU(void);
void Fusi_Get_Power_Off_Battery(void);
void Fusi_Get_Power_On_PDU_Circuits(void);
void Fusi_Get_Power_On_Protocol_Inverter(void);
void Fusi_Get_Power_On_Battery_CAN(void);
void Fusi_Get_Power_On_Battery_Ready(void);
void Fusi_Get_Power_On_PDU_Close(void);
void Fusi_Get_Power_On_HVIL(void);

extern uint8 Fusi_Change_State_Check(void);

extern float32 Fusi_get_speed_Derrating(sint16 s16_Speed_rpm);
extern uint8 Fusi_get_Check_Engine(void);
extern uint8 Fusi_get_Request_Stop(void);
extern uint8 Fusi_get_Motor_Off(void);
extern uint8 Fusi_get_No_Pressure(void);
extern uint8 Fusi_get_Buzzer(void);
extern uint8 Fusi_get_Limit_Speed(void);
extern uint8 Fusi_get_Pressure_Ok(void);
extern uint8 Fusi_get_Vision_No_Responce(void);
extern uint8 Fusi_get_Block_Power_On(void);
extern uint8 Fusi_get_Power_Off_Park(void);
extern uint8 Fusi_get_Power_Off_Now(void);

extern void Fusi_Acction_Check_Engine(void);
extern void Fusi_Acction_Buzzer(void);
extern void Fusi_Acction_Block_Power_ON(void);
extern void Fusi_Acction_Park_Request(void);
extern void Fusi_Acction_Limit_Speed(void);
extern void Fusi_Acction_Power_Off_On_Stop(void);
extern void Fusi_Acction_Power_Off_Now(void);

extern void Fusi_Execute(void);

extern uint8 Fusi_get_DTC_Level(void);
extern uint16 Fusi_get_DTC(void);
extern uint8 Fusi_get_Change_State_Check_Status(void);

void Fusi_set_DTC(uint16 u16_DTC);
extern void Fusi_Init_DTCs(void);



#endif // FUSI_CONTROLS_H_INCLUDED
