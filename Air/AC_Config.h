#ifndef AC_CONFIG_H_INCLUDED
#define AC_CONFIG_H_INCLUDED

typedef enum
{
    AC_CONFIG_NO_CONF = 0x0,
    AC_CONFIG_TPDU = 0x3,
    AC_CONFIG_DC_COMPRESSOR = 0xC,
} E_AC_Config_States;

typedef struct{
    E_AC_Config_States e_Type;
} T_AC_Config_Vars;


extern void AC_Config_Init(void);
extern void AC_Config_Run(void);


#endif // AC_CONFIG_H_INCLUDED
