#ifndef TACHOGRAPH_CAN_H_INCLUDED
#define TACHOGRAPH_CAN_H_INCLUDED

#define TACHOGRAPH_COUNTER_LIMIT            10
#define TACHOGRAPH_COUNTER_SLOW_5           5
#define TACHOGRAPH_COUNTER_SLOW_10          2
#define TACHOGRAPH_COUNTER_SLOW_100         10

//AGRALE TACHOGRAPH
//Urea level indicator
#define UREA_OFF                             0x00
#define UREA_ON_SOLID                        0x01
#define UREA_RESERVED1                       0x02
#define UREA_RESERVED2                       0x03
#define UREA_ON_FASTBLINK                    0x04

// MutEx
extern T_x_os_mutex t_Mutex_Tacho_Msg;

// CAN vars
extern void *mpv_MsgObjTx_Tachograph;       // module global defined TX object for Tachograph
extern void *mpv_MsgObjRx_Tachograph;       // module global defined RX object for Tachograph
extern void *mpv_MsgObjRx_TachographBug;       // module global defined RX object for Tachograph
extern T_x_can_msg t_Tacho_Msg;             //Message Holder RX object

// Funcs
extern void Tachograph_TX_AT1T1(void);      // used
extern void Tachograph_TX_CCVS1_1(void);    // used
extern void Tachograph_TX_DD(void);
extern void Tachograph_TX_TCO1(void);       // used
extern void Tachograph_TX_TRF2(void);       // used
extern void Tachograph_TX_WFI(void);        // used
extern void Tachograph_TX_EEC1(void);       // used
extern void Tachograph_TX_ET1(void);        // used
extern void Tachograph_TX_EBC1(void);       // used
extern void Tachograph_TX_EFL_P1_2(void);   // used
extern void Tachograph_TX_ETC7(void);       // used


extern void Send_Tachograph(void);
extern void Tacho_receive(void);
extern void Tachograph_RX_CCVS1(void);
extern void Tachograph_RX_EFL_P1(void);

extern void Tacho_eval(void);

#endif // TACHOGRAPH_CAN_H_INCLUDED
