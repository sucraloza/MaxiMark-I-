#ifndef GLOBAL_CONTROLS_H_INCLUDED
#define GLOBAL_CONTROLS_H_INCLUDED

extern void System_Output_Update(void);

extern void System_Input_Update(void);

//System State Functions
extern uint8 System_get_Traction_Ready(void);
extern void System_Change_State(uint8 u8_New_State);
extern uint8 System_get_Drive_Block(void);
extern void System_set_Drive_Block(uint8 u8_Block_ref);
extern uint8 System_get_Charge_Lock(void);
extern void System_set_Charge_Lock(uint8 u8_Charge_Lock);
extern uint8 System_get_Charge_State_Detected(void);
extern void System_set_Charge_State_Detected(uint8 u8_Charge_Lock);
extern uint8 System_get_State(void);
extern uint8 System_get_DCLink_State(void);
extern uint8 System_get_Motor_Tank_Level(void);
extern uint8 System_get_Heating_Tank_Level(void);
extern uint8 System_get_Steering_Tank_Level(void);
extern uint8 System_get_Drivers_Feedback(void);
extern void System_set_Drivers_Feedback(uint8 u8_Feedback);


//System Power OnOff Functions
extern uint8 System_Power_OnOff_Get_Timeout(void);
extern void System_Power_On_Goto_State(uint8 u8_New_State);
extern void System_Power_Off_Goto_State(uint8 u8_New_State);

extern uint8 System_get_Power_On_State(void);
extern uint8 System_get_Power_Off_State(void);

//System Vehicle OnOff Functions
extern uint8 System_Vehicle_OnOff_Get_Timeout(void);
extern void System_Vehicle_On_Goto_State(uint8 u8_New_State);
extern void System_Vehicle_Off_Goto_State(uint8 u8_New_State);

extern void Error_Beep(void);
extern void Error_Beepers(void);

extern uint8 System_get_Aux_State(void);

//TODO Throttle
extern float32 Throttle_Get_Thottle_pu(void);
extern uint8 Throttle_Get_State(void);
#define THROTTLE_CERO 0

extern void System_PDU_To_Battery(void);

extern void System_Compresor_Detect(uint8 u8_rpm);

#endif // GLOBAL_CONTROLS_H_INCLUDED
