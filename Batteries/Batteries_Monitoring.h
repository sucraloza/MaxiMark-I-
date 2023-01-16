#ifndef BATTERIES_MONITORING_H_INCLUDED
#define BATTERIES_MONITORING_H_INCLUDED

extern void Battery_Monitoring_Current_Limit(void);
extern void Battery_Monitoring_Electric_Energy(void);
extern void Battery_Monitoring_Fault_1(void);
extern void Battery_Monitoring_Fault_2(void);
extern void Battery_Monitoring_Temperature(void);
extern void Battery_Monitoring_Cell_Voltage(void);

extern void Batteries_Monitoring_State_1(void);
extern void Batteries_Monitoring_State_2(void);
extern void Batteries_Monitoring_State_3(void);
extern void Batteries_Monitoring_State_4(void);
extern void Batteries_Monitoring_State_5(void);
extern void Batteries_Monitoring_State_6(void);
extern void Batteries_Monitoring_State_7(void);

extern void Batteries_Send_Monitoring(void);

#endif // BATTERIES_MONITORING_H_INCLUDED
