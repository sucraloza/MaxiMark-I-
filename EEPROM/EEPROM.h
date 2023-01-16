#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

#include "OS_Defs.h"

/*Define of vars save in the EEPROM*/
#define ODOMETER_EEPROM_ADDRESS ((uint32) 1)
#define ODOMETER_DATA_SIZE ((uint32) 4)

#define HVIL_EEPROM_ADDRESS ((uint32) 10)
#define HVIL_DATA_SIZE ((uint32) 1)

#define AC_TYPE_EEPROM_ADDRESS ( (uint32) 20)
#define AC_TYPE_DATA_SIZE ((uint32) 1)

#define EEPROM_DISABLE 0x00
#define EEPROM_ENABLE  0x03

#define EEPROM_HVIL_DISABLE 0x00
#define EEPROM_HVIL_ENABLE  0x03

#define EEPROM_ODOMETER_ID  1
#define EEPROM_HVIL_ID      2
#define EEPROM_AC_TYPE_ID   3


typedef struct
{
    uint8 u8_EEPROM_Enable;
    uint8 u8_EEPROM_HVIL_Enable;
    uint8 u8_EEPROM_AC_Type;
} St_EEPROM;

extern sint16 EEPROM_Write( const uint32 ou32_Address, const uint32 ou32_Count,  uint8 * const opu8_Data);
extern sint16 EEPROM_Read(const uint32 ou32_Address, const uint32 ou32_Count,  uint8 * const opu8_Data);

extern St_EEPROM st_EEPROM;

extern void EEPROM_Init(uint8 u8_CAN_Chanel);

extern void EEPROM_Rx(void);

extern uint8 EEPROM_get_HVIL_Enable(void);
extern uint8 EEPROM_get_AC_Type(void);

#endif // EEPROM_H_INCLUDED
