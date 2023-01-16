#ifndef BATTERIES_CAN_H_INCLUDED
#define BATTERIES_CAN_H_INCLUDED

extern void Battery_TX_Command(void);
extern void Battery_TX_VIN(void);
extern void Battery_TX_Isometer(void);

extern void Battery_RX_Current_Limit(void);
extern void Battery_RX_Electric_Energy(void);
extern void Battery_RX_Fault_1(void);
extern void Battery_RX_Fault_2(void);
//extern void Battery_RX_Temperature(void);
extern void Battery_RX_Cell_Voltage(void);

extern void Battery_RX_Sate_1(void);
extern void Battery_RX_Sate_2(void);
extern void Battery_RX_Sate_3(void);
extern void Battery_RX_Sate_4(void);
extern void Battery_RX_Sate_5(void);
extern void Battery_RX_Sate_6(void);
extern void Battery_RX_Sate_7(void);

extern void Battery_Send_Msg(void);
extern uint8 Battery_Recived(void);



#endif // INTERRUPTS_H_INCLUDED
