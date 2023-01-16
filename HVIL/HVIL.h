#ifndef HVIL_H_INCLUDED
#define HVIL_H_INCLUDED

#define HVIL_OPEN       0x0
#define HVIL_CLOSE      0x3

#define HVIL_OPERATION_OFF      0x0
#define HVIL_OPERATION_WAIT     0x3
#define HVIL_OPERATION_ON       0x5
#define HVIL_OPERATION_ERROR    0x6

#define HVIL_FILTER_GAIN_A0  0.0728f
#define HVIL_FILTER_GAIN_A1  0.0728f
#define HVIL_FILTER_GAIN_B1  -0.8543f

#define HVIL_WAIT_COUNTER_LIMIT 20 // 20 Cycles of HVIL_set_Feedback

typedef struct
{
    uint8 u8_State;
    uint8 u8_Feedback;
    T_Filter_Vars t_Filter;
    uint8 u8_Counter;
}t_HVIL_Vars;


extern void HVIL_Init(void);
extern void HVIL_Activate(void);
extern void HVIL_Deactivate(void);
extern uint8 HVIL_get_State(void);
extern uint8 HVIL_get_State(void);
extern uint8 HVIL_is_Close(void);
extern uint8 HVIL_get_Feedback(void);

extern uint8 HVIL_get_ref_GPIO(void);
extern void HVIL_set_Feedback(uint8 u8_HVIL_Input);
extern void HVIL_Save(void);


#endif /* HVIL_H_INCLUDED */
