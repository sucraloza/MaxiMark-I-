#ifndef POWER_TRAIN_CAN_H_INCLUDED
#define POWER_TRAIN_CAN_H_INCLUDED

//Configuracion protocolo de Comunicacion
extern void Inverter_Tx_Protocol(void);
extern void Inverter_Tx_VmuPullingVmuResp(void);
extern void Inverter_Rx_McuPullingVmu(void);
extern void Inverter_Rx_McuPullingMcuResp(void);

//Mensajes periodicos que se le debe enviar al Inv
extern void Inverter_Tx_VmuCommand1(void);
extern void Inverter_Tx_VmuCommand2(void);
extern void Inverter_Tx_VmuCommandSafety(void);

//Mensajes que llegan a la ECU desde el Inv
extern void Inverter_Rx_McuCommand1Resp(void);
extern void Inverter_Rx_McuCommand2Resp(void);
extern void Inverter_Rx_McuInfo1(void);
extern void Inverter_Rx_McuInfo2(void);
extern void Inverter_Rx_McuEventInfo1(void);
extern void Inverter_Rx_McuOnEventInfo1(void);
extern void Inverter_Rx_McuThermal1(void);

extern void Inverter_Received(void);
extern void Inverter_Send(void);
#endif // INTERRUPTS_H_INCLUDED
