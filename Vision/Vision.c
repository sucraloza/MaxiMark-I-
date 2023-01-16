#include "OS_Defs.h"
#include "Vision.h"

T_Vision_Vars t_Vision;
T_x_os_mutex t_Mutex_Vision;

void *mpv_MsgObjTx_Vision;
void *mpv_MsgObjRx_Vision;

void Vision_Init(void){
    sint16 s16_Error;
    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    t_CanObjInit.t_Id.u32_Id = 0x00000988u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjTx_Vision);
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    t_CanObjInit.t_Id.u32_Id = 0x00008809;               // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 29bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjRx_Vision);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error = x_os_mutex_init(&t_Mutex_Vision);
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_Vision.u8_Critical_Counter = 0;
    t_Vision.u8_Fast_Counter = 0;
    t_Vision.u8_Normal_Counter = 0;
    t_Vision.u8_Slow_Counter = 0;
    t_Vision.u8_Gear = 0;
    t_Vision.u8_Air_Charging = 0;
    t_Vision.u8_Waiting = 0;
    t_Vision.u8_Pressure_dBar = 0;
    t_Vision.u8_SOC = 0;
    t_Vision.u8_DTC_Level = 0;
    t_Vision.u8_Rolling_Counter = 255;
    t_Vision.u8_Vehicle_State = 0;
    t_Vision.u8_Change_Page = 0;
    t_Vision.s8_Engine_Temperature_C = 0;
    t_Vision.s8_Batteries_Temperature_C = 0;
    t_Vision.u16_Isolation_kohm = 0;
    t_Vision.u16_DTC = 0;
    t_Vision.u32_Odometer_100m = 0;
    t_Vision.u32_Timeout = 0;
    t_Vision.f32_Voltage_DC_V = 0;
    t_Vision.u_Indicators.all = 0;
    t_Vision.t_Version.en_Jarvis_VersionStatus = RESERVED;
    t_Vision.t_Version.en_Mark_VersionStatus = MARK_VERSION_STATUS;
    t_Vision.t_Version.u8_Vision_Status = 0;
    t_Vision.t_Version.u8_Jarvis_VersionMajor = 0;
    t_Vision.t_Version.u8_Jarvis_VersionMinor = 0;
    t_Vision.t_Version.u8_Jarvis_VersionPatch = 0;
    t_Vision.t_Version.u8_Mark_VersionMajor = MARK_VERSION_MAJOR;
    t_Vision.t_Version.u8_Mark_VersionMinor = MARK_VERSION_MINOR;
    t_Vision.t_Version.u8_Mark_VersionPatch = MARK_VERSION_PATCH;

}


void Vision_Set_Gear(uint8 u8_Gear){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_Gear = u8_Gear;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Air_Charging(uint8 u8_Air){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_Air_Charging = u8_Air & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Waiting(uint8 u8_Wait){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_Waiting = u8_Wait & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Air_Pressure_mBar(uint16 u16_Pressure_mBar){
    uint8 u8_Pressure = u16_Pressure_mBar / 100;
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_Pressure_dBar = u8_Pressure;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_SOC(uint8 u8_SOC){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_SOC = u8_SOC;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Fusi_DTC_Level(uint8 u8_Level){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_DTC_Level = u8_Level;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Engine_Temperature_C(sint8 s8_Temperature){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.s8_Engine_Temperature_C = s8_Temperature;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Batteries_Temperature_C(sint8 s8_Temperature){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.s8_Batteries_Temperature_C = s8_Temperature;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Isolation(uint16 u16_Isolation_kohm){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u16_Isolation_kohm = u16_Isolation_kohm;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Fusi_DTC(uint16 u16_DTC){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u16_DTC = u16_DTC;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Odometer_km(float32 f32_km){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u32_Odometer_100m = f32_km * 10.0f;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Voltage_DC_V(float32 f32_DCVoltage_V){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.f32_Voltage_DC_V = f32_DCVoltage_V;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Vehicle_State(uint8 u8_Vehicle_State){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_Vehicle_State = u8_Vehicle_State;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}

void Vision_Set_Change_Page(uint8 u8_Change_Page){
    if(!u8_Change_Page) return;

    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u8_Change_Page = u8_Change_Page;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}


void Vision_Set_Is_Charging(uint8 u8_Is_Charging){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Battery_Charging = u8_Is_Charging & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Check_Engine(uint8 u8_Check_Engine){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Check_Engine = u8_Check_Engine & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Emergency_Button(uint8 u8_Emergency_Button){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Emergency_Button = u8_Emergency_Button & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Engine_Limitation(uint8 u8_Engine_Limitation){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Engine_Limitation = u8_Engine_Limitation & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Gun_Connected(uint8 u8_Gun){
   if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Gun_Connected = u8_Gun & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Handbreak(uint8 u8_Handbreak){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Handbreak = u8_Handbreak & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}
void Vision_Set_Stop_Required(uint8 u8_StopRequired){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) == C_NO_ERR)
    {
        t_Vision.u_Indicators.bits.b1_Stop_Required = u8_StopRequired & 0x1;
        x_os_mutex_release(&t_Mutex_Vision);
    }
}

void Vision_Set_Jarvis_Major_Version(uint8 u8_ver){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) != C_NO_ERR)
    {
        return;
    }
    t_Vision.t_Version.u8_Jarvis_VersionMajor = u8_ver;
    x_os_mutex_release(&t_Mutex_Vision);
}

void Vision_Set_Jarvis_Minor_Version(uint8 u8_ver){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) != C_NO_ERR)
    {
        return;
    }
    t_Vision.t_Version.u8_Jarvis_VersionMinor = u8_ver;
    x_os_mutex_release(&t_Mutex_Vision);
}

void Vision_Set_Jarvis_Pach_Version(uint8 u8_ver){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) != C_NO_ERR)
    {
        return;
    }
    t_Vision.t_Version.u8_Jarvis_VersionPatch = u8_ver;
    x_os_mutex_release(&t_Mutex_Vision);
}

void Vision_Set_Jarvis_Status_Version(En_VersionStatus en_status){
    if (x_os_mutex_get(&t_Mutex_Vision, 10) != C_NO_ERR)
    {
        return;
    }
    t_Vision.t_Version.en_Jarvis_VersionStatus = en_status;
    x_os_mutex_release(&t_Mutex_Vision);
}

void Vision_TX_Batteries(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = VISION_BATTERIES_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 2;

    t_Msg.au8_Data[0] = t_Vision.u8_SOC;
    t_Msg.au8_Data[1] = t_Vision.s8_Batteries_Temperature_C + 40;
    x_can_obj_send_msg(mpv_MsgObjTx_Vision, &t_Msg);
}
void Vision_TX_Vehicle(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = VISION_VEHICLE_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint8 u8_Temporal = t_Vision.f32_Voltage_DC_V * 5;

    t_Msg.au8_Data[0] = t_Vision.u_Indicators.all;

    t_Msg.au8_Data[1] = t_Vision.u8_Gear & 0x7;
    t_Msg.au8_Data[1] |= (t_Vision.u8_Waiting & 0x1) << 6;
    t_Msg.au8_Data[1] |= (t_Vision.u8_Air_Charging & 0x1) << 7;

    t_Msg.au8_Data[2] = (uint8)(t_Vision.u16_Isolation_kohm & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((t_Vision.u16_Isolation_kohm >> 8) & 0xFF);

    t_Msg.au8_Data[4] = (uint8)(t_Vision.s8_Engine_Temperature_C + 40);
    t_Msg.au8_Data[5] = u8_Temporal;
    t_Msg.au8_Data[6] = 0;
    u8_Temporal = t_Vision.u8_Pressure_dBar;
    t_Msg.au8_Data[7] = u8_Temporal;

    x_can_obj_send_msg(mpv_MsgObjTx_Vision, &t_Msg);
}
void Vision_TX_Version(uint8 u8_dev){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = VISION_VERSION_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 3;

    t_Msg.au8_Data[0] = u8_dev & 0x3;
    switch(u8_dev){
        case VISION_VERSION_ACU:
            t_Msg.au8_Data[0] |= (t_Vision.t_Version.u8_Jarvis_VersionMajor & 0x07) << 2;
            t_Msg.au8_Data[1] = t_Vision.t_Version.u8_Jarvis_VersionMinor;
            t_Msg.au8_Data[2] = t_Vision.t_Version.u8_Jarvis_VersionPatch;
            break;

        case VISION_VERSION_VCU:
            t_Msg.au8_Data[0] |= (t_Vision.t_Version.u8_Mark_VersionMajor & 0x07) << 2;
            t_Msg.au8_Data[1] = t_Vision.t_Version.u8_Mark_VersionMinor;
            t_Msg.au8_Data[2] = t_Vision.t_Version.u8_Mark_VersionPatch;
            break;
    }

    x_can_obj_send_msg(mpv_MsgObjTx_Vision, &t_Msg);

}

void Vision_TX_Fusi(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = VISION_FUSI_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 5;

    t_Msg.au8_Data[0] = (uint8)(t_Vision.u16_DTC & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((t_Vision.u16_DTC >> 8) & 0xFF);
    t_Msg.au8_Data[2] = t_Vision.u8_DTC_Level;
    t_Msg.au8_Data[3] = t_Vision.u8_Vehicle_State;
    t_Msg.au8_Data[4] = t_Vision.u8_Change_Page;
    t_Vision.u8_Change_Page = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_Vision, &t_Msg);
}

void Vision_TX_TostClear(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = VISION_FUSI_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 5;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = 0;
    t_Msg.au8_Data[2] = t_Vision.u8_DTC_Level;
    t_Msg.au8_Data[3] = t_Vision.u8_Vehicle_State;
    t_Msg.au8_Data[4] = 0;


    x_can_obj_send_msg(mpv_MsgObjTx_Vision, &t_Msg);
}

void Vision_TX_Odometer(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = VISION_ODOMETER_TX_ID;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 4;

    t_Msg.au8_Data[0] = (uint8)(t_Vision.u32_Odometer_100m & 0xFF);
    t_Msg.au8_Data[1] = (uint8)((t_Vision.u32_Odometer_100m >> 8) & 0xFF);
    t_Msg.au8_Data[2] = (uint8)((t_Vision.u32_Odometer_100m >> 16) & 0xFF);
    t_Msg.au8_Data[3] = (uint8)((t_Vision.u32_Odometer_100m >> 24) & 0xFF);

    x_can_obj_send_msg(mpv_MsgObjTx_Vision, &t_Msg);
}

void Vision_Update(void){
    t_Vision.u8_Critical_Counter++;
    if(t_Vision.u8_Critical_Counter >= VISION_CRITICAL_COUNTER_LIMIT){
        t_Vision.u8_Critical_Counter = 0;
        Vision_TX_Vehicle();
        return;
    }

    t_Vision.u8_Fast_Counter++;
    if(t_Vision.u8_Fast_Counter >= VISION_FAST_COUNTER_LIMIT){
        t_Vision.u8_Fast_Counter = 0;
        Vision_TX_Batteries();
        Vision_TX_Odometer();
        return;
    }

    t_Vision.u8_Normal_Counter++;
    if(t_Vision.u8_Normal_Counter >= VISION_NORMAL_COUNTER_LIMIT){
        t_Vision.u8_Normal_Counter = 0;
        Vision_TX_TostClear();
        Vision_TX_Fusi();
        return;
    }

    t_Vision.u8_Slow_Counter++;
    if(t_Vision.u8_Slow_Counter >= VISION_SLOW_COUNTER_LIMIT){
        t_Vision.u8_Slow_Counter = 0;
        Vision_TX_Version(VISION_VERSION_ACU);
        Vision_TX_Version(VISION_VERSION_VCU);
        return;
    }
}

uint8 Vision_Receive(void)
{
    sint16 s16_Error;
    uint8 u8_Recived;
    u8_Recived = FALSE;
    T_x_can_msg t_Msg;

    s16_Error = x_can_obj_get_msg(mpv_MsgObjRx_Vision, &t_Msg);
    while (s16_Error == C_NO_ERR)
    {
        u8_Recived = TRUE;
        if(t_Msg.t_Id.u32_Id == VISION_PERIODIC_RX_ID)
        {
            if(t_Msg.au8_Data[0] != t_Vision.u8_Rolling_Counter){
                t_Vision.u8_Rolling_Counter = t_Msg.au8_Data[0];
            }
        }
        s16_Error = x_can_obj_get_msg(mpv_MsgObjRx_Vision, &t_Msg);
    }

    return u8_Recived;
}

void Vision_Set_Timeout(void){
    t_Vision.u32_Timeout = Timeout_Set_ms(45000);
}

void Vision_off(uint16 u16_DTC, uint8 u8_key){
    if(u8_key == KEY_OFF){
        if(u16_DTC == 0){
            GPIO_set_T15_Screen(DS_ON);
        }
        else{
            if(Timeout_Check(t_Vision.u32_Timeout)){
                GPIO_set_T15_Screen(DS_ON);
            }
            else{
                GPIO_set_T15_Screen(DS_OFF);
            }
        }
    }
}

