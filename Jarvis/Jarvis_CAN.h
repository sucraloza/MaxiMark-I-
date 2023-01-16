#ifndef JARVIS_H_INCLUDED
#define JARVIS_H_INCLUDED

extern void Jarvis_TX_Command(void);
extern void Jarvis_RX_GPIO(void);
extern void Jarvis_RX_Version(void);
extern void Jarvis_RX_System(void);
extern void Jarvis_RX_PTC(void);

extern uint8 Jarvis_Receive(void);

#endif // JARVIS_H_INCLUDED
