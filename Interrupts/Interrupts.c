#include "OS_Defs.h"

void vTask_Outputs(void *opv_Arg)
{
    // sint16 s16_Error;
    uint32 u32_Events;
    uint8 u8_IsrCounter;
    uint8 u8_rol;
    u8_rol = 0;
    opv_Arg = opv_Arg;
    u8_IsrCounter = 0;

    while (TRUE)
    {
        x_os_wait_for_events();
        u32_Events = x_os_get_events(); // get events
        u8_IsrCounter++;
        if (u32_Events == FALSE)
        { // Timer event
            if (u8_IsrCounter >= 100)
            {
                MON_TX_Int_Ok(0, u8_rol); // Send Live Signal
                u8_rol++;
                u8_IsrCounter = 0;
            }
            if (u8_IsrCounter == 25 || u8_IsrCounter == 75)
            {
                Pump_TX_Control();
            }
            System_Output_Update();       //NO CAN

            Inverter_Send();        //CAN 2
            Battery_Send_Msg();     //CAN 3
            System_Output_Update();       //NO CAN
            Jarvis_TX_Command();    //CAN 1
            Update_Vision();        //NO CAN
            Vision_Update();
            Send_Panel();       //CAN 2
            Send_Tachograph();  //CAN 3
            Fusi_Send();
            SCU_Send_Msg();
            Steering_Send_Msg();
            BTMS_Send();
        }
    }
}

void vTask_Fault(void *opv_Arg)
{
    // sint16 s16_Error;
    uint32 u32_Events;
    uint8 u8_IsrCounter;
    uint8 u8_rol;
    u8_rol = 0;
    opv_Arg = opv_Arg;
    u8_IsrCounter = 0;

    while (TRUE)
    {
        x_os_wait_for_events();
        u32_Events = x_os_get_events(); // get events
        u8_IsrCounter++;
        if ((u32_Events & X_OS_EVENT_01) == X_OS_EVENT_01)
        { // Event 1 Triggered
            if (u8_IsrCounter >= 100)
            {
                MON_TX_Int_Ok(1, u8_rol); // Send Live Signal
                u8_rol++;
                u8_IsrCounter = 0;
            }
        }
    }
}

void vTask_State_Machine(void *opv_Arg)
{
    //    sint16 s16_Error;
    uint32 u32_Events;
    uint8 u8_IsrCounter;
    uint8 u8_rol;
    u8_rol = 0;
    opv_Arg = opv_Arg;
    u8_IsrCounter = 0;

    while (TRUE)
    {
        x_os_wait_for_events();
        u32_Events = x_os_get_events(); // get events
        u8_IsrCounter++;
        if (u32_Events == FALSE)
        { // Timer event
            if (u8_IsrCounter >= 100)
            {
                MON_TX_Int_Ok(2, u8_rol); // Send Live Signal
                u8_rol++;
                u8_IsrCounter = 0;
            }
            Run_System_Machine_State();
        }
    }
}

void vTask_Inputs(void *opv_Arg)
{
    //    sint16 s16_Error;
    uint32 u32_Events;
    uint8 u8_IsrCounter;
    uint8 u8_rol;
    u8_rol = 0;
    opv_Arg = opv_Arg;
    u8_IsrCounter = 0;

    while (TRUE)
    {
        x_os_wait_for_events();
        u32_Events = x_os_get_events(); // get events
        u8_IsrCounter++;
        if (u32_Events == FALSE)
        { // Timer event
            if (u8_IsrCounter % 2 == 0)
            {
                Get_Drive_Throttle();
            }
            if (u8_IsrCounter >= 100)
            {
                MON_TX_Int_Ok(3, u8_rol); // Send Live Signal
                u8_rol++;
                u8_IsrCounter = 0;
            }
            System_Input_Update();
            Tacho_eval();
        }
    }
}

void vTask_CAN_RX(void *opv_Arg)
{
    //    sint16 s16_Error;
    uint32 u32_Events;
    uint8 u8_IsrCounter;
    uint8 u8_rol;
    u8_rol = 0;
    opv_Arg = opv_Arg;
    u8_IsrCounter = 0;

    while (TRUE)
    {
        x_os_wait_for_events();
        u32_Events = x_os_get_events(); // get events
        u8_IsrCounter++;
        if (u32_Events == FALSE)
        { // Timer event
            if (u8_IsrCounter >= 100)
            {
                MON_TX_Int_Ok(4, u8_rol); // Send Live Signal
                u8_rol++;
                u8_IsrCounter = 0;
            }
            Buttons_Update();
            Gear_Update();


            Inverter_Received();

            if(Battery_Recived()){
                Telemetry_Track_Gun_Connection(Battery_get_Gun_Connection());
                Telemetry_Track_Battery_Heating(Battery_get_Heating());
                Telemetry_Track_BTMS(Battery_get_Cooling());
                Telemetry_Track_BMS_HV_State(Battery_get_BMS_HV_State());
            }

            if(Jarvis_Receive()){
                Telemetry_Track_PTC1(Jarvis_get_PTC1_State());
                Telemetry_Track_PTC2(Jarvis_get_PTC2_State());
            }
            Monitoring_Recived();
            Tacho_receive();
            Fusi_Recived();
            Vision_Receive();
            SCU_Recived_Msg();
            BTMS_Recived();
            Update_Buzzer();
        }
    }
}

void vTask_Monitoring(void *opv_Arg)
{
    //    sint16 s16_Error;
    uint32 u32_Events;
    uint8 u8_IsrCounter;
    uint8 u8_rol;
    u8_rol = 0;
    opv_Arg = opv_Arg;
    u8_IsrCounter = 0;

    while (TRUE)
    {
        x_os_wait_for_events();
        u32_Events = x_os_get_events(); // get events
        u8_IsrCounter++;
        if (u32_Events == FALSE)
        { //On each cycle 10ms
            if (u8_IsrCounter >= 100) // Send Live Signal every 1s
            {
                MON_TX_Int_Ok(5, u8_rol);
                u8_rol++;
                u8_IsrCounter = 0;
            }

            //Batteries_Send_Monitoring();
            //Inverter_Monitoring_Send();
            Monitoring_Send();
            Telemetry_Send();
        }
    }
}
