#ifndef GPIO_DEFS_H_INCLUDED
#define GPIO_DEFS_H_INCLUDED

//KS1 CONNECTOR
#define GPIO_PDU_PRECHARGE      X_OUT_01    // PIN 32   //PDU Precharge Output
#define GPIO_PDU_DISCHARGE      X_OUT_02    // PIN 33   //PDU Discharge Output
#define GPIO_PDU_BTMS           X_OUT_03    // PIN 51   //PDU Steering Contactor Output (Not in use)
#define GPIO_PDU_AC             X_OUT_05    // PIN 13   //PDU AC Contactor Output (Not in use)
#define GPIO_PDU_INVERTER       X_OUT_06    // PIN 14   //PDU Main Contactor Output

#define GPIO_HVIL_OUT           X_OUT_07    // PIN 71   //HVIL Output
// #define GPIO_STEERING_CONT      X_OUT_08    // PIN 72   //Steering Contactor Output  // Redefined on KS2
#define GPIO_DAPIG              X_OUT_04    // PIN 52   //Dapig Override Output (Not in use)

#define GPIO_HVIL_IN            X_IN_01     // PIN 70   //HVIL Input
#define GPIO_BATTERY_VOLTAGE    X_IN_02     // PIN 12   //24V Voltage messurement
#define GPIO_THROTTLE_A         X_IN_09     // PIN 68   //THROTLE_A Input
#define GPIO_THROTTLE_B         X_IN_05     // PIN 69   //THROTLE_B Input
// #define GPIO_AC_FRONT           X_IN_08     // PIN 49   //AC Controler Input (Front)
#define GPIO_AC_REAR            X_IN_04     // PIN 50   //AC Compresor engage Input (Rear)
#define GPIO_IN_TEMP_COMP       X_IN_13     // PIN 67   //Comp Temperature PTC100 Input

//KS2 CONNECTOR
#define GPIO_T15_BCU            X_OUT_10    // PIN 30   //Ign BCU Output
#define GPIO_T15_INV            X_OUT_13    // PIN 50   //Ign Inverter Output
#define GPIO_T15_SCREEN         X_OUT_14    // PIN 31   //Ign RBN Panel
#define GPIO_T15_DIGI           X_OUT_18    // PIN 32   //Ign fuente 12V LVDU (Tiene la logica negada)
#define GPIO_T15_STEERING       X_OUT_21    // PIN 52   //Ign Steering Control Output
#define GPIO_T15_BTMS           X_OUT_22    // PIN 33   //Ign BTMS Output
#define GPIO_T15_BUZZER         X_OUT_17    // PIN 51   //Ign Buzzer Output

#define GPIO_TPDU_AC            X_OUT_23    // PIN 14   //TPDU Air Conditioning Output
#define GPIO_TPDU_COMP          X_OUT_24    // PIN 15   //TPDU Air Compresor Output
#define GPIO_BUZZER_SPEED       X_OUT_19    // PIN 13   //Speed Signal for Buzzer (Freq Mod)
#define GPIO_OUT_CTANK          X_OUT_12    // PIN 69   //Coolling Tank Supply
// #define GPIO_OUT_HTANK          X_OUT_16    // PIN 70   //Heating Tank Supply
#define GPIO_STEERING_CONT      X_OUT_16    // PIN 70   //Steering Contactor Output
// #define GPIO_OUT_STANK          X_OUT_20    // PIN 71   //Steering Tank Supply


#define GPIO_EMERGENCY          X_IN_17     // PIN 46   //Emergency Input
#define GPIO_INERTIA            X_IN_22     // PIN 28   //Inertia Input
#define GPIO_KEY_ACCESSORIES    X_IN_21     // PIN 47   //Accessories Input
#define GPIO_KEY_START          X_IN_25     // PIN 48   //Start Input
#define GPIO_CHARGE_WAKEUP      X_IN_26     // PIN 29   //Wakeup charge Input
#define GPIO_IN_CTANK           X_IN_20     // PIN 66   //Coolling Tank Input
#define GPIO_IN_HTANK           X_IN_24     // PIN 67   //Heating Tank Input
// #define GPIO_IN_STANK           X_IN_28     // PIN 68   //Steering Tank Input

//Sensor Supply
#define GPIO_SUPPLY_COMP                X_SYS_SENSOR_SUPPLY_01  // PIN 66    //Comp Temperature Supplay
#define GPIO_SUPPLY_COMP_VOLTAGE_mV     3000                    // PIN 66    //Comp Temperature Supplay

#endif /* GPIO_DEFS_H_INCLUDED */
