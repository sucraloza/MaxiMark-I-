#ifndef GEAR_H_INCLUDED
#define GEAR_H_INCLUDED

//GEAR VALUES
#define GEAR_PARK      0x00
#define GEAR_NEUTRAL   0x03
#define GEAR_DIRECT    0x05
#define GEAR_REVERSE   0x06

typedef struct
{
    uint8 u8_State;
}St_Gear_Vars;


extern void Gear_Init(void);
extern void Gear_Update(void);
extern uint8 Gear_get_State(void);

#endif /* GEAR_H_INCLUDED */
