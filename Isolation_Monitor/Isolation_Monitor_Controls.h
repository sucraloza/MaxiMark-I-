#ifndef ISOLATION_MONITOR_CONTROLS_H_INCLUDED
#define ISOLATION_MONITOR_CONTROLS_H_INCLUDED

uint16 Isometer_Get_Recistance_kOhm(void);
uint16 Isometer_Get_Voltage_V(void);
uint8 Isometer_Get_State(void);
uint8 Isometer_Get_Alarm(void);
uint8 Isometer_Get_Warking_State(void);
uint8 Isometer_Get_Isolation_Fault(void);

#endif // ISOLATION_MONITOR_CONTROLS_H_INCLUDED
