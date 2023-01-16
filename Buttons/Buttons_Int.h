#ifndef BUTTONS_INTEGRATION_H_INCLUDED
#define BUTTONS_INTEGRATION_H_INCLUDED

//----- VISION --------
#define BUTTON_CHANGE_PAGE  1

//----- GEAR ------
#define BUTTON_DIRECT   5
#define BUTTON_NEUTRAL  4
#define BUTTON_REVERSE  3

#define BUTTONS_GEAR_D  1
#define BUTTONS_GEAR_N  0
#define BUTTONS_GEAR_R  -1

typedef struct
{
    uint8 b2_Direct:2;
    uint8 b2_Neutral:2;
    uint8 b2_Reverse:2;
    uint8 b2_Change_Page:2;
} T_Buttons_Flags;

typedef union
{
    uint8 all;
    T_Buttons_Flags bits;
} U_Buttons_Flags;


typedef struct
{
    U_Buttons_Flags u_Flags;
    uint8 u8_Reset_State;
    uint8 u8_Config_Complete;
} T_Buttons_Vars;


extern void Buttons_Init(void);
extern uint8 Buttons_Reset(void);
extern uint8 Buttons_Update(void);
extern void Buttons_Clear_Gear(void);
extern void Buttons_Set_Neutral(void);
extern void Buttons_Gear_Fix_Led(void);
extern sint8 Buttons_Get_Gear(void);

void Buttons_Handle_Change_Page(uint8 u8_change_page);
extern uint8 Buttons_Get_Change_Page(void);

#endif /* BUTTONS_INTEGRATION_H_INCLUDED */
