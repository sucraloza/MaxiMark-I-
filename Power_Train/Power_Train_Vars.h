#ifndef POWER_TRAIN_VARS_H_INCLUDED
#define POWER_TRAIN_VARS_H_INCLUDED

typedef struct {
    uint32 b4_Operational_State:4;       //Current operational state of inverter
    uint32 b3_Operational_Mode:3;         //Current operational mode of inverter
    uint32 b1_Protocol_Set:1;
    uint32 b2_Command_Mode:2;
    uint32 rsv:6;
    uint32 b4_Traction_Derraiting:4;
    uint32 b4_Regen_Derraiting:4;
} st_Inverter_State_Flags;

typedef union {
    uint32 all;
    st_Inverter_State_Flags bits;
}Un_Inverter_State_Flags;

typedef struct{
    uint8 b2_Operation_State:2;
    uint8 b2_Operation_Mode:2;
    uint8 b2_Command_Mode:2;
    uint8 b1_Speed_Valid:1;
    uint8 b1_Torque_Valid:1;
} St_Inverter_Refs_Flags;

typedef union{
    uint8 all;
    St_Inverter_Refs_Flags bits;
} Un_Inverter_Refs_Flags;

typedef struct{
    Un_Inverter_Refs_Flags un_Flags;
    sint16 s16_Speed_rpm;
    sint16 s16_Safe_Speed_rpm;
    float32 f32_Torque_nm;
    float32 f32_Safe_Torque_nm;
} St_Inverter_Refs;

#define INVERTER_REFS_INIT(v)                   \
    v.un_Flags.all = 0;                         \
    v.un_Flags.bits.b2_Command_Mode = 1;        \
    v.s16_Speed_rpm = 0;                        \
    v.s16_Safe_Speed_rpm = 0;                   \
    v.f32_Torque_nm = 0;                        \
    v.f32_Safe_Torque_nm = 0;

typedef struct{
    float32  f32_Power_Current_A;
    uint16  u16_Power_Voltage_V;
    float32 f32_Auxiliary_Voltage_V;
    float32 f32_Avg_Current_A;
} St_Inverter_Electrical;

#define INVERTER_ELECTRICAL_INIT(v)         \
    v.f32_Power_Current_A = 0;              \
    v.u16_Power_Voltage_V = 0;              \
    v.f32_Auxiliary_Voltage_V = 0.0f;       \
    v.f32_Avg_Current_A = 0;

typedef struct{
    sint16 s16_Speed_rpm;                     //Velocidad Motor (max:32760rpm,min:-32760rpm,Offset:-32760rpm,0->6 y 65528->65535 invalidao, resolucion:1rpm)
    float32 f32_Estimated_Torque_nm;          //Estimacion del torque del motor (unidad:Nm,Resolucion:0.125Nm,Offset:-4096Nm,min:-4096Nm,Max:4096Nm,valores de 0->47 y 65489->65535 son invalidos)
    uint16 u16_Drive_Torque_nm;               //Torque disponible Traccion (unidad:Nm,Resolucion:0.125Nm,Offset:0Nm,min:0Nm,Max:4096Nm,valores de 32721->32767 son invalidos)
    uint16 u16_Regen_Torque_nm;               //Torque disponible Regeneracion (unidad:Nm,Resolucion:0.125Nm,Offset:0Nm,min:0Nm,Max:4096Nm,valores de 32721->32767 son invalidos)
    uint16 u16_Max_Foward_Speed_rpm;          //Maxima Velocidad a la que el motor puede operar de manera segura en directa(unidad:rpm,resolucion:100rpm,min:0rpm,max:32800rpm)
    uint16 u16_Max_Reverse_Speed_rpm;         //Maxima Velocidad a la que el motor puede operar de manera segura en reversa(unidad:rpm,resolucion:100rpm,min:0rpm,max:32800rpm)
    uint16 u16_Motor_Angle;                  //Angulo rotor (resolucion:1/2^16,Offset:0,min:0,max:65536)
    sint16 s16_Filtered_Speed_rpm;
    float32 f32_Avg_Torque_nm;
    float32 f32_Max_Meesured_Torque_nm;
    float32 f32_Min_Meesured_Torque_nm;
    float32 f32_Avg_Speed_kmh;
    sint16 s8_Max_Speed_kmh;
} St_Inverter_Mechanical;

#define INVERTER_MECHANICAL_INIT(v)                 \
    v.s16_Speed_rpm = 0;                            \
    v.f32_Estimated_Torque_nm = 0.0f;               \
    v.u16_Drive_Torque_nm = 0;                      \
    v.u16_Regen_Torque_nm = 0;                      \
    v.u16_Max_Foward_Speed_rpm = 0;                 \
    v.u16_Max_Reverse_Speed_rpm = 0;                \
    v.u16_Motor_Angle = 0;                          \
    v.s16_Filtered_Speed_rpm = 0;                   \
    v.f32_Avg_Torque_nm = 0;                        \
    v.f32_Max_Meesured_Torque_nm = 0;               \
    v.f32_Min_Meesured_Torque_nm = 0;               \
    v.f32_Avg_Speed_kmh = 0;                        \
    v.s8_Max_Speed_kmh = 0;

typedef struct{
    float32  f32_Inv_Required_Flow_pu;      //Flujo requerido de refrigerante inv (unidad,%del flujo max,res:100/128,min:0%,max:100%)
    float32  f32_Motor_Required_Flow_pu;    //Flujo requerido de refrigerante motor (unidad,%del flujo max,res:100/128,min:0%,max:100%)
    uint8    u8_Thermal_Indicator_pu;       //Temperatura del sistema de motorizacion(unidad:PU,res:100/128,min0,max:100,[0,40[ frio, [40,60[ tibio, [60;100] Caliete y degradado)
} St_Inverter_Cooling;

#define INVERTER_COOLING_INIT(v)                 \
    v.f32_Inv_Required_Flow_pu = 0.0f;           \
    v.f32_Motor_Required_Flow_pu = 0.0f;         \
    v.u8_Thermal_Indicator_pu = 0;

typedef struct{
    uint8  b1_Battery_Interlock:1;      //InterLock Battery (0:No Fault,1:Fault)
    uint8  b1_Motor_Interlock:1;        //InterLock Motor (0:No Fault,1:Fault)
    uint8  b1_Cover_Open:1;             //Tapa Inv Abierta (0:No Fault,1:Fault)
    uint8  b1_Emergency_Stop:1;         //Parada de emergency contactor DC se abrira(0:Sistema ok,1:Emergencia)
    uint8  b1_Comunication:1;           //CAN FAULT
    uint8  b1_Isolation:1;
    uint8  rsv:2;
}St_Inverter_Faults_Flags;

typedef union{
    uint8 all;
    St_Inverter_Faults_Flags bits;
}Un_Inverter_Faults_Flags;

typedef struct{
    uint16 u16_Inv_Event_Id;                    //Eventos Inv
    uint16 u16_Cnt_Falla_Inv_CAN;
    Un_Inverter_Faults_Flags un_Flags;
}St_Inverter_Faults;

#define INVERTER_FAULT_INIT(v)                  \
    v.u16_Inv_Event_Id = 0;                     \
    v.u16_Cnt_Falla_Inv_CAN = 0;                \
    v.un_Flags.all = 0;

typedef struct{
    uint8 b1_Is_Not_Valid_Discharge_Current:1;
    uint8 b1_Is_Not_Valid_Max_Voltage:1;
    uint8 b1_Is_Not_Valid_Min_Voltage:1;
    uint8 b1_Is_Not_Valid_Charge_Current:1;
    uint8 rsv:4;
}St_Inverter_Battery_Limits_Flags;

typedef union{
    uint8 all;
    St_Inverter_Battery_Limits_Flags bits;
}Un_Inverter_Battery_Limits_Flags;

typedef struct{
    uint16 u16_Max_Charge_Current_A;      //30A //Corriente maximo de carga de baterias (unidad:Ampere,min:0,max:4094,resolucion:1A)
    uint16 u16_Max_Discharge_Current_A;   //30A //Corriente maxima de descarga de baterias (unidad:Ampere,min:0,max:4094,resolucion:1A)
    uint16 u16_Max_Battery_Voltage_V;           //Tension maxima de baterias (unidad:Volt,min:0,max:4094,resolucion:1V)
    uint16 u16_Min_Battery_Voltage_V;           //Tension minima de baterias (unidad:Volt,min:0,max:4094,resolucion:1V)
    Un_Inverter_Battery_Limits_Flags un_Flags;
} St_Inverter_Limits;

#define INVERTER_LIMITS_INIT(v)                  \
    v.u16_Max_Charge_Current_A = 150;            \
    v.u16_Max_Discharge_Current_A = 350;         \
    v.u16_Max_Battery_Voltage_V = 700;           \
    v.u16_Min_Battery_Voltage_V = 530;           \
    v.un_Flags.all = 0x00;

typedef struct{
    uint8 u8_VmuCommand1_RollCnt;
    uint8 u8_VmuCommand2_RollCnt;
    uint8 u8_VmuCommandSafety_RollCnt;
    uint16 u16_VmuBootNumber;
    uint8 u8_Mayor_Protocol;
    uint8 u8_Minor_Protocol;
    uint8 u8_Mayor_Extension;
    uint8 u8_Minor_Extension;
    uint8 u8_Send_Turn;
}St_Inverter_Communication;

#define INVERTER_COMUNICATION_INIT(v)               \
    v.u8_VmuCommand1_RollCnt = 0;                   \
    v.u8_VmuCommand2_RollCnt = 0;                   \
    v.u8_VmuCommandSafety_RollCnt = 0;              \
    v.u16_VmuBootNumber = 0;                        \
    v.u8_Mayor_Protocol = 0;                        \
    v.u8_Minor_Protocol = 0;                        \
    v.u8_Mayor_Extension = 0;                       \
    v.u8_Minor_Extension = 0;                       \
    v.u8_Send_Turn = 0;

#define INVERTER_SPEED_FILTER_INIT(v)   \
    v.f32_Gain_A0 = 0.01f;              \
    v.f32_Gain_A1 = 0.0f;               \
    v.f64_Gain_B1 = -0.99f;             \
    v.f32_X1 = 0.0f;                    \
    v.f64_Y1 = 0.0f;                    \
    v.f64_GX = 0.0f;                    \
    v.f64_GY = 0.0f;

typedef struct{
    float32 f32_Direct_Torque_Gain_nm;
    float32 f32_Reverse_Torque_Gain_nm;         //Must be a negative number
    float32 f32_Maximun_Foward_Torque_nm;
    float32 f32_Maximun_Reverse_Torque_nm;      //Must be a negative number
    float32 f32_Maximun_Regen_Torque_nm;        //Must be a negative number
    sint16 s16_Max_Foward_Speed_rpm;
    sint16 s16_Max_Revese_Speed_rpm;            //Must be a negative number
    uint16 u16_Inverter_Absolut_Maximun_Discharge_Current_A;
    uint16 u16_Inverter_Absolut_Maximun_Regen_Current_A;
    uint16 u16_Inverter_Absolut_Maximun_Voltage_v;

}St_Inverter_Parameters;

//Peak torque 2300nm
//Continuos Torque 1273nm
#define INVERTER_PARAMETERS_INIT(v)                               \
    v.f32_Direct_Torque_Gain_nm = 2400;                           \
    v.f32_Reverse_Torque_Gain_nm = -2400;                         \
    v.f32_Maximun_Foward_Torque_nm = 2400;                        \
    v.f32_Maximun_Reverse_Torque_nm = -2400;                      \
    v.f32_Maximun_Regen_Torque_nm = -1200;                        \
    v.s16_Max_Foward_Speed_rpm = 2700;                            \
    v.s16_Max_Revese_Speed_rpm = -2700;                           \
    v.u16_Inverter_Absolut_Maximun_Discharge_Current_A = 350;     \
    v.u16_Inverter_Absolut_Maximun_Regen_Current_A = 150;         \
    v.u16_Inverter_Absolut_Maximun_Voltage_v = 700;


typedef struct{
    Un_Inverter_State_Flags un_State;
    St_Inverter_Refs st_Refs;
    St_Inverter_Electrical st_Electrical;
    St_Inverter_Mechanical st_Mechanical;
    St_Inverter_Cooling st_Cooling;
    St_Inverter_Faults st_Fauts;
    St_Inverter_Limits st_Limits;
    St_Inverter_Communication st_Coms;
    St_Filter st_Speed_Filter;
    St_Inverter_Parameters st_Parameters;
}St_Inverter_Variables;

#define INIT_INVERTER(v)                            \
    v.un_State.all = 0;                             \
    INVERTER_REFS_INIT(v.st_Refs)                   \
    INVERTER_ELECTRICAL_INIT(v.st_Electrical)       \
    INVERTER_MECHANICAL_INIT(v.st_Mechanical)       \
    INVERTER_COOLING_INIT(v.st_Cooling)             \
    INVERTER_FAULT_INIT(v.st_Fauts)                 \
    INVERTER_LIMITS_INIT(v.st_Limits)               \
    INVERTER_COMUNICATION_INIT(v.st_Coms)           \
    INVERTER_SPEED_FILTER_INIT(v.st_Speed_Filter)   \
    INVERTER_PARAMETERS_INIT(v.st_Parameters)

extern St_Inverter_Variables st_Inverter;

extern T_x_os_mutex t_Mutex_Inverter;

extern void *mpv_MsgObjTx_Inverter; // module global defined TX object
extern void *mpv_MsgObjRx_Inverter; // module global defined RX object

extern T_x_can_msg t_Inverter_Msg;
extern T_x_os_mutex t_Mutex_Inverter_Msg;

#endif // INTERRUPTS_H_INCLUDED
