#ifndef PANEL_CAN_H_INCLUDED
#define PANEL_CAN_H_INCLUDED

#define PANEL_FAST_COUNTER_LIMIT 5
#define PANEL_SLOW_COUNTER_LIMIT 20

extern void Panel_Tx_Speed(void);
extern void Panel_Tx_Indicators(void);
extern void Panel_Tx_Buzzer(void);

extern void Send_Panel(void);
#endif // PANEL_CAN_H_INCLUDED
