#ifndef FUSI_CAN_H_INCLUDED
#define FUSI_CAN_H_INCLUDED

extern void Fusi_TX_Flags_System(uint8 u8_CAN);
extern void Fusi_TX_Flags_Battery(uint8 u8_CAN);
extern void Fusi_TX_Flags_PDU(uint8 u8_CAN);
extern void Fusi_TX_Flags_Motor(uint8 u8_CAN);
extern void Fusi_TX_Flags_Steering(uint8 u8_CAN);
extern void Fusi_TX_Flags_AuxInverter(uint8 u8_CAN);
extern void Fusi_TX_Flags_Air(uint8 u8_CAN);
extern void Fusi_TX_Flags_Heating(uint8 u8_CAN);

extern void Fusi_TX_Flags_Acctions(uint8 u8_CAN);
extern void Fusi_TX_DTC_History(uint8 u8_CAN);

extern void Fusi_RX_Clear(void);

extern void Fusi_Send(void);
extern void Fusi_Recived(void);


#endif // FUSI_CAN_H_INCLUDED
