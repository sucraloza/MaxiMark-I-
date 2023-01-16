#ifndef BM_Buttons_H_INCLUDED
#define BM_Buttons_H_INCLUDED

#include <x_stdtypes.h>

#define BT_HEADER  			0x041B
#define BT_BAUDRATE			0x3 	//0x2: 500kbps, 0x3: 250kbps
#define BT_BUTTON_COUNT  	5u
#define BT_KEYPAD_ADDRESS  	0x21
#define BT_ECU_ADDRESS  	0x1A

#define BT_DEFAULT_LED_BRIGHTNESS 		0x3F
#define BT_DEFAULT_BACKLIGHT_BRIGHTNESS 0x1E



typedef enum
{
    BT_CONFIG_STATE_ECU_ADDRESS = 0x0,
    BT_CONFIG_STATE_BOUDRATE = 0x1,
    BT_CONFIG_STATE_KEYPDA_ADDRESS = 0x2,
    BT_CONFIG_STATE_HEARTBEAT_PERIOD = 0x3,
    BT_CONFIG_STATE_SEND_PERIOD = 0x4,
    BT_CONFIG_STATE_HEARTBEAT_ENABLE = 0x5,
    BT_CONFIG_STATE_EVENT_ENABLE = 0x6,
    BT_CONFIG_STATE_SHOW_ENABLE = 0x7,
    BT_CONFIG_STATE_LED_BRHIGHTNESS = 0x8,
    BT_CONFIG_STATE_BALCKLIGHT_BRHIGHTNESS = 0x9,
} E_Config_States;

typedef enum
{
    BT_COMMAND_LED = 0x1,
    BT_COMMAND_LED_BRIGHTNESS = 0x2,
    BT_COMMAND_BACKLIGHT_LEVEL = 0x3,
    BT_COMMAND_STARTUP_MESSAGE = 0x28,
    BT_COMMAND_SOFTWARE_VERSION = 0x2A,
    BT_COMMAND_LED_SHOW = 0x34,
	BT_COMMAND_DESTINATION_ADDRESS = 0x6E,
    BT_COMMAND_BAUD_SETTING = 0x6F,
    BT_COMMAND_SRC_ADDRESS = 0x70,
    BT_COMMAND_ENABLE_PERIODIC_TX = 0x71,
    BT_COMMAND_ENABLE_EVENT_TX = 0x72,
    BT_COMMAND_ENABLE_LED_ACK = 0x73,
    BT_COMMAND_ENABLE_ADDR_CLAIM = 0x74,
    BT_COMMAND_CONFIG_HEARTBEAT = 0x75,
    BT_COMMAND_CONFIG_PERIODIC_TX = 0x77,
    BT_COMMAND_DEMO_MODE = 0x7A,
    BT_COMMAND_STARTUP_BACKLIGHT = 0x7B,
    BT_COMMAND_STARTUP_BRIGHTNESS = 0x7C,
    BT_COMMAND_BACKLIGHT_COLOR = 0x7D,
    BT_MESSAGE_HEARTBEAT = 0xF9,
    BT_MESSAGE_KEY_STATE = 0x01
} E_Button_Messages;

typedef enum
{
	BT_LED_COLOR_OFF = 0x0,
	BT_LED_COLOR_RED,
	BT_LED_COLOR_GREEN,
	BT_LED_COLOR_BLUE,
	BT_LED_COLOR_YELLOW,
	BT_LED_COLOR_CYAN,
	BT_LED_COLOR_MAGENTA,
	BT_LED_COLOR_WHITE,
	BT_LED_COLOR_AMBER,
	BT_LED_COLOR_YELLOW_GREEN
} E_Button_LED_Color;

typedef enum
{
	BT_LED_STATE_OFF = 0x0,
	BT_LED_STATE_ON,
	BT_LED_STATE_BLINK,
	BT_LED_STATE_ALTERNATE
} E_Button_LED_State;

typedef enum
{
	BT_LED_SHOW_OFF = 0x0,
	BT_LED_SHOW_COMPLETE,
	BT_LED_SHOW_FAST
} E_Button_Show;

typedef enum
{
	BT_KEY_DISABLED,
	BT_KEY_ENABLED
} E_Key_Enable;

typedef enum
{
	BT_KEY_STATE_OFF,
	BT_KEY_STATE_ON
} E_Key_State;

typedef struct
{
	uint8 u8_Index;
	E_Button_LED_Color e_Color_1;
	E_Button_LED_Color e_Color_2;
	E_Button_LED_State e_LED_State;
	E_Key_Enable e_Key_Enable;
	E_Key_State e_Key_State;
} T_Key_Vars;

typedef struct
{
	uint8 b1_heartbeat_tx:1;
	uint8 b1_periodic_tx:1;
	uint8 b1_event_tx:1;
	uint8 b1_address_claim:1;
	uint8 b1_led_ack:1;
	uint8 b1_startup_msg:1;
	uint8 rsv:2;
} T_Button_Config;

typedef union
{
	uint8 all;
	T_Button_Config bits;
} U_Button_Config;

typedef struct
{
	uint32 u32_TX_Id;
	uint32 u32_RX_Id;
    uint8 u8_Config_Turn;
	T_Key_Vars at_Keys[BT_BUTTON_COUNT];
	U_Button_Config u_Config;
	uint8 u8_Transmission_Period_10ms;
	uint8 u8_Heartbeat_Period_10ms;
	E_Button_Show e_Show;

	float32 f32_Led_Brightness;
	float32 f32_Backlight_Brightness;
	E_Button_LED_Color e_Backlight_Color;
	charn s_Software_Version[5];

	uint8 u8_life;

} T_BM_Buttons_Vars;

extern void BM_Buttons_Init(void);
extern sint16 BM_Buttons_General_Config(void);
extern sint16 BM_Buttons_Key_Send_Config(uint8 u8_index);

/*Setters Declaration*/
extern void BM_Buttons_Config_Key(uint8 u8_index, T_Key_Vars *pt_key);
extern void BM_Buttons_Set_Key_Disable(uint8 u8_index);
extern void BM_Buttons_Set_Key_Disable(uint8 u8_index);
extern void BM_Buttons_Set_Key_Enabled(uint8 u8_index);
extern void BM_Buttons_Set_Led_Off(uint8 u8_index);
extern void BM_Buttons_Set_Led_On(uint8 u8_index);
extern void BM_Buttons_Set_Led_Blink(uint8 u8_index);
extern void BM_Buttons_Set_Led_AltBlink(uint8 u8_index);
extern void BM_Buttons_Set_Led_AltBlink(uint8 u8_index);
extern void BM_Buttons_Set_Led_Collor(uint8 u8_index, E_Button_LED_Color e_color);
extern void BM_Buttons_Set_Led_Collor_Alt(uint8 u8_index, E_Button_LED_Color e_color);
extern void BM_Buttons_Set_Backlight_Collor(E_Button_LED_Color e_color);
extern void BM_Buttons_Set_Led_Brightness(float32 f32_level);
extern void BM_Buttons_Set_Backlight_Brightness(float32 f32_level);
extern void BM_Buttons_Set_Heartbeat(uint8 u8_period_10ms);
extern void BM_Buttons_Set_Period(uint8 u8_period_10ms);
extern void BM_Buttons_Set_OnEvent_Send(uint8 u8_ref);

/* Getters Declaration */
extern void BM_Buttons_Get_Key(uint8 u8_index, T_Key_Vars *pt_key);
extern E_Key_State BM_Buttons_Get_Key_State(uint8 u8_index);
extern uint8 BM_Buttons_Is_On(uint8 u8_index);
extern float32 BM_Buttons_Get_Led_Brightness(void);
extern float32 BM_Buttons_Get_Backlight_Brightness(void);

/* Reciver */
extern uint8 BM_Buttons_Recived(void);




#endif /* BM_Buttons_H_INCLUDED */
