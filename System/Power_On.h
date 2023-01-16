#ifndef POWER_ON_H_INCLUDED
#define POWER_ON_H_INCLUDED

#define POWER_ON_FAULT_PDU_CIRUIT             0x01
#define POWER_ON_FAULT_ISOLATION_TIMEOUT      0x02
#define POWER_ON_FAULT_ISOLATION_GND          0x04
#define POWER_ON_FAULT_ISOLATION_ALARM        0x08
#define POWER_ON_FAULT_INVERTER_PROTOCOL      0x10
#define POWER_ON_FAULT_BATTERY_TIMEOUT        0x20
#define POWER_ON_FAULT_CLOSE_TIMEOUT          0x40
#define POWER_ON_FAULT_FREE8                  0x80

#define POWER_OFF_FAULT_INV_STANDBY           0x01
#define POWER_OFF_FAULT_AUX_CURRENT_TIMEOUT   0x02
#define POWER_OFF_FAULT_BATTERY_TIMEOUT       0x04
#define POWER_OFF_FAULT_OPEN_TIMEOUT          0x08
#define POWER_OFF_FAULT_FREE5                 0x10
#define POWER_OFF_FAULT_FREE6                 0x20
#define POWER_OFF_FAULT_FREE7                 0x40
#define POWER_OFF_FAULT_FREE8                 0x80

extern void Power_On_Fault_Triger(uint8 u8_Mask_Fault);
extern void Power_Off_Fault_Triger(uint8 u8_Mask_Fault);

extern void Power_On_Check_Circuits(void);
extern void Power_On_Power_T15(void);
extern void Power_On_Buttons(void);
extern void Power_On_Battery_CAN(void);
extern void Power_On_Protocol_Inverter(void);
extern void Power_On_Check_Battery_Ready(void);
extern void Power_On_PDU_Close(void);
extern void Power_On_Fault(void);

extern void Power_Off_Check_Inv_Standby(void);
extern void Power_Off_Disable_Systems(void);
extern void Power_Off_Battery_Open(void);
extern void Power_Off_PDU_Open(void);
extern void Power_Off_Wait(void);
extern void Power_Off_Power_Off_T15(void);
extern void Power_Off_Wait_Battery(void);
extern void Power_Off_Fault(void);


#endif // POWER_ON_H_INCLUDED
