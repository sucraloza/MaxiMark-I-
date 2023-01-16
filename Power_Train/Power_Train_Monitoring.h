#ifndef POWER_TRAIN_MONITORING_H_INCLUDED
#define POWER_TRAIN_MONITORING_H_INCLUDED

extern void Inverter_Monitoring_State(void);
extern void Inverter_Monitoring_Refs_1(void);
extern void Inverter_Monitoring_Refs_2(void);
extern void Inverter_Monitoring_Electrical(void);
extern void Inverter_Monitoring_Mechanical_1(void);
extern void Inverter_Monitoring_Mechanical_2(void);
extern void Inverter_Monitoring_Cooling(void);
extern void Inverter_Monitoring_Faults(void);
extern void Inverter_Monitoring_Limits(void);
extern void Inverter_Monitoring_Coms(void);

extern uint8 INMON_Counter;

extern void Inverter_Monitoring_Send(void);

#endif // POWER_TRAIN_MONITORING_H_INCLUDED
