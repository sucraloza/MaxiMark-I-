#ifndef STATE_MACHIENE_H_INCLUDED
#define STATE_MACHIENE_H_INCLUDED

extern void State_Sleep(void);
extern void State_Power_On(void);
extern void State_Auxiliary(void);
extern void State_Drive(void);
extern void State_Charge(void);
extern void State_Fault(void);
extern void State_Power_Off(void);

extern void Run_System_Machine_State(void);

#endif // STATE_MACHIENE_H_INCLUDED
