#ifndef MONITORING_CAN_H_INCLUDED
#define MONITORING_CAN_H_INCLUDED

extern void MON_TX_Int_Ok(uint8 u8_var1, uint8 u8_var2);
extern void MON_TX_Error_Var(sint32 s32_Error);
extern void MON_TX_Var16(uint16 u16_Var);
extern void MON_TX_GPIO_State(void);
extern void MON_TX_PDU_Aux_Vars(void);
extern void MON_TX_System_Inputs_Vars(void);
extern void MON_TX_System_Status(void);
extern void MON_TX_Throttle(void);
extern void MON_TX_Throttle_1(float64 f64_In_1,float64 f64_In_2,float64 f64_In_3,float64 f64_In_4);
extern void MON_TX_Steering(void);

extern void MON_TX_TPDU(void);

extern void MON_RX_GPIO_Command(void);
extern void MON_RX_Control(void);



extern void Monitoring_Send(void);
extern void Monitoring_Recived(void);

extern void *mpv_MsgObjTx_Monitoring; // module global defined TX object for Monitoring
extern void *mpv_MsgObjRx_Monitoring; // module global defined RX object for Monitoring

extern T_x_can_msg t_Monitoring_Msg;          //Message Holder RX object
extern T_x_os_mutex t_Mutex_Monitoring_Msg;

#endif // INTERRUPTS_H_INCLUDED
