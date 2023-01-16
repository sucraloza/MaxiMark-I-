#ifndef PDU_CONTROLS_H_INCLUDED
#define PDU_CONTROLS_H_INCLUDED


//Access Functions
extern void PDU_Set_Open(void);
extern void PDU_Set_Close(void);
extern uint8 PDU_Is_Open(void);
extern uint8 PDU_Is_Closed(void);

extern uint8 PDU_Get_Fault(void);

//Machine States
extern void PDU_Machine_State(void);

extern uint8 PDU_get_State(void);

extern uint8 PDU_get_Fault_Precharge(void);
extern uint8 PDU_get_Fault_No_Voltage(void);
extern uint8 PDU_get_Fault_Bypass(void);
extern uint8 PDU_get_Fault_Welded(void);
extern uint8 PDU_get_Fault_Discharge(void);
extern uint8 PDU_get_DC_Charge_Failure(void);

extern sint16 PDU_Update_Battery_Voltage_V(uint16 u16_Batt_Voltage_V);

#endif
