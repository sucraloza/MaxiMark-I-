#ifndef GPIO_PRIVATE_H_INCLUDED
#define GPIO_PRIVATE_H_INCLUDED
#include "GPIO_Defs.h"

//Variables
typedef struct{
    uint16 b2_T15_BCU:2;
    uint16 b2_T15_INV:2;
    uint16 b2_T15_Screen:2;
    uint16 b2_T15_Digi:2;
    uint16 b2_T15_Steering:2;
    uint16 b2_T15_BTMS:2;
    uint16 b2_T15_Buzzer:2;
    uint16 b2_rsv:2;
} St_T15_Outputs;

typedef union{
    uint16 all;
    St_T15_Outputs bits;
} Un_T15_Outputs;

typedef struct{
    uint16 b2_PDU_Discharge:2;
    uint16 b2_PDU_Precharge:2;
    uint16 b2_PDU_Contactor_Main:2;
    uint16 b2_PDU_Contactor_AC:2;
    uint16 b2_PDU_Contactor_BTMS:2;
    uint16 b2_rsv:6;
} St_PDU_Outputs;

typedef union{
    uint16 all;
    St_PDU_Outputs bits;
} Un_PDU_Outputs;

typedef struct{
    uint16 b2_HVIL:2;
    uint16 b2_TPDU_Compresor:2;
    uint16 b2_TPDU_AC:2;
    uint16 b2_Dapig_Override:2;
    uint16 b2_Cooling_Tank:2;
    uint16 b2_Heating_Tank:2;
    uint16 b2_Steering_Tank:2;
    uint16 b2_Steering_Contactor:2;
} St_Generic_Outputs;

typedef union{
    uint16 all;
    St_Generic_Outputs bits;
} Un_Generic_Outputs;

typedef struct{
    uint32 b2_Inertia:2;
    uint32 b2_Emergency:2;
    uint32 b2_Key_Accesory:2;
    uint32 b2_Key_Start:2;

    uint32 b2_Charge_Wakeup:2;
    uint32 b2_Cooling_Tank:2;
    uint32 b2_Heating_Tank:2;
    uint32 b2_Steering_Tank:2;

    uint32 b2_HVIL:2;
    uint32 b2_AC_Front:2;
    uint32 b2_AC_Rear:2;

    uint32 rsv:10;
} St_Digital_Inputs;

typedef union{
    uint32 all;
    St_Digital_Inputs bits;
} Un_Digital_Inputs;

typedef struct{
    Un_T15_Outputs un_Outs_T15;
    Un_PDU_Outputs un_Outs_PDU;
    Un_Generic_Outputs un_Outs_Gen;
    Un_Digital_Inputs un_Ins_Digital;
    uint32 u32_Buzzer_Freq_mHz;
    uint16 u16_Buzzer_Duty_10mpu;
    sint32 s32_Compresor_Current_uA;
} St_GPIO_Variables;

#define GPIO_INIT(v)                            \
    v.un_Outs_T15.all = 0;                      \
    v.un_Outs_PDU.all = 0;                      \
    v.un_Outs_Gen.all = 0;                      \
    v.un_Ins_Digital.all = 0;                   \
    v.u32_Buzzer_Freq_mHz = 0;                  \
    v.u16_Buzzer_Duty_10mpu = 0;                \
    v.s32_Compresor_Current_uA = 0;

//Private Functions
void GPIO_Update_T15_Outputs(void);
void GPIO_Update_PDU_Outputs(void);
void GPIO_Update_Generic_Outputs(void);
void GPIO_Update_Buzzer(void);

//Public Functions
extern void GPIO_Init(void);


extern void GPIO_Update_Inputs(void);
extern uint32 GPIO_get_Digital_Inputs(void);
extern uint8 GPIO_get_Inertia(void);
extern uint8 GPIO_get_Emergency(void);
extern uint8 GPIO_get_Key_Acc(void);
extern uint8 GPIO_get_Key_Start(void);
extern uint8 GPIO_get_Charge_Wakeup(void);
extern uint8 GPIO_get_Cooling_Tank(void);
extern uint8 GPIO_get_Heating_Tank(void);
extern uint8 GPIO_get_Steering_Tank(void);
extern uint8 GPIO_get_HVIL_Feedback(void);
extern uint8 GPIO_get_AC_Front(void);
extern uint8 GPIO_get_AC_Rear(void);
extern sint32 GPIO_get_Compresor_Current_uA(void);

extern void GPIO_Update_Outputs(void);

extern uint8 GPIO_get_T15_BCU(void);
extern uint8 GPIO_get_T15_Inv(void);
extern uint8 GPIO_get_T15_Steering(void);
extern uint8 GPIO_get_T15_Screen(void);
extern uint8 GPIO_get_T15_Digi(void);
extern uint8 GPIO_get_T15_BTMS(void);
extern uint8 GPIO_get_T15_Buzzer(void);

extern uint8 GPIO_get_PDU_Discharge(void);
extern uint8 GPIO_get_PDU_Precharge(void);
extern uint8 GPIO_get_PDU_Contactor_Main(void);
extern uint8 GPIO_get_PDU_Contactor_BTMS(void);
extern uint8 GPIO_get_PDU_Contactor_AC(void);

extern uint8 GPIO_get_HVIL_Ref(void);
extern uint8 GPIO_get_TPSU_Compressor(void);
extern uint8 GPIO_get_TPDU_AC(void);
extern uint8 GPIO_get_Dapig_Override(void);
extern uint8 GPIO_get_Cooling_Tank_Ref(void);
extern uint8 GPIO_get_Heating_Tank_Ref(void);
extern uint8 GPIO_get_Steering_Tank_Ref(void);
extern uint8 GPIO_get_Steering_Contactor(void);


extern void GPIO_set_T15_BCU(uint8 u8_Out_Ref);
extern void GPIO_set_T15_Inv(uint8 u8_Out_Ref);
extern void GPIO_set_T15_Steering(uint8 u8_Out_Ref);
extern void GPIO_set_T15_Screen(uint8 u8_Out_Ref);
extern void GPIO_set_T15_Digi(uint8 u8_Out_Ref);
extern void GPIO_set_T15_BTMS(uint8 u8_Out_Ref);
extern void GPIO_set_T15_Buzzer(uint8 u8_Out_Ref);

extern void GPIO_set_PDU_Discharge(uint8 u8_Out_Ref);
extern void GPIO_set_PDU_Precharge(uint8 u8_Out_Ref);
extern void GPIO_set_PDU_Contactor_Main(uint8 u8_Out_Ref);
extern void GPIO_set_PDU_Contactor_BTMS(uint8 u8_Out_Ref);
extern void GPIO_set_PDU_Contactor_AC(uint8 u8_Out_Ref);

extern void GPIO_set_HVIL_Ref(uint8 u8_Out_Ref);
extern void GPIO_set_TPDU_Compressor(uint8 u8_Out_Ref);
extern void GPIO_set_TPDU_AC(uint8 u8_Out_Ref);
extern void GPIO_set_Dapig_Override(uint8 u8_Out_Ref);
extern void GPIO_set_Cooling_Tank(uint8 u8_Out_Ref);
extern void GPIO_set_Heating_Tank(uint8 u8_Out_Ref);
extern void GPIO_set_Steering_Tank(uint8 u8_Out_Ref);
extern void GPIO_set_Steering_Contactor(uint8 u8_Out_Ref);

extern void GPIO_set_Buzzer_Frequency_mHz(uint32 u32_Out_Ref);
extern void GPIO_set_Buzzer_Duty_10mpu(uint16 u16_Out_Ref);

extern uint16 GPIO_get_T15_Refs(void);
extern uint16 GPIO_get_PDU_Refs(void);
extern uint16 GPIO_get_Gen_Refs(void);

extern St_GPIO_Variables st_GPIO;
#endif /* GPIO_PRIVATE_H_INCLUDED */
