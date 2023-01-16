#include "OS_Defs.h"

void Telemetry_Track_ECU_State(uint8 u8_New){
    if(st_Telemetry.st_Tracking.u8_ECU_State != u8_New){
        st_Telemetry.st_Tracking.u8_ECU_State = u8_New;
        Telemetry_System_Status();
    }
}
void Telemetry_Track_Key_State(uint8 u8_New){
    if(st_Telemetry.st_Tracking.u8_Key_State != u8_New){
        st_Telemetry.st_Tracking.u8_Key_State = u8_New;
        Telemetry_System_Status();
    }
}
void Telemetry_Track_Battery_Heating(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b1_Battery_Heating != (u8_New & 0x01)){
        st_Telemetry.st_Tracking.un_Flags.bits.b1_Battery_Heating = (u8_New & 0x01);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}
void Telemetry_Track_BTMS(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b1_BTMS_State != (u8_New & 0x01)){
        st_Telemetry.st_Tracking.un_Flags.bits.b1_BTMS_State = (u8_New & 0x01);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}
void Telemetry_Track_Gun_Connection(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b1_Gun_Connection != (u8_New & 0x01)){
        st_Telemetry.st_Tracking.un_Flags.bits.b1_Gun_Connection = (u8_New & 0x01);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}
void Telemetry_Track_BMS_HV_State(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b2_BMS_HV_State != (u8_New & 0x03)){
        st_Telemetry.st_Tracking.un_Flags.bits.b2_BMS_HV_State = (u8_New & 0x03);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}

void Telemetry_Track_Compresor_Use(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b1_Compressor_Use != (u8_New & 0x01)){
        st_Telemetry.st_Tracking.un_Flags.bits.b1_Compressor_Use = (u8_New & 0x01);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}
void Telemetry_Track_PTC1(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b1_PTC1_State != (u8_New & 0x01)){
        st_Telemetry.st_Tracking.un_Flags.bits.b1_PTC1_State = (u8_New & 0x01);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}
void Telemetry_Track_PTC2(uint8 u8_New){
    if(st_Telemetry.st_Tracking.un_Flags.bits.b1_PTC2_State != (u8_New & 0x01)){
        st_Telemetry.st_Tracking.un_Flags.bits.b1_PTC2_State = (u8_New & 0x01);
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}

void Telemetry_Track_HVIL_Reference(uint8 u8_New){
    if(st_Telemetry.st_Tracking.u8_HVIL_Reference_State != u8_New){
        st_Telemetry.st_Tracking.u8_HVIL_Reference_State = u8_New;
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}

void Telemetry_Track_HVIL_Feedback(uint8 u8_New){
    if(st_Telemetry.st_Tracking.u8_HVIL_Feedback_State != u8_New){
        st_Telemetry.st_Tracking.u8_HVIL_Feedback_State = u8_New;
        st_Telemetry.u8_Tracking_Triggered = TRUE;
    }
}

void Tracking_Check(void){


}
