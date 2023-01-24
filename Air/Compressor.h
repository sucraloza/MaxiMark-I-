#ifndef COMPRESSOR_H_INCLUDED
#define COMPRESSOR_H_INCLUDED

// -------- Configuration--------

#define COMPRESSOR_OK_TIMEOUT_MS    30000u
#define COMPRESSOR_START_BLAST_MS   60000u
#define COMPRESSOR_BLAST_MS         25000u
#define COMPRESSOR_TIMEOUT_MS       120000u
#define COMPRESSOR_WAIT_MS          30000u
#define COMPRESSOR_HEAT_MS          300000u

#define COMPRESSOR_MIN_PRESSURE_MBAR 8000
#define COMPRESSOR_OFF_PRESSURE_MBAR 9000

#define COMPRESSOR_OFF_SPEED_RPM   1000

#define COMPRESOR_TEMP_ACTIVATION_LIMIT_C   110
#define COMPRESOR_TEMP_CLEAR_LIMIT_C        100

// -------- State definitions --------
#define COMPRESSOR_STATE_DISABLE            0x00
#define COMPRESSOR_STATE_COMPRESOR_OK       0x03
#define COMPRESSOR_STATE_START              0x05
#define COMPRESSOR_STATE_ON                 0x06
#define COMPRESSOR_STATE_BLAST              0x09
#define COMPRESSOR_STATE_STANDBY            0x0A
#define COMPRESSOR_STATE_OVERHEAT           0x0C
#define COMPRESSOR_STATE_TIMEOUT            0x0F
#define COMPRESSOR_STATE_COMPRESOR_OK_HEAT  0x11
#define COMPRESSOR_STATE_HEATING            0x12
#define COMPRESSOR_STATE_OFF                0x14
#define COMPRESSOR_STATE_FAULT              0xFF

// -------- Variables --------
typedef struct{
    uint8 u8_State;
    uint8 u8_Fault_Leak;
    uint8 u8_Fault_Motor;
    uint8 u8_OverHeat;
    uint8 u8_Charge_Air;
    uint8 u8_Compresor_Ok;
    uint16 u16_Air_Pressure_mbar;
    sint16 s16_Speed_rpm;
    uint32 u32_Timeout;
    sint8 s8_Temperatura_C;
} T_Compressor_Vars;

// -------- Public functions --------
//Initialization
extern void Compressor_Init(void);
//Getters
extern uint8 Compressor_get_Ok(void);
extern uint8 Compressor_get_State(void);
extern uint8 Compressor_get_Charge_Air(void);
extern uint8 Compressor_get_Fault_Leak(void);
extern uint8 Compressor_get_Fault_Motor(void);
extern uint8 Compressor_get_Over_Heat(void);
//Monitor
extern void Compresor_Monitor(uint32 u32_ID, void* oppv_Mailbox);
//Machine State
extern void Compressor_Run(void);

// -------- Private functions --------
void Compressor_State_Disable(void);
void Compressor_State_Compresor_Ok(void);
void Compressor_State_Start(void);
void Compressor_State_On(void);
void Compressor_State_Blast(void);
void Compressor_State_Standby(void);
void Compressor_State_Overheat(void);
void Compressor_State_Timeout(void);
void Compressor_State_Compresor_Ok_Heat(void);
void Compressor_State_Heating(void);
void Compressor_State_Off(void);
void Compressor_State_Fault(void);

void Compressor_Overheat(void);

#endif // COMPRESSOR_H_INCLUDED
