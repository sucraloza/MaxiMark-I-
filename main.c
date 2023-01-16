/*!
   \internal
   \file       main.c
   \brief      Mark-II

   \implementation
   project     ESX-3XL
   copyright   STW (c) 1999-2009
   license     use only under terms of contract / confidential

   created     23.10.2019 Tony
   \endimplementation

   \internal
   \history
   Date(dd.mm.yyyy)  Author        Description
   23.20.2019        Tony          file created
   09.01.2019        Sigritor      Test run
   \endhistory
*/

#include "OS_Defs.h"
#include "System/Global_Defs.h"

#include "MarkVersion.h"

const T_x_lib_info gt_AppInfo X_MEM_APPLICATION_INFO =
{
  X_LIB_INFO_APPL_MAGIC,                        // Magic key (signaling a valid Application Info Block) (do not edit!)
  X_LIB_INFO_STRUCT_VERSION,                    // Version information of the info struct               (do not edit!)
  X_LIB_INFO_CONTAINS_DEVICE_ID       |         // Bit - coded content Map;
  X_LIB_INFO_CONTAINS_DATE_TIME       |         // -> remove bit from content Map and the content itself
  X_LIB_INFO_CONTAINS_PROJECT_NAME    |
  X_LIB_INFO_CONTAINS_PROJECT_VERSION |
  X_LIB_INFO_CONTAINS_ADDITIONAL_INFO,
  X_LIB_INFO_DEVICE_ID,                         // ECU device ID  ; masked by X_LIB_INFO_CONTAINS_DEVICE_ID
  __DATE__,                                     // current date   ; masked by X_LIB_INFO_CONTAINS_DATE_TIME
  __TIME__,                                     // current time   ; masked by X_LIB_INFO_CONTAINS_DATE_TIME
  "Mark-II",                                    // project name   ; masked by X_LIB_INFO_CONTAINS_PROJECT_NAME
  "v0.0.0-a",                                   // project version; masked by X_LIB_INFO_CONTAINS_PROJECT_VERSION
  X_LIB_INFO_CONTAINS_ADDITIONAL_MAX,           // size of additional information (max. 0xFF)
                                                //                ; masked by X_LIB_INFO_CONTAINS_ADDITIONAL_INFO
  "Additional info of \"Mark-II\"."             // Additional Info (max. 255 chars)
                                                //                ; masked by X_LIB_INFO_CONTAINS_ADDITIONAL_INFO
};

int main(void)
{
    T_x_sys_system_information tSys;
    sint16 s16_Error;
    uint8  u8_Ignition;
    T_x_can_obj_config t_CanObjInit;

    s16_Error = x_sys_init(&tSys);         // init ESX-3XL hardware, this function call is mandatory
    if(s16_Error != C_NO_ERR) Error_Beep();

    x_sys_set_all_relays(X_ON);   // enable power stage

    //Interrupt system
    s16_Error = x_os_init(X_OS_HEAP_DEFAULT);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error =  x_os_install_task(6, 10, 100, vTask_Outputs, 200);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error =  x_os_install_task(5, 0, 100, vTask_Fault, 200);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error =  x_os_install_task(4, 10, 100, vTask_State_Machine, 200);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error =  x_os_install_task(3, 10, 100, vTask_Inputs, 200);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error =  x_os_install_task(2, 5, 100, vTask_CAN_RX, 200);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error =  x_os_install_task(1, 10, 100, vTask_Monitoring, 200);
    if(s16_Error != C_NO_ERR) Error_Beep();






    //CAN Configuration
    s16_Error = x_can_bus_init(X_CAN_BUS_01, 250000u);
    if(s16_Error != C_NO_ERR) Error_Beep();

    x_can_bus_set_masks(X_CAN_BUS_01,0x7F0,0x000000FF); //Global Enable Mask
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00000000u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_Monitoring);

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x0000A088u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_Jarvis);


    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00001114u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_CAPL);

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x522u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_PTC1);

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00008817u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_Fusi);


    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x000088A0u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjRx_Jarvis);

    // init a RX object on CAN node1
    t_CanObjInit.t_Id.u32_Id = 0x00001488u;         // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjRx_Monitoring);

    // init a RX object on CAN node1
    t_CanObjInit.t_Id.u32_Id = 0x502u;         // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;                // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjRx_PTC1);

    //init a RX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00001788u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjRx_Fusi);






    s16_Error = x_can_bus_init(X_CAN_BUS_02, 500000u);
    if(s16_Error != C_NO_ERR) Error_Beep();

    x_can_bus_set_masks(X_CAN_BUS_02,0x0F0,0x000000FF);
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;        // no callback function

    // init a TX object on CAN node2
    t_CanObjInit.t_Id.u32_Id = 0x040u;              // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_02, &t_CanObjInit, &mpv_MsgObjTx_Inverter);

    // init a TX object on CAN node2
    t_CanObjInit.t_Id.u32_Id = 0x000u;              // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_02, &t_CanObjInit, &mpv_MsgObjTx_Panel);


    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;        // no callback function

    // init a RX object on CAN node2
    t_CanObjInit.t_Id.u32_Id = 0x040u;              // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    x_can_obj_init(X_CAN_BUS_02, &t_CanObjInit, &mpv_MsgObjRx_Inverter);






    s16_Error = x_can_bus_init(X_CAN_BUS_03, 250000u);
    if(s16_Error != C_NO_ERR) Error_Beep();

    x_can_bus_set_masks(X_CAN_BUS_03,0x7F0,0x000000FF);
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    // init a TX object on CAN node3 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00000000u;         // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 11bit ID
    x_can_obj_init(X_CAN_BUS_03, &t_CanObjInit, &mpv_MsgObjTx_Pump);

    // init a TX object on CAN node3
    t_CanObjInit.t_Id.u32_Id = 0x0000F3EFu;         // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 29bit ID
    x_can_obj_init(X_CAN_BUS_03, &t_CanObjInit, &mpv_MsgObjTx_Battery);

    // init a TX object on CAN node3
    t_CanObjInit.t_Id.u32_Id = 0x00000000u;         // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 29bit ID
    x_can_obj_init(X_CAN_BUS_03, &t_CanObjInit, &mpv_MsgObjTx_Tachograph);


    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    // init a RX object on CAN node3
    t_CanObjInit.t_Id.u32_Id = 0x0000EFF3u;         // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 29bit ID
    x_can_obj_init(X_CAN_BUS_03, &t_CanObjInit, &mpv_MsgObjRx_Battery);

    // init a RX object on CAN node3
    t_CanObjInit.t_Id.u32_Id = 0x00000017u;          // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 29bit ID
    x_can_obj_init(X_CAN_BUS_03, &t_CanObjInit, &mpv_MsgObjRx_Tachograph);






    s16_Error = x_can_bus_init(X_CAN_BUS_04, 250000u);
    if(s16_Error != C_NO_ERR) Error_Beep();

    x_can_bus_set_masks(X_CAN_BUS_04,0x7F0,0x000000FF);
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    // init a TX object on CAN node4 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x501u;              // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjTx_PTC4);
    if(s16_Error != C_NO_ERR) Error_Beep();

    //init a TX object on CAN node4 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00008817u;         // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 11bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjTx_Fusi4);
    if(s16_Error != C_NO_ERR) Error_Beep();

    //init a TX object on CAN node4 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00008819u;         // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                   // use 11bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjTx_Telemetry);
    if(s16_Error != C_NO_ERR) Error_Beep();


    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

/*     // init a RX object on CAN node4
    t_CanObjInit.t_Id.u32_Id = 0x00000021u;         // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                // use 29bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjRx_TachographBug);
    if(s16_Error != C_NO_ERR) Error_Beep(); */

    /* T_x_can_id t_CompareId;             T_x_can_id t_Mask;
    t_Mask.u32_Id      = 0x000000FFu;   t_Mask.u8_Xtd = TRUE;
    t_CompareId.u32_Id = 0x00000021u;   t_CompareId.u8_Xtd = TRUE;
    x_can_obj_set_filter(mpv_MsgObjRx_TachographBug, &t_CompareId, &t_Mask); */

    // init a RX object on CAN node4
    t_CanObjInit.t_Id.u32_Id = 0x511;               // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 29bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjRx_PTC4);
    if(s16_Error != C_NO_ERR) Error_Beep();



    GPIO_Init();
    Key_Init();
    HVIL_Init();
    SCU_Init();
    Steering_Init();
    Vision_Init();
    BTMS_Init();

    //Variables Initialization
    //Debugging
    INIT_DEBUGGING(debuggeo)
    //Vehicle Systems
    INIT_VEHICLE_SYSTEMS(st_System)
    s16_Error = x_os_mutex_init(&t_Mutex_System);
    if(s16_Error != C_NO_ERR) Error_Beep();
    //Batteries
    INIT_BATTERY_VARS(st_Battery)
    s16_Error = x_os_mutex_init(&t_Mutex_Battery);
    if(s16_Error != C_NO_ERR) Error_Beep();
    s16_Error = x_os_mutex_init(&t_Mutex_Battery_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();

    INIT_BATTERY_CONTERS(st_Battery_Counters);

    //Monitoring
    s16_Error = x_os_mutex_init(&t_Mutex_Monitoring_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();
    //Jarvis
    INIT_JARVIS(st_Jarvis)
    s16_Error = x_os_mutex_init(&t_Mutex_Jarvis);
    if(s16_Error != C_NO_ERR) Error_Beep();
    s16_Error = x_os_mutex_init(&t_Mutex_Jarvis_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();
    //PDU
    INIT_PDU(st_PDU)
    s16_Error = x_os_mutex_init(&t_Mutex_PDU);
    if(s16_Error != C_NO_ERR) Error_Beep();
    //INVERTER
    INIT_INVERTER(st_Inverter)
    s16_Error = x_os_mutex_init(&t_Mutex_Inverter);
    if(s16_Error != C_NO_ERR) Error_Beep();
    s16_Error = x_os_mutex_init(&t_Mutex_Inverter_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();
    INMON_Counter = 0;  //Counter Monitoreo Inverter
    //Throttle
    INIT_THROTTLE(st_Throttle);
    s16_Error = x_os_mutex_init(&t_Mutex_Throttle);
    if(s16_Error != C_NO_ERR) Error_Beep();

    u8_Enable_Cooling_Pump = FALSE;

    //Monitoring
    MONITORING_INIT(st_Monitoring)

    INIT_PANEL(st_Panel);

    INIT_FUSI(st_Fusi);
    INIT_FUSI_COUNTERS(st_Fusi_Counters);
    Fusi_Init_DTCs();
    s16_Error = x_os_mutex_init(&t_Mutex_Fusi_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();

    INIT_TACHOGRAPH(st_Tachograph);
    INIT_TACHOGRAPH_RXVARS(un_Tachograph_RxVars);
    s16_Error = x_os_mutex_init(&t_Mutex_Tacho_Msg);
    if(s16_Error != C_NO_ERR) Error_Beep();

    Buttons_Init();
    Gear_Init();
    Buzzer_Init();

    EEPROM_Init(X_CAN_BUS_01);
    AC_Config_Init();

    Fusi_Eep_Load();


    x_os_start(1000);

    while (TRUE)
    {
        x_sys_delay_us(10000);                // delay 10 milli sec
        x_sys_get_ignition(&u8_Ignition);     // read D+ signal
        if (u8_Ignition == X_OFF)             // no D+ signal?
        {
            // clean up...
            Fusi_Eep_Save();
            x_sys_delay_us(3000000);          // delay 3 sec
            x_sys_close();                    // shut down system ...
        }
    }
}
