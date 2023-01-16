#ifndef CAPL_H_INCLUDED
#define CAPL_H_INCLUDED

extern void CAPL_Send_Batt_On(void);
extern void CAPL_Send_Isometer_On(void);
extern void CAPL_Send_Votage_On(void);
extern void CAPL_Send_Votage_Off(void);

extern void CAPL_Send_Inv_Votage_On(void);
extern void CAPL_Send_Inv_Votage_Off(void);
extern void CAPL_Send_Inv_Operational(void);
extern void CAPL_Send_Inv_Standby(void);
extern void CAPL_Send_Inv_Power_Off(void);
extern void CAPL_Send_Inv_EV(void);
extern void CAPL_Send_Inv_Neutral(void);
extern void CAPL_Send_Batt_Off(void);
extern void CAPL_Send_Battery_Timer(void);
extern void CAPL_Send_Battery_Ign_Off(void);



extern void *mpv_MsgObjTx_CAPL; // module global defined TX object for CAPL


#endif // CAPL_H_INCLUDED
