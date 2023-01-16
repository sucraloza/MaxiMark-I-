#include "OS_Defs.h"

void Jarvis_TX_Command(void)
{
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x1805A088;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 6;

    uint8 u8_Gear, u8_Ignition;
    switch (Gear_get_State())
    {
        case GEAR_NEUTRAL: u8_Gear = 0x0; break;
        case GEAR_PARK: u8_Gear = 0x1; break;
        case GEAR_DIRECT: u8_Gear = 0xA; break;
        case GEAR_REVERSE: u8_Gear = 0x5; break;
        default: u8_Gear = 0x0; break;
    }

    switch (st_Jarvis.st_Command.u8_Ignition)
    {
        case KEY_OFF: u8_Ignition = 0x0; break;
        case KEY_ACCESORY: u8_Ignition = 0x1; break;
        case KEY_START: u8_Ignition = 0x3; break;
        default: u8_Ignition = 0x0; break;
    }

    t_Msg.au8_Data[0] = st_Jarvis.st_Command.u8_Is_Regen;
    t_Msg.au8_Data[1] = st_Jarvis.st_Command.u8_Speed_kmh;
    t_Msg.au8_Data[2] = ((u8_Gear & 0x0F) << 0) | ((u8_Ignition & 0x03) << 4);

    if(st_System.st_State.u8_ECU_State == ECU_STATE_GPIO_CHECK){
        t_Msg.au8_Data[3] = ECU_STATE_SLEEP;
    }
    else{
        t_Msg.au8_Data[3] = st_System.st_State.u8_ECU_State;
    }

    t_Msg.au8_Data[4] = st_Battery.st_State_2.u8_SOC;
    t_Msg.au8_Data[5] = (uint8) (SCU_get_PSU_Temperature_C() + 40);

    x_can_obj_send_msg(mpv_MsgObjTx_Jarvis, &t_Msg);
}

void Jarvis_RX_Converter(void)
{
    if (t_Jarvis_Msg.t_Id.u32_Id != 0x181088A0)
    {
        return;
    }

    sint8 s8_PSU_Temperature_C;
    sint8 s8_Device_Temperature_C;
    float32 f32_Auxiliary_Voltage_V;
    uint16 u16_DC_Voltage_V;
    uint16 u16_DC_Power_kW;
    float32 f32_DC_Power_kW;
    uint8 u8_Flags;
    uint8 u8_Charging_Current_A;
    uint8 u8_Avg_Charging_Current_A;

    s8_PSU_Temperature_C = t_Jarvis_Msg.au8_Data[0] - 40;
    u16_DC_Voltage_V = t_Jarvis_Msg.au8_Data[1] * 4;
    f32_Auxiliary_Voltage_V = t_Jarvis_Msg.au8_Data[2] * 0.2f;
    u16_DC_Power_kW = t_Jarvis_Msg.au8_Data[4];
    u16_DC_Power_kW = u16_DC_Power_kW << 8;
    f32_DC_Power_kW = u16_DC_Power_kW + t_Jarvis_Msg.au8_Data[3];

    f32_DC_Power_kW = (f32_DC_Power_kW * 0.005f) - 160;
    u8_Flags = t_Jarvis_Msg.au8_Data[5];

    s8_Device_Temperature_C = t_Jarvis_Msg.au8_Data[6] - 40;

    u8_Charging_Current_A = t_Jarvis_Msg.au8_Data[7];
    u8_Avg_Charging_Current_A = 0.94f * st_Jarvis.st_Aux_Converter.u8_Avg_Charging_Current_A + 0.04f * u8_Charging_Current_A;

    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }

    st_Jarvis.st_Aux_Converter.s8_PSU_Temperature_C = s8_PSU_Temperature_C;
    st_Jarvis.st_Aux_Converter.s8_Device_Temperature_C = s8_Device_Temperature_C;
    st_Jarvis.st_Aux_Converter.f32_Auxiliary_Voltage_V = f32_Auxiliary_Voltage_V;
    st_Jarvis.st_Aux_Converter.u16_DC_Voltage_V = u16_DC_Voltage_V;
    st_Jarvis.st_Aux_Converter.f32_DC_Power_kW = f32_DC_Power_kW;
    st_Jarvis.st_Aux_Converter.un_Flags.all = u8_Flags;
    st_Jarvis.st_Aux_Converter.u8_Charging_Current_A =  u8_Charging_Current_A;
    st_Jarvis.st_Aux_Converter.u8_Avg_Charging_Current_A = u8_Avg_Charging_Current_A;
    x_os_mutex_release(&t_Mutex_Jarvis);
}

void Jarvis_RX_GPIO(void)
{
    if (t_Jarvis_Msg.t_Id.u32_Id != 0x180088A0)
    {
        return;
    }

    uint16 u16_Flags;
    uint16 u16_Pressure1;
    uint16 u16_Pressure2;

    u16_Flags = (t_Jarvis_Msg.au8_Data[5] << 8) | t_Jarvis_Msg.au8_Data[0];
    u16_Pressure1 = (t_Jarvis_Msg.au8_Data[2] << 8) | t_Jarvis_Msg.au8_Data[1];
    u16_Pressure2 = (t_Jarvis_Msg.au8_Data[4] << 8) | t_Jarvis_Msg.au8_Data[3];

    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }

    st_Jarvis.st_GPIO.un_Flags.all = u16_Flags;
    st_Jarvis.st_GPIO.u16_Pressure1_mBar = u16_Pressure1;
    st_Jarvis.st_GPIO.u16_Pressure2_mBar = u16_Pressure2;

    x_os_mutex_release(&t_Mutex_Jarvis);
}

void Jarvis_RX_Version(void)
{
    if (t_Jarvis_Msg.t_Id.u32_Id != 0x182088A0)
    {
        return;
    }

    uint8 u8_major, u8_minor, u8_patch, u8_status;

    u8_major = t_Jarvis_Msg.au8_Data[0];
    u8_minor = t_Jarvis_Msg.au8_Data[1];
    u8_patch = t_Jarvis_Msg.au8_Data[2];
    u8_status = t_Jarvis_Msg.au8_Data[3] & 0x07;

    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }

    st_Jarvis.st_Version.u8_Version_Major = u8_major;
    st_Jarvis.st_Version.u8_Version_Minor = u8_minor;
    st_Jarvis.st_Version.u8_Version_Patch = u8_patch;
    st_Jarvis.st_Version.en_Version_Status = u8_status;

    x_os_mutex_release(&t_Mutex_Jarvis);
}

void Jarvis_RX_System(void)
{
    if (t_Jarvis_Msg.t_Id.u32_Id != 0x180488A0)
    {
        return;
    }

    uint16 u16_Flags;
    uint8 u8_Compresor_State;

    u16_Flags = (t_Jarvis_Msg.au8_Data[1] << 8) | t_Jarvis_Msg.au8_Data[0];

    u8_Compresor_State = (t_Jarvis_Msg.au8_Data[2] >> 3) & 0x1;

    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }

    st_Jarvis.un_System.all = u16_Flags;
    st_Jarvis.st_Aux_Converter.u8_Compresor_State = u8_Compresor_State;

    x_os_mutex_release(&t_Mutex_Jarvis);
}

void Jarvis_RX_PTC(void){
    if (t_Jarvis_Msg.t_Id.u32_Id != 0x181788A0)
    {
        return;
    }

    uint8  u8_PTC1_State;
    sint8  s8_PTC1_Temperature_C;
    uint16 u16_PTC1_Power_W;
    uint8  u8_PTC2_State;
    sint8  s8_PTC2_Temperature_C;
    uint16 u16_PTC2_Power_W;


    u8_PTC1_State = t_Jarvis_Msg.au8_Data[0] & 0x1;
    u8_PTC2_State = (t_Jarvis_Msg.au8_Data[0] & 0x2) >> 1;
    s8_PTC1_Temperature_C = t_Jarvis_Msg.au8_Data[1] - 40;
    u16_PTC1_Power_W = t_Jarvis_Msg.au8_Data[2] * 50;
    s8_PTC2_Temperature_C = t_Jarvis_Msg.au8_Data[3] - 40;
    u16_PTC2_Power_W = t_Jarvis_Msg.au8_Data[4] * 50;

    if (x_os_mutex_get(&t_Mutex_Jarvis, 10) != C_NO_ERR)
    {
        return;
    }

    st_Jarvis.st_PTC.u8_PTC1_State = u8_PTC1_State;
    st_Jarvis.st_PTC.u8_PTC2_State = u8_PTC2_State;
    st_Jarvis.st_PTC.s8_PTC1_Temperature_C = s8_PTC1_Temperature_C;
    st_Jarvis.st_PTC.u16_PTC1_Power_W = u16_PTC1_Power_W;
    st_Jarvis.st_PTC.s8_PTC2_Temperature_C = s8_PTC2_Temperature_C;
    st_Jarvis.st_PTC.u16_PTC2_Power_W = u16_PTC2_Power_W;

    x_os_mutex_release(&t_Mutex_Jarvis);
}

uint8 Jarvis_Receive(void)
{
    sint16 s16_Error;
    sint16 s16_Error_Can;
    uint8 u8_Recived;
    u8_Recived = FALSE;

    s16_Error = x_os_mutex_get(&t_Mutex_Jarvis_Msg, 10);
    if (s16_Error != C_NO_ERR)
    {
        Error_Beep();
        return u8_Recived;
    }

    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Jarvis, &t_Jarvis_Msg);
    while (s16_Error_Can == C_NO_ERR && s16_Error == C_NO_ERR)
    {
        u8_Recived = TRUE;
        switch (t_Jarvis_Msg.t_Id.u32_Id)
        {
            case 0x181088A0:
                x_os_mutex_release(&t_Mutex_Jarvis_Msg);
                Jarvis_RX_Converter();
                break;

            case 0x180088A0:
                x_os_mutex_release(&t_Mutex_Jarvis_Msg);
                Jarvis_RX_GPIO();
                break;

            case 0x182088A0:
                x_os_mutex_release(&t_Mutex_Jarvis_Msg);
                Jarvis_RX_Version();
                break;

            case 0x180488A0:
                x_os_mutex_release(&t_Mutex_Jarvis_Msg);
                Jarvis_RX_System();
                break;

            case 0x181788A0:
                x_os_mutex_release(&t_Mutex_Jarvis_Msg);
                Jarvis_RX_PTC();
                break;

            default: x_os_mutex_release(&t_Mutex_Jarvis_Msg); break;
        }
        s16_Error = x_os_mutex_get(&t_Mutex_Jarvis_Msg, 10); // Trato de obtener el mutex denuevo
        if (s16_Error != C_NO_ERR)
        {
            break; // No obtube el mutex por lo que salgo del while
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_Jarvis, &t_Jarvis_Msg);
    }
    x_os_mutex_release(&t_Mutex_Jarvis_Msg);
    return u8_Recived;
}
