#include "Os_Defs.h"
#include "BM_Buttons.h"

T_BM_Buttons_Vars t_BM_Buttons;

void *mpv_MsgObjRx_BM_Buttons;
void *mpv_MsgObjTx_BM_Buttons;

/** Transmitters Declaration*/
sint16 BM_Buttons_TX_Std_Msg(uint8 u8_Command, uint8 u8_Data);
sint16 BM_Buttons_TX_LED_Command(uint8 u8_index);
sint16 BM_Buttons_TX_Temp_Led_Brightness(void);
sint16 BM_Buttons_TX_Temp_Backlight_Brightness(void);
sint16 BM_Buttons_TX_StartupMessage(void);
sint16 BM_Buttons_TX_SoftwareVersion(void);
sint16 BM_Buttons_TX_Startup_Show(void);
sint16 BM_Buttons_TX_ECU_Address(void);
sint16 BM_Buttons_TX_Baudrate(void);
sint16 BM_Buttons_TX_Keypad_Address(void);
sint16 BM_Buttons_TX_Enable_Periodic_Send(void);
sint16 BM_Buttons_TX_Enable_Event_Transmition(void);
sint16 BM_Buttons_TX_Enable_Led_Ack(void);
sint16 BM_Buttons_TX_Enable_Claim(void);
sint16 BM_Buttons_TX_Heartbeat(void);
sint16 BM_Buttons_TX_Set_Period(void);
sint16 BM_Buttons_TX_Demo(void);
sint16 BM_Buttons_TX_Default_Backlight_Brightness(void);
sint16 BM_Buttons_TX_Default_Led_Brightness(void);
sint16 BM_Buttons_TX_Backlight_Color(void);

/* Recivers Declaration */
void BM_Buttons_RX_Software_Version(uint8 *au8_data);
void BM_Buttons_RX_Contact_State(uint8 *au8_data);
void BM_Buttons_RX_Heartbeat(uint8 *au8_data);

/** Initialization Inplementation*/
void BM_Buttons_Init(void)
{
    sint16 s16_Error;

    t_BM_Buttons.u8_Heartbeat_Period_10ms = 0;
    t_BM_Buttons.u8_Transmission_Period_10ms = 0;
	t_BM_Buttons.u_Config.all = 0;
	t_BM_Buttons.u_Config.bits.b1_event_tx = 1;
    t_BM_Buttons.f32_Led_Brightness = 1.0f;
	t_BM_Buttons.f32_Backlight_Brightness = 0.5f;
	t_BM_Buttons.e_Show = BT_LED_SHOW_OFF;
	t_BM_Buttons.e_Backlight_Color = BT_LED_COLOR_AMBER;
    t_BM_Buttons.u8_life = 0;
    t_BM_Buttons.u8_Config_Turn = 0;

	T_Key_Vars t_Key;
	t_Key.e_Color_1 = BT_LED_COLOR_YELLOW_GREEN;
	t_Key.e_Color_2 = BT_LED_COLOR_OFF;
	t_Key.e_Key_Enable = BT_KEY_DISABLED;
    t_Key.e_Key_State = BT_KEY_STATE_OFF;
	t_Key.e_LED_State = BT_LED_STATE_OFF;

    uint8 u8_count;
    for (u8_count = 1; u8_count <= BT_BUTTON_COUNT; u8_count++)
	{
	    BM_Buttons_Config_Key(u8_count, &t_Key);
	}

    uint32 u32_Address = BT_ECU_ADDRESS;
    u32_Address = u32_Address << 8;
    u32_Address = 0x18EF0000 + u32_Address + BT_KEYPAD_ADDRESS;
    t_BM_Buttons.u32_RX_Id = u32_Address;

    u32_Address = BT_KEYPAD_ADDRESS;
    u32_Address = u32_Address << 8;
    u32_Address = 0x18EF0000 + u32_Address + BT_ECU_ADDRESS;
    t_BM_Buttons.u32_TX_Id = u32_Address;


    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = t_BM_Buttons.u32_RX_Id;     // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                    // use 11bit ID
    s16_Error = x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjRx_BM_Buttons);

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 5u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = t_BM_Buttons.u32_TX_Id;     // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;                    // use 11bit ID
    x_can_obj_init(X_CAN_BUS_04, &t_CanObjInit, &mpv_MsgObjTx_BM_Buttons);
}

sint16 BM_Buttons_General_Config(void){
    sint16 s16_Error;
    switch(t_BM_Buttons.u8_Config_Turn){
        case BT_CONFIG_STATE_ECU_ADDRESS:
            s16_Error = BM_Buttons_TX_ECU_Address();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_BOUDRATE:
            s16_Error = BM_Buttons_TX_Baudrate();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_KEYPDA_ADDRESS:
            s16_Error = BM_Buttons_TX_Keypad_Address();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_HEARTBEAT_PERIOD:
            s16_Error = BM_Buttons_TX_Heartbeat();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_SEND_PERIOD:
            s16_Error = BM_Buttons_TX_Set_Period();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_HEARTBEAT_ENABLE:
            s16_Error = BM_Buttons_TX_Enable_Periodic_Send();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_EVENT_ENABLE:
            s16_Error = BM_Buttons_TX_Enable_Event_Transmition();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_SHOW_ENABLE:
            s16_Error = BM_Buttons_TX_Startup_Show();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_LED_BRHIGHTNESS:
            s16_Error = BM_Buttons_TX_Temp_Led_Brightness();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        case BT_CONFIG_STATE_BALCKLIGHT_BRHIGHTNESS:
            s16_Error = BM_Buttons_TX_Temp_Backlight_Brightness();
            if(s16_Error != C_NO_ERR) return s16_Error;
            else {t_BM_Buttons.u8_Config_Turn++; return FALSE;}
        default:
            t_BM_Buttons.u8_Config_Turn = 0;
            return TRUE;
    }
}

sint16 BM_Buttons_Key_Send_Config(uint8 u8_index){
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return -1;
    return BM_Buttons_TX_LED_Command(u8_index);
}

/** Setters Inplementation*/
void BM_Buttons_Config_Key(uint8 u8_index, T_Key_Vars *pt_key)
{
	if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;

	t_BM_Buttons.at_Keys[u8_index - 1] = *pt_key;
	t_BM_Buttons.at_Keys[u8_index - 1].u8_Index = u8_index;
}
void BM_Buttons_Set_Key_Disable(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
	t_BM_Buttons.at_Keys[u8_index - 1].e_Key_Enable = BT_KEY_DISABLED;
    t_BM_Buttons.at_Keys[u8_index - 1].e_LED_State = BT_LED_STATE_OFF;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Key_Enabled(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
	t_BM_Buttons.at_Keys[u8_index - 1].e_Key_Enable = BT_KEY_ENABLED;
    t_BM_Buttons.at_Keys[u8_index - 1].e_LED_State = BT_LED_STATE_BLINK;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Led_Off(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
	if (t_BM_Buttons.at_Keys[u8_index - 1].e_Key_Enable == BT_KEY_DISABLED) return;
	t_BM_Buttons.at_Keys[u8_index - 1].e_LED_State = BT_LED_STATE_OFF;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Led_On(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
	if (t_BM_Buttons.at_Keys[u8_index - 1].e_Key_Enable == BT_KEY_DISABLED) return;
	t_BM_Buttons.at_Keys[u8_index - 1].e_LED_State = BT_LED_STATE_ON;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Led_Blink(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
	if (t_BM_Buttons.at_Keys[u8_index - 1].e_Key_Enable == BT_KEY_DISABLED) return;
	t_BM_Buttons.at_Keys[u8_index - 1].e_LED_State = BT_LED_STATE_BLINK;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Led_AltBlink(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
	if (t_BM_Buttons.at_Keys[u8_index - 1].e_Key_Enable == BT_KEY_DISABLED) return;
	t_BM_Buttons.at_Keys[u8_index - 1].e_LED_State = BT_LED_STATE_ALTERNATE;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Led_Collor(uint8 u8_index, E_Button_LED_Color e_color)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
    t_BM_Buttons.at_Keys[u8_index - 1].e_Color_1 = e_color;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Led_Collor_Alt(uint8 u8_index, E_Button_LED_Color e_color)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
    t_BM_Buttons.at_Keys[u8_index - 1].e_Color_2 = e_color;
    BM_Buttons_TX_LED_Command(u8_index);
}
void BM_Buttons_Set_Backlight_Collor(E_Button_LED_Color e_color)
{
    if ((e_color == BT_LED_COLOR_OFF) || ((uint8) e_color) > 0x9) e_color = BT_LED_COLOR_AMBER;
    t_BM_Buttons.e_Backlight_Color = e_color;
    BM_Buttons_TX_Backlight_Color();
}
void BM_Buttons_Set_Led_Brightness(float32 f32_level)
{
    if(f32_level > 1.0f) f32_level = 1.0f;
    if(f32_level < 0.0f) f32_level = 0.0f;
    t_BM_Buttons.f32_Led_Brightness = f32_level;
    BM_Buttons_TX_Temp_Led_Brightness();
}
void BM_Buttons_Set_Backlight_Brightness(float32 f32_level)
{
    if(f32_level > 1.0f) f32_level = 1.0f;
    if(f32_level < 0.0f) f32_level = 0.0f;
    t_BM_Buttons.f32_Backlight_Brightness = f32_level;
    BM_Buttons_TX_Temp_Backlight_Brightness();
}
void BM_Buttons_Set_Heartbeat(uint8 u8_period_10ms)
{
    if (u8_period_10ms == 0){
        t_BM_Buttons.u8_Heartbeat_Period_10ms = 0x05;
        t_BM_Buttons.u_Config.bits.b1_heartbeat_tx = 0;
        return;
    }
    if (u8_period_10ms > 0xFE) u8_period_10ms = 0xFE;
    if (u8_period_10ms < 0x05) u8_period_10ms = 0x05;
    t_BM_Buttons.u8_Heartbeat_Period_10ms = u8_period_10ms;
    t_BM_Buttons.u_Config.bits.b1_heartbeat_tx = 1;
    BM_Buttons_TX_Heartbeat();
}
void BM_Buttons_Set_Period(uint8 u8_period_10ms)
{
    if (u8_period_10ms == 0){
        t_BM_Buttons.u8_Transmission_Period_10ms = 0x05;
        t_BM_Buttons.u_Config.bits.b1_periodic_tx = 0;
        return;
    }
    if (u8_period_10ms > 0xFE) u8_period_10ms = 0xFE;
    if (u8_period_10ms < 0x05) u8_period_10ms = 0x05;
    t_BM_Buttons.u8_Transmission_Period_10ms = u8_period_10ms;
    t_BM_Buttons.u_Config.bits.b1_periodic_tx = 1;
    BM_Buttons_TX_Enable_Periodic_Send();
    BM_Buttons_TX_Set_Period();
}
void BM_Buttons_Set_OnEvent_Send(uint8 u8_ref)
{
    t_BM_Buttons.u_Config.bits.b1_event_tx = (u8_ref != 0);
    BM_Buttons_TX_Enable_Event_Transmition();
}

/** Getters Inplementation*/
void BM_Buttons_Get_Key(uint8 u8_index, T_Key_Vars *pt_key)
{
	if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return;
    *pt_key = t_BM_Buttons.at_Keys[u8_index - 1];
}
E_Key_State BM_Buttons_Get_Key_State(uint8 u8_index)
{
	if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return (E_Key_State) 0;
    return t_BM_Buttons.at_Keys[u8_index - 1].e_Key_State;
}
uint8 BM_Buttons_Is_On(uint8 u8_index)
{
	if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return 0;
    return (t_BM_Buttons.at_Keys[u8_index - 1].e_Key_State == BT_KEY_STATE_ON);
}
float32 BM_Buttons_Get_Led_Brightness(void)
{
	return t_BM_Buttons.f32_Led_Brightness;
}
float32 BM_Buttons_Get_Backlight_Brightness(void)
{
	return t_BM_Buttons.f32_Backlight_Brightness;
}


/** Transmitters Inplementation*/
sint16 BM_Buttons_TX_Std_Msg(uint8 u8_Command, uint8 u8_Data)
{
    T_x_can_msg t_msg;
    t_msg.t_Id.u32_Id = t_BM_Buttons.u32_TX_Id;
    t_msg.t_Id.u8_Xtd = TRUE;
    t_msg.u8_Dlc = 8;

    t_msg.au8_Data[0] = (uint8) ((BT_HEADER >> 8) & 0xFF);
    t_msg.au8_Data[1] = (uint8) ((BT_HEADER >> 0) & 0xFF);
    t_msg.au8_Data[2] = u8_Command;
    t_msg.au8_Data[3] = u8_Data;
    t_msg.au8_Data[4] = 0xFF;
    t_msg.au8_Data[5] = 0xFF;
    t_msg.au8_Data[6] = 0xFF;
    t_msg.au8_Data[7] = 0xFF;


    return x_can_obj_send_msg(mpv_MsgObjTx_BM_Buttons, &t_msg);
}
sint16 BM_Buttons_TX_LED_Command(uint8 u8_index)
{
    if ((u8_index > BT_BUTTON_COUNT) || (u8_index == 0)) return -1;
    T_Key_Vars t_key;
    BM_Buttons_Get_Key(u8_index, &t_key);

    T_x_can_msg t_msg;
    t_msg.t_Id.u32_Id = t_BM_Buttons.u32_TX_Id;
    t_msg.t_Id.u8_Xtd = TRUE;
    t_msg.u8_Dlc = 8;

    t_msg.au8_Data[0] = (uint8) ((BT_HEADER >> 8) & 0xFF);
    t_msg.au8_Data[1] = (uint8) ((BT_HEADER >> 0) & 0xFF);
    t_msg.au8_Data[2] = (uint8) BT_COMMAND_LED;
    t_msg.au8_Data[3] = t_key.u8_Index;
    t_msg.au8_Data[4] = (uint8) t_key.e_Color_1;
    t_msg.au8_Data[5] = (uint8) t_key.e_LED_State;
    t_msg.au8_Data[6] = (uint8) t_key.e_Color_2;
    t_msg.au8_Data[7] = 0xFF;

    return x_can_obj_send_msg(mpv_MsgObjTx_BM_Buttons, &t_msg);
}
sint16 BM_Buttons_TX_Temp_Led_Brightness(void)
{
    uint8 u8_brightness = (t_BM_Buttons.f32_Led_Brightness * 0.64f);
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_LED_BRIGHTNESS,u8_brightness);
}
sint16 BM_Buttons_TX_Temp_Backlight_Brightness(void)
{
    uint8 u8_backlight = (t_BM_Buttons.f32_Backlight_Brightness * 0.64f);
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_BACKLIGHT_LEVEL,u8_backlight);
}
sint16 BM_Buttons_TX_StartupMessage(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_STARTUP_MESSAGE,t_BM_Buttons.u_Config.bits.b1_startup_msg);
}
sint16 BM_Buttons_TX_SoftwareVersion(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_SOFTWARE_VERSION,0xFF);
}
sint16 BM_Buttons_TX_Startup_Show(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_LED_SHOW,((uint8) t_BM_Buttons.e_Show) & 0x3);
}
sint16 BM_Buttons_TX_ECU_Address(void)
{
    T_x_can_msg t_msg;
    t_msg.t_Id.u32_Id = 0x18EF2100u;
    t_msg.t_Id.u8_Xtd = TRUE;
    t_msg.u8_Dlc = 8;

    t_msg.au8_Data[0] = (uint8) ((BT_HEADER >> 8) & 0xFF);
    t_msg.au8_Data[1] = (uint8) ((BT_HEADER >> 0) & 0xFF);
    t_msg.au8_Data[2] = BT_COMMAND_DESTINATION_ADDRESS;
    t_msg.au8_Data[3] = BT_ECU_ADDRESS;
    t_msg.au8_Data[4] = 0xFF;
    t_msg.au8_Data[5] = 0xFF;
    t_msg.au8_Data[6] = 0xFF;
    t_msg.au8_Data[7] = 0xFF;


    return x_can_obj_send_msg(mpv_MsgObjTx_BM_Buttons, &t_msg);
}
sint16 BM_Buttons_TX_Baudrate(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_BAUD_SETTING,BT_BAUDRATE);
}
sint16 BM_Buttons_TX_Keypad_Address(void)
{

    T_x_can_msg t_msg;
    t_msg.t_Id.u32_Id = t_BM_Buttons.u32_TX_Id;
    t_msg.t_Id.u8_Xtd = TRUE;
    t_msg.u8_Dlc = 8;

    t_msg.au8_Data[0] = (uint8) ((BT_HEADER >> 8) & 0xFF);
    t_msg.au8_Data[1] = (uint8) ((BT_HEADER >> 0) & 0xFF);
    t_msg.au8_Data[2] = (uint8) BT_COMMAND_SRC_ADDRESS;
    t_msg.au8_Data[3] = BT_KEYPAD_ADDRESS;
    t_msg.au8_Data[4] = BT_KEYPAD_ADDRESS;
    t_msg.au8_Data[5] = 0xFF;
    t_msg.au8_Data[6] = 0xFF;
    t_msg.au8_Data[7] = 0xFF;

    return x_can_obj_send_msg(mpv_MsgObjTx_BM_Buttons, &t_msg);
}
sint16 BM_Buttons_TX_Enable_Periodic_Send(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_ENABLE_PERIODIC_TX,t_BM_Buttons.u_Config.bits.b1_periodic_tx);
}
sint16 BM_Buttons_TX_Enable_Event_Transmition(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_ENABLE_EVENT_TX,t_BM_Buttons.u_Config.bits.b1_event_tx);
}
sint16 BM_Buttons_TX_Enable_Led_Ack(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_ENABLE_LED_ACK,t_BM_Buttons.u_Config.bits.b1_led_ack);
}
sint16 BM_Buttons_TX_Enable_Claim(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_ENABLE_ADDR_CLAIM,0);
}
sint16 BM_Buttons_TX_Heartbeat(void)
{
    T_x_can_msg t_msg;
    t_msg.t_Id.u32_Id = t_BM_Buttons.u32_TX_Id;
    t_msg.t_Id.u8_Xtd = TRUE;
    t_msg.u8_Dlc = 8;

    t_msg.au8_Data[0] = (uint8) ((BT_HEADER >> 8) & 0xFF);
    t_msg.au8_Data[1] = (uint8) ((BT_HEADER >> 0) & 0xFF);
    t_msg.au8_Data[2] = (uint8) BT_COMMAND_CONFIG_HEARTBEAT;
    t_msg.au8_Data[3] = t_BM_Buttons.u_Config.bits.b1_heartbeat_tx;
    t_msg.au8_Data[4] = t_BM_Buttons.u8_Heartbeat_Period_10ms;
    t_msg.au8_Data[5] = 0xFF;
    t_msg.au8_Data[6] = 0xFF;
    t_msg.au8_Data[7] = 0xFF;

    return x_can_obj_send_msg(mpv_MsgObjTx_BM_Buttons, &t_msg);
}
sint16 BM_Buttons_TX_Set_Period(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_CONFIG_PERIODIC_TX,t_BM_Buttons.u8_Transmission_Period_10ms);
}
sint16 BM_Buttons_TX_Demo(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_DEMO_MODE,0);
}
sint16 BM_Buttons_TX_Default_Backlight_Brightness(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_STARTUP_BACKLIGHT,BT_DEFAULT_BACKLIGHT_BRIGHTNESS);
}
sint16 BM_Buttons_TX_Default_Led_Brightness(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_STARTUP_BRIGHTNESS,BT_DEFAULT_LED_BRIGHTNESS);
}
sint16 BM_Buttons_TX_Backlight_Color(void)
{
    return BM_Buttons_TX_Std_Msg(BT_COMMAND_BACKLIGHT_COLOR, (uint8) t_BM_Buttons.e_Backlight_Color);
}

/** Recived */
void BM_Buttons_RX_Software_Version(uint8 *au8_data)
{
    t_BM_Buttons.s_Software_Version[0] = (charn) au8_data[3];
    t_BM_Buttons.s_Software_Version[1] = (charn) au8_data[4];
    t_BM_Buttons.s_Software_Version[2] = (charn) au8_data[5];
    t_BM_Buttons.s_Software_Version[3] = (charn) au8_data[6];
    t_BM_Buttons.s_Software_Version[4] = (charn) au8_data[7];
}

void BM_Buttons_RX_Contact_State(uint8 *au8_data)
{
    uint8 u8_index = au8_data[3];
    if(au8_data[4] != 0) t_BM_Buttons.at_Keys[u8_index - 1].e_Key_State = BT_KEY_STATE_ON;
    else t_BM_Buttons.at_Keys[u8_index - 1].e_Key_State = BT_KEY_STATE_OFF;
}

void BM_Buttons_RX_Heartbeat(uint8 *au8_data)
{
    t_BM_Buttons.u8_life = au8_data[3];
    uint8 u8_index;
    uint8 u8_Mask = 0x01;
    for(u8_index = 1; u8_index <= BT_BUTTON_COUNT; u8_index++){
        if((u8_Mask & au8_data[4]) != 0) t_BM_Buttons.at_Keys[u8_index - 1].e_Key_State = BT_KEY_STATE_ON;
        else t_BM_Buttons.at_Keys[u8_index - 1].e_Key_State =BT_KEY_STATE_OFF;
        u8_Mask = u8_Mask << 1;
    }
}

uint8 BM_Buttons_Recived(void){
    sint16 s16_Error_Can;
    uint8 u8_Recived;
    u8_Recived = FALSE;
    T_x_can_msg t_msg;
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_BM_Buttons, &t_msg);
    while(s16_Error_Can == C_NO_ERR){
        if(t_msg.t_Id.u32_Id != t_BM_Buttons.u32_RX_Id ) continue;
        u8_Recived = TRUE;
        switch(t_msg.au8_Data[2]){
            case BT_COMMAND_SOFTWARE_VERSION:
                BM_Buttons_RX_Software_Version(t_msg.au8_Data);
                break;
            case BT_MESSAGE_KEY_STATE:
                BM_Buttons_RX_Contact_State(t_msg.au8_Data);
                break;
            case BT_MESSAGE_HEARTBEAT:
                BM_Buttons_RX_Heartbeat(t_msg.au8_Data);
                break;
            default:
                break;
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_BM_Buttons, &t_msg);
    }
    return u8_Recived;
}
