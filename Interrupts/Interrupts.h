#ifndef INTERRUPTS_H_INCLUDED
#define INTERRUPTS_H_INCLUDED

extern void vTask_Outputs(void *opv_Arg);
extern void vTask_Fault(void *opv_Arg);
extern void vTask_State_Machine(void *opv_Arg);
extern void vTask_Inputs(void *opv_Arg);
extern void vTask_CAN_RX(void *opv_Arg);
extern void vTask_Monitoring(void *opv_Arg);

#endif // INTERRUPTS_H_INCLUDED
