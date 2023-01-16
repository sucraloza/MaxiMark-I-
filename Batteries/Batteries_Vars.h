#ifndef BATTERIES_CONTROL_VARS_H_INCLUDED
#define BATTERIES_CONTROL_VARS_H_INCLUDED

//Batteries References bits
typedef struct{
	uint64 b8_Life_Signal:8;		    //rolling counter
	uint64 b2_HV_On_Off:2;			    //HV Power On & HV Power Off Command
	uint64 b2_Aux_1_On_Off:2;		    //Aux 1 Power On & Power Off Command (if Apply)
	uint64 b2_Aux_2_On_Off:2;		    //Aux 2 Power On & Power Off Command (if Apply)
	uint64 b2_Aux_3_On_Off:2;		    //Aux 3 Power On & Power Off Command (if Apply)
	uint64 b8_VehicleSpeed_kmh:8;	    //Vehicle speed in km/h
	uint64 b2_Main_Positive_Status:2;   //Status Contactor: 0:rsv, 1:Open, 2:Close, 3:Invalid
	uint64 b2_Main_Positive_Fault:2;    //Fault Contactor: 0:Normal, 1:Weld, 2:Open Fault, 3:Invalid
	uint64 b2_Main_Negitive_Status:2;
	uint64 b2_Main_Negitive_Fault:2;
	uint64 b2_Chrg_Positive_Status:2;
	uint64 b2_Chrg_Positive_Fault:2;
	uint64 b2_Heat_Positive_Status:2;
	uint64 b2_Heat_Positive_Fault:2;
	uint64 b2_Heat_Negitive_Status:2;
	uint64 b2_Heat_Negitive_Fault:2;
	uint64 b2_Aux_1_Status:2;
	uint64 b2_Aux_1_Fault:2;
	uint64 b2_Aux_2_Status:2;
	uint64 b2_Aux_2_Fault:2;
	uint64 b2_Aux_3_Status:2;
	uint64 b2_Aux_3_Fault:2;
	uint64 b2_Aux_4_Status:2;
	uint64 b2_Aux_4_Fault:2;
	uint64 b4_rsv:4;
} St_Battery_Command;

//Batteries Command Variables
typedef union{
	uint64 all;
	St_Battery_Command bits;
} Un_Battery_Command;

#define INIT_BATTERY_COMMAND(v)         \
    v.all = 0;

typedef struct{
	uint8 b4_Work_Status:4; //Fult Level 0:Selfcheck 1:Working 2:Stop_Working 3:Equipment_Failure
	uint8 b4_Fault_Level:4; //Fult Level 0:Normal 1:Light 2:Severe 3:Most_Severe
} St_Battery_Isometer_Infomation_Flags;

typedef union{
	uint8 all;
	St_Battery_Isometer_Infomation_Flags bits;
} Un_Battery_Isometer_Infomation_Flags;

typedef struct{
	Un_Battery_Isometer_Infomation_Flags un_Flags;
	uint16 u16_Positive_Resistance;
	uint16 u16_Negitive_Resistance;
} St_Battery_Isometer_Infomation;

#define INIT_BATTERY_ISOMETER(v)         \
    v.un_Flags.all = 0;                  \
    v.u16_Positive_Resistance = 0;       \
    v.u16_Negitive_Resistance = 0;

typedef struct{
	uint16 u16_Avail_Inpulse_Discharge_Current_A;
	uint16 u16_Avail_Inpulse_Charge_Current_A;
	uint16 u16_Avail_Sustain_Charge_Current_A;
} St_Battery_Current_Limitations;

#define INIT_BATTERY_CURRERNT_LIMITATION(v)             \
    v.u16_Avail_Inpulse_Discharge_Current_A = 0;        \
    v.u16_Avail_Inpulse_Charge_Current_A = 0;           \
    v.u16_Avail_Sustain_Charge_Current_A =0;


typedef struct{
	uint64 b2_Main_Positive_Relay_Stat:2;                       //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Main_Negitive_Relay_Status:2;                     //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Precharge_Relay_Status:2;                         //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_DC_Charge_Positive_1_Relay_Status:2;              //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_DC_Charge_Negitive_1_Relay_Status:2;              //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_DC_Charge_Positive_2_Relay_Status:2;              //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_DC_Charge_Negitive_2_Relay_Status:2;              //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Heat_Positive_Relay_Status:2;                     //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Heat_Negitive_Relay_Status:2;                     //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Pantograph_Charge_Positive_2_Relay_Status:2;      //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Pantograph_Charge_Negitive_2_Relay_Status:2;      //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_AC_Charge_Positive_2_Relay_Status:2;              //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_AC_Charge_Negitive_2_Relay_Status:2;              //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Aux_1_Relay_Status:2;                             //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Aux_2_Relay_Status:2;                             //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_Aux_3_Relay_Status:2;                             //0:rsv 1:Opened 2:Closed 3:Invalid
	uint64 b2_BMS_HV_Status:2;                                  //0:HV Open 1:Precharge or Precheck 2:HV Closed 3:Fail to HV On
	uint64 b1_BMS_Balance_Status:1;                             //0:NO Balancing 1:Balancing
	uint64 b2_DC_Charge_Connected_Status:2;                     //0:Not Connected 1: Single Gun Connected 2: Double Gun Connected 3: Invalid
	uint64 b1_Pantograph_Charge_Connected_Status:1;             //0:Not Connected 1:Connected
	uint64 b1_AC_Charge_Connected_Status:1;                     //0:Not Connected 1:Connected
	uint64 b2_Charge_Mode:2;                                    //0:Not Charging 1:DC Charging 2:AC Charging 3:Other Charging
	uint64 b2_Charge_Status:2;                                  //0:Not Charging 1:Charging 2:Charging Finish 3:Charging Error
	uint64 b1_Heating_Status:1;                                 //0:Not Heating 1:Heating
	uint64 b2_Cooling_Status:2;                                 //0:Not Cooling 1:Cooling
	uint64 b18_Recharge_Cycles:18;                              //BMS Charge Cycles
} St_Battery_State_1;

typedef union{
    uint64 all;
    St_Battery_State_1 bits;
} Un_Battery_State_1;

#define INIT_BATTERY_STATE_1(v)                         \
    v.all = 0;

typedef struct{
    uint8 b2_Fault_Level:2;                     //0:Normal 1:Light_Fault 2:Severe_Fault 3:Most_Severe_Fault
    uint8 b2_Request_HV_Power_Off:2;            //0:Normal 1:Light_Fault 2:Severe_Fault 3:Most_Severe_Fault
    uint8 b4_Life:4;
} St_Battery_State_2_Flags;

typedef union{
    uint8 all;
    St_Battery_State_2_Flags bits;
} Un_Battery_State_2_Flags;

typedef struct{
    uint8 u8_SOC;
    uint8 u8_SOH;
    sint16 s16_Pack_Current_A;
    uint16 u16_Pack_Inside_Voltage_V;
    uint8 u8_Fault_Code;
    Un_Battery_State_2_Flags un_Flags;
} St_Battery_State_2;

#define INIT_BATTERY_STATE_2(v)                         \
    v.u8_SOC = 0;                                       \
    v.u8_SOH = 0;                                       \
    v.s16_Pack_Current_A = 0;                           \
    v.u16_Pack_Inside_Voltage_V = 0;                    \
    v.u8_Fault_Code = 0;                                \
    v.un_Flags.all = 0;

typedef struct{
    uint16 u16_Positive_Insulation_Resistance_kohm;     //0xFFFF: invalid
    uint16 u16_Negative_Insulation_Resistance_kohm;     //0xFFFF: invalid
    uint16 u16_System_Insulation_Resistance_kohm;       //0xFFFF: invalid
    uint8  u8_Insolation_Monitor_Status;                //0:Insulation detection Stoped 1:Insulation Detection Active
} St_Battery_State_3;

#define INIT_BATTERY_STATE_3(v)                        \
    v.u16_Positive_Insulation_Resistance_kohm = 0;     \
    v.u16_Negative_Insulation_Resistance_kohm = 0;     \
    v.u16_System_Insulation_Resistance_kohm = 0;       \
    v.u8_Insolation_Monitor_Status = 0;

typedef struct{
    sint8 s8_Max_Temperature_C;                        //Temperature in Celsius Offset: -40 0xFE: Abnormal 0xFF: Invalid
    sint8 s8_Min_Temperature_C;                        //Temperature in Celsius Offset: -40 0xFE: Abnormal 0xFF: Invalid
    sint8 s8_Avg_Temperature_C;                        //Temperature in Celsius Offset: -40 0xFE: Abnormal 0xFF: Invalid
    uint8  u8_Max_Temperature_Cell_Number;
    uint8  u8_Max_Temperature_Position;
    uint8  u8_Min_Temperature_Cell_Number;
    uint8  u8_Min_Temperature_Position;
} St_Battery_State_4;

#define INIT_BATTERY_STATE_4(v)                       \
    v.s8_Max_Temperature_C = 0;                       \
    v.s8_Min_Temperature_C = 0;                       \
    v.s8_Avg_Temperature_C = 0;                       \
    v.u8_Max_Temperature_Cell_Number = 0;             \
    v.u8_Max_Temperature_Position = 0;                \
    v.u8_Min_Temperature_Cell_Number = 0;             \
    v.u8_Min_Temperature_Position = 0;


typedef struct{
    float32 f32_Max_Cell_Voltage_V;
    float32 f32_Min_Cell_Voltage_V;
    float32 f32_Avg_Cell_Voltage_V;
} St_Battery_State_5;

#define INIT_BATTERY_STATE_5(v)                      \
    v.f32_Max_Cell_Voltage_V = 0;                    \
    v.f32_Min_Cell_Voltage_V = 0;                    \
    v.f32_Avg_Cell_Voltage_V = 0;

typedef struct{
    uint8 u8_Max_Voltage_Cell_Number;
    uint16 u16_Max_Voltage_Cell_Position;
    uint8 u8_Min_Voltage_Cell_Number;
    uint16 u16_Min_Voltage_Cell_Position;
} St_Battery_State_6;


#define INIT_BATTERY_STATE_6(v)                 \
    v.u8_Max_Voltage_Cell_Number = 0;           \
    v.u16_Max_Voltage_Cell_Position = 0;        \
    v.u8_Min_Voltage_Cell_Number = 0;           \
    v.u16_Min_Voltage_Cell_Position = 0;

typedef struct{
    sint8 s8_Gun_1_DC_Positive_Temperature_C;       //Temperature in Celsius Offset: -40 0xFF: Invalid
    sint8 s8_Gun_1_DC_Negitive_Temperature_C;       //Temperature in Celsius Offset: -40 0xFF: Invalid
    sint8 s8_Gun_2_DC_Positive_Temperature_C;       //Temperature in Celsius Offset: -40 0xFF: Invalid
    sint8 s8_Gun_2_DC_Negitive_Temperature_C;       //Temperature in Celsius Offset: -40 0xFF: Invalid
    sint8 s8_Gun_AC_Positive_Temperature_C;         //Temperature in Celsius Offset: -40 0xFF: Invalid
    sint8 s8_Gun_AC_Negitive_Temperature_C;         //Temperature in Celsius Offset: -40 0xFF: Invalid
} St_Battery_State_7;

#define INIT_BATTERY_STATE_7(v)                     \
    v.s8_Gun_1_DC_Positive_Temperature_C = 0;       \
    v.s8_Gun_1_DC_Negitive_Temperature_C = 0;       \
    v.s8_Gun_2_DC_Positive_Temperature_C = 0;       \
    v.s8_Gun_2_DC_Negitive_Temperature_C = 0;       \
    v.s8_Gun_AC_Positive_Temperature_C = 0;         \
    v.s8_Gun_AC_Negitive_Temperature_C = 0;



typedef struct{
    uint32 b2_Fault_Number_32960:2;
    uint32 b2_Temperature_Diference:2;              //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Over_Temperature:2;                   //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Pack_Over_Voltage:2;                  //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Pack_Low_Voltage:2;                   //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Pack_Low_SOC_:2;                      //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Cell_Over_Voltage:2;                  //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Cell_Low_Voltage:2;                   //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Low_Isolation_Resistance:2;           //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Cell_Voltage_Uniformity:2;            //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Over_Charge:2;                        //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b2_Over_SOC:2;                           //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint32 b1_SOC_Change_to_Fast:1;                 //0:normal 1:Fault
    uint32 b1_Baterry_System_Not_Mach:1;            //0:normal 1:Fault
    uint32 b1_HVIL_Alarm:1;                         //0:normal 1:Fault
    uint32 rsv:5;
} St_Battery_Faults_1;

typedef union{
    uint32 all;
    St_Battery_Faults_1 bits;
} Un_Battery_Faults_1;

#define INIT_BATTERY_FAULT_1(v)             \
    v.all = 0;

typedef struct{
    uint64 b8_Fault_Nuber:8;
    uint64 b1_Cell_Iver_Discharge:1;                        //0:normal 1:Fault
    uint64 b1_Cell_Over_Temperature:1;                      //0:normal 1:Fault
    uint64 b1_Pack_Fire_Warning:1;                          //0:normal 1:Fault
    uint64 b2_Over_Discharge_Current:2;                     //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint64 b2_Over_Charge_Current_in_Drive:2;               //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint64 b1_Current_Sensor_Error:1;                       //0:normal 1:Fault
    uint64 b1_Power_Supply_Error:1;                         //0:normal 1:Fault
    uint64 b1_BTMS_Error:1;                                 //0:normal 1:Fault
    uint64 b1_Heating_Film_Error:1;                         //0:normal 1:Fault
    uint64 b1_Pack_Self_Protect:1;                          //0:normal 1:Fault
    uint64 b1_Main_Loop_Precharge_Error:1;                  //0:normal 1:Fault
    uint64 b1_Aux_Loop_Precharge_Error:1;                   //0:normal 1:Fault
    uint64 b1_Low_Insulation_When_Charging:1;               //0:normal 1:Fault
    uint64 b1_Aux_CAN_Lost:1;                               //0:normal 1:Fault
    uint64 b1_Inter_Comunication_Error:1;                   //0:normal 1:Fault
    uint64 b1_DCDC_Error:1;                                 //0:normal 1:Fault
    uint64 b1_Branch_Break_Error:1;                         //0:normal 1:Fault
    uint64 b1_Heat_Relay_Open:1;                            //0:normal 1:Fault
    uint64 b1_Heat_Relay_Weld:1;                            //0:normal 1:Fault
    uint64 b1_Main_Positive_Relay_Open:1;                   //0:normal 1:Fault
    uint64 b1_Main_Positive_Relay_Weld:1;                   //0:normal 1:Fault
    uint64 b1_Main_Negitive_Relay_Open:1;                   //0:normal 1:Fault
    uint64 b1_Main_Negitive_Relay_Weld:1;                   //0:normal 1:Fault
    uint64 b1_DC_Charging_Positive_1_Relay_Open:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Positive_1_Relay_Weld:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Negitive_1_Relay_Open:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Negitive_1_Relay_Weld:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Positive_2_Relay_Open:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Positive_2_Relay_Weld:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Negitive_2_Relay_Open:1;          //0:normal 1:Fault
    uint64 b1_DC_Charging_Negitive_2_Relay_Weld:1;          //0:normal 1:Fault
    uint64 b1_AC_Charging_Positive_Relay_Open:1;            //0:normal 1:Fault
    uint64 b1_AC_Charging_Positive_Relay_Weld:1;            //0:normal 1:Fault
    uint64 b1_AC_Charging_Negitive_Relay_Open:1;            //0:normal 1:Fault
    uint64 b1_AC_Charging_Negitive_Relay_Weld:1;            //0:normal 1:Fault
    uint64 b1_Pantograph_Charging_Positive_Relay_Open:1;    //0:normal 1:Fault
    uint64 b1_Pantograph_Charging_Positive_Relay_Weld:1;    //0:normal 1:Fault
    uint64 b1_Pantograph_Charging_Negitive_Relay_Open:1;    //0:normal 1:Fault
    uint64 b1_Pantograph_Charging_Negitive_Relay_Weld:1;    //0:normal 1:Fault
    uint64 b1_Charge_Connection_Error:1;                    //0:normal 1:Fault
    uint64 b1_Over_Discharge_Current_While_Charging:1;      //0:normal 1:Fault
    uint64 b1_Over_Charge_Current_While_Charging:1;         //0:normal 1:Fault
    uint64 b1_Charge_NTC_Error:1;                           //0:normal 1:Fault
    uint64 b2_Charge_NTC_Temp_Over:2;                       //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint64 b1_Electrical_Lock_Error:1;                      //0:normal 1:Fault
    uint64 b1_Fire_Detection:1;                             //0:normal 1:Fault
    uint64 b2_Continuos_Dicharge_Current_Over_Load:2;       //0:normal 1:Light_Fault 2:Severe_Fault 3::Most_Severe_Fault
    uint64 b1_SOC_Diference_Error:1;                        //0:normal 1:Fault
    uint64 rsv:4;
} St_Battery_Faults_2;

typedef union{
    uint64 all;
    St_Battery_Faults_2 bits;
} Un_Battery_Faults_2;

#define INIT_BATTERY_FAULT_2(v)             \
    v.all = 0;

typedef struct{
    uint8 u8_CAN_Timeout_Counter;
    uint8 u8_CAN_Timeout_Flag;
} St_Battery_CAN;

typedef struct{
	Un_Battery_Command un_Command;
	St_Battery_Isometer_Infomation st_Isometer;
    St_Battery_Current_Limitations st_Current_Limits;
    Un_Battery_State_1 un_State_1;
    St_Battery_State_2 st_State_2;
    St_Battery_State_3 st_State_3;
    St_Battery_State_4 st_State_4;
    St_Battery_State_5 st_State_5;
    St_Battery_State_6 st_State_6;
    St_Battery_State_7 st_State_7;
    Un_Battery_Faults_1 un_Faults_1;
    Un_Battery_Faults_2 un_Faults_2;
    St_Battery_CAN st_CAN;
} St_Battery_Vars;

#define INIT_BATTERY_VARS(v)                                    \
    INIT_BATTERY_COMMAND(v.un_Command)                          \
    INIT_BATTERY_ISOMETER(v.st_Isometer)                        \
    INIT_BATTERY_CURRERNT_LIMITATION(v.st_Current_Limits)       \
    INIT_BATTERY_STATE_1(v.un_State_1)                          \
    INIT_BATTERY_STATE_2(v.st_State_2)                          \
    INIT_BATTERY_STATE_3(v.st_State_3)                          \
    INIT_BATTERY_STATE_4(v.st_State_4)                          \
    INIT_BATTERY_STATE_5(v.st_State_5)                          \
    INIT_BATTERY_STATE_6(v.st_State_6)                          \
    INIT_BATTERY_STATE_7(v.st_State_7)                          \
    INIT_BATTERY_FAULT_1(v.un_Faults_1)                         \
    INIT_BATTERY_FAULT_2(v.un_Faults_2)

typedef struct{
    uint8 u8_Fast_Counter;
    uint8 u8_Normal_Counter;
    uint8 u8_Slow_Counter;
} St_Battery_Counters;

#define INIT_BATTERY_CONTERS(v)         \
    v.u8_Fast_Counter = 0;              \
    v.u8_Normal_Counter = 0;            \
    v.u8_Slow_Counter = 0;


extern St_Battery_Vars st_Battery;
extern T_x_os_mutex t_Mutex_Battery;

extern St_Battery_Counters st_Battery_Counters;

extern void *mpv_MsgObjTx_Battery; // module global defined TX object for Batteries
extern void *mpv_MsgObjRx_Battery; // module global defined RX object for Batteries

extern T_x_can_msg t_Battery_Msg;          //Message Holder RX object
extern T_x_os_mutex t_Mutex_Battery_Msg;


#endif // INTERRUPTS_H_INCLUDED
