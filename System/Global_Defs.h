#ifndef GLOBAL_DEFS_H_INCLUDED
#define GLOBAL_DEFS_H_INCLUDED

#define WHEEL_RADIUS        383.5          // Wheel Radius 383.5 in mm
#define GEARBOX_REDUCTION   4.3            // Reduction 4.3
//#define SPEED_FACTOR        0.0876f        // Speed [KM/H] =  0.0876 * RPM
#define SPEED_FACTOR        0.03357f        // Speed [KM/H] =  0.03357 * RPM

//ECU_STATE
#define ECU_STATE_SLEEP               0x00
#define ECU_STATE_POWER_ON            0x03
#define ECU_STATE_AUXILIARIES         0x05
#define ECU_STATE_POWER_OFF           0x06
#define ECU_STATE_DRIVE               0x09
#define ECU_STATE_CHARGE              0x0A
#define ECU_STATE_FAULT               0x0C
#define ECU_STATE_GPIO_CHECK          0x0F


//POWER_ON_STATE
#define POWER_ON_STATE_CHECK_CIRCUITS      0x00
#define POWER_ON_STATE_POWER_ON_T15        0x03
#define POWER_ON_STATE_BUTTONS             0x05
#define POWER_ON_STATE_BATTERY_CAN         0x06
#define POWER_ON_STATE_PROTOCOL_INVERTER   0x09
#define POWER_ON_STATE_BATTERY_READY       0x0A
#define POWER_ON_STATE_CIRCUIT_CLOSE       0x0C
#define POWER_ON_STATE_FAULT               0x0F

//POWER_OFF_STATE
#define POWER_OFF_STATE_CHECK_INV_STANDBY     0x00
#define POWER_OFF_STATE_DISABLE_SYSTEMS       0x03
#define POWER_OFF_STATE_CIRCUIT_OPEN          0x05
#define POWER_OFF_STATE_BATTERY_OPEN          0x06
#define POWER_OFF_STATE_WAIT                  0x09
#define POWER_OFF_STATE_POWER_OFF_T15         0x0A
#define POWER_OFF_STATE_WAIT_BATTERY          0x0C
#define POWER_OFF_STATE_FAULT                 0x0F

//VEHICLE_ON_STATE
#define VEHICLE_ON_STATE_CHECK_CHARGING        0x00
#define VEHICLE_ON_STATE_CHECK_AIRPUMP         0x03
#define VEHICLE_ON_STATE_CHECK_THROTTLE        0x05
#define VEHICLE_ON_STATE_CHECK_BREAK           0x06
#define VEHICLE_ON_STATE_INV_OPERATIONAL       0x09


//VEHICLE_OFF_STATE
#define VEHICLE_OFF_STATE_INVERTER_NEUTRAL     0x00
#define VEHICLE_OFF_STATE_INVERTER_STANDBY     0x03

//Battery Chage Limit SOC
#define MAX_CHARGE_SOC_LIMIT     101     //90%

//SECURE PARAMETERS
#define SYSTEM_SECURE_SPEED_RPM         900
#define SYSTEM_SECURE_THROTTLE_PU       0.01f

//DRIVERS FEEDBACK
#define SYSTEMS_FEEDBACK_SLEEP               0x00
#define SYSTEMS_FEEDBACK_CONTACT             0x01
#define SYSTEMS_FEEDBACK_WAITING             0x02
#define SYSTEMS_FEEDBACK_AIR                 0x03
#define SYSTEMS_FEEDBACK_READY_TO_GO         0x04
#define SYSTEMS_FEEDBACK_CHARGING            0x05
#define SYSTEMS_FEEDBACK_POWER_OFF           0x06
#define SYSTEMS_FEEDBACK_CHECK_CONTROLS      0x07

//HVIL
#define SYSTEM_HVIL_INACTIVE    0x00u
#define SYSTEM_HVIL_ACTIVE      0x03u

//INCLUDES
#include "System/Global_Vars.h"                 //Global System Variables
#include "System/Global_Controls.h"             //Global System Controls
#include "System/State_Machiene.h"
#include "Interrupts/Interrupts.h"              //Interrupt functions
#include "Batteries/Batteries_Defs.h"           //Battery Definitions
#include "Batteries/Batteries_Vars.h"           //Battery Variables
#include "Batteries/Batteries_CAN.h"            //Battery CAN Communication
#include "Batteries/Batteries_Control.h"        //Battery Control System
#include "Batteries/Batteries_Monitoring.h"     //Battery Monitoring System
#include "BTMS/btms.h"
#include "Monitoring/Monitoring_CAN.h"          //CAN communication with PC
#include "Monitoring/Monitoring_Vars.h"         //CAN communication with PC Control
#include "Jarvis/Jarvis_Defs.h"                 //Auxiliary ECU Definitions
#include "Jarvis/Jarvis_Vars.h"                 //Auxiliary ECU Variables
#include "Jarvis/Jarvis_CAN.h"                  //CAN communication with Auxiliary ECU
#include "Jarvis/Jarvis_Functions.h"            //Access Functions of Auxiliary ECU
#include "PDU/PDU_Defs.h"                       //PDU Defines
#include "PDU/PDU_Vars.h"                       //PDU Variables
#include "PDU/PDU_Controls.h"                   //PDU Control Functions
#include "PDU/PDU_Isolation.h"                  //PDU Control Functions
#include "Cooling/Cooling_Pump_Vars.h"
#include "Cooling/Cooling_Pump_Controls.h"
#include "Cooling/Cooling_Pump_CAN.h"
#include "Isolation_Monitor/Isolation_Monitor_Defs.h"
#include "Isolation_Monitor/Isolation_Monitor_Vars.h"
#include "Isolation_Monitor/Isolation_Monitor_CAN.h"
#include "Isolation_Monitor/Isolation_Monitor_Controls.h"
#include "System/Auxiliary_Functions.h"
#include "System/Power_On.h"
#include "Power_Train/Power_Train_Defs.h"
#include "Power_Train/Power_Train_Vars.h"
#include "Power_Train/Power_Train_CAN.h"
#include "Power_Train/Power_Train_Control.h"
#include "Power_Train/Power_Train_Monitoring.h"
#include "Power_Train/Odometer/odometer.h"
#include "Throttle/Throttle_Vars.h"
#include "Throttle/Throttle_Control.h"
#include "Panel/Panel_Vars.h"
#include "Panel/Panel_CAN.h"
#include "Panel/Panel_Controls.h"
#include "PTC/PTC.h"
#include "Fusi/Fusi_Defs.h"
#include "Fusi/Fusi_Vars.h"
#include "Fusi/Fusi_Controls.h"
#include "Fusi/Fusi_CAN.h"
#include "Tachograph/Tachograph_Vars.h"
#include "Tachograph/Tachograph_CAN.h"
#include "Tachograph/Tachograph_Controls.h"
#include "Telemetry/Telemetry_Defs.h"
#include "Telemetry/Telemetry_Vars.h"
#include "Telemetry/Telemetry_Controls.h"
#include "Telemetry/Telemetry_CAN.h"
#include "GPIO/GPIO_Controls.h"
#include "SCU/SCU_Controls.h"
#include "SCU/TPDU/tpdu.h"
#include "Key/Key.h"
#include "Gear/Gear.h"
#include "HVIL/HVIL.h"
#include "Buttons/Buttons_Int.h"
#include "Air/Air.h"
#include "Air/Compressor.h"
#include "Air/AC_Compressor.h"
#include "Air/Service_Air.h"
#include "Air/AC_Config.h"
#include "Steering/Steering.h"
#include "Timeout/Timeout.h"
#include "Vision/Vision.h"
#include "Buzzer/buzzer.h"
#include "Monitoring/CAPL.h"
#include "EEPROM/EEPROM.h"




#endif /* GLOBAL_DEFS_H_INCLUDED */
