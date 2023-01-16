#include "Os_Defs.h"
#include "Buttons_Int.h"
#include "BM_Buttons.h"

T_Buttons_Vars t_Buttons;

void Buttons_Init(void){
    t_Buttons.u_Flags.all = 0;
    t_Buttons.u8_Reset_State = 0;
    t_Buttons.u8_Config_Complete = FALSE;
    BM_Buttons_Init();
}

uint8 Buttons_Reset(void){
    sint16 s16_Error;
    uint8 u8_count;
    uint8 u8_state = t_Buttons.u8_Reset_State;

    T_Key_Vars t_Key;
    if(t_Buttons.u8_Config_Complete){
        u8_state = 2;
    }

    switch(u8_state){
        case 0:
            t_Buttons.u8_Config_Complete = FALSE;
            if(BM_Buttons_General_Config()){
                u8_state = u8_state + 1;
            }
            break;
        case 1:
            t_Key.e_Color_1 = BT_LED_COLOR_YELLOW_GREEN;
            t_Key.e_Color_2 = BT_LED_COLOR_OFF;
            t_Key.e_Key_Enable = BT_KEY_DISABLED;
            t_Key.e_Key_State = BT_KEY_STATE_OFF;
            t_Key.e_LED_State = BT_LED_STATE_OFF;

            for (u8_count = 1; u8_count <= BT_BUTTON_COUNT; u8_count++)
            {
                BM_Buttons_Config_Key(u8_count, &t_Key);
            }

            t_Key.e_Color_1 = BT_LED_COLOR_CYAN;
            t_Key.e_Key_Enable = BT_KEY_ENABLED;
            BM_Buttons_Config_Key(BUTTON_DIRECT, &t_Key);
            BM_Buttons_Config_Key(BUTTON_NEUTRAL, &t_Key);
            BM_Buttons_Config_Key(BUTTON_REVERSE, &t_Key);

            t_Key.e_Color_1 = BT_LED_COLOR_RED;
            BM_Buttons_Config_Key(BUTTON_CHANGE_PAGE, &t_Key);

            for (u8_count = 1; u8_count <= BT_BUTTON_COUNT; u8_count++)
            {
                s16_Error = BM_Buttons_Key_Send_Config(u8_count);
                if(s16_Error != C_NO_ERR){
                    return FALSE;
                }
            }
            u8_state = u8_state + 1;
            break;
        case 2:
            t_Buttons.u8_Reset_State = 0;
            t_Buttons.u8_Config_Complete = TRUE;
            return TRUE;
        default:
            t_Buttons.u8_Config_Complete = FALSE;
            u8_state = 0;

    }
    t_Buttons.u8_Reset_State = u8_state;
    return FALSE;
}

uint8 Buttons_Update(void){
    if(!BM_Buttons_Recived()){
        return 0;
    }
    Buttons_Handle_Change_Page(BM_Buttons_Is_On(BUTTON_CHANGE_PAGE));
    uint8 u8_direct = BM_Buttons_Is_On(BUTTON_DIRECT);
    uint8 u8_neutral = BM_Buttons_Is_On(BUTTON_NEUTRAL);
    uint8 u8_reverse = BM_Buttons_Is_On(BUTTON_REVERSE);

    if(( u8_direct + u8_neutral + u8_reverse) > 1 ){
        return 0;
    }
    if(( u8_direct + u8_neutral + u8_reverse) == 0 ){
        return 0;
    }
    if(u8_direct){
        BM_Buttons_Set_Led_Blink(BUTTON_DIRECT);
        BM_Buttons_Set_Led_Off(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_Off(BUTTON_REVERSE);

        t_Buttons.u_Flags.bits.b2_Direct = DS_ON;
        t_Buttons.u_Flags.bits.b2_Neutral = DS_OFF;
        t_Buttons.u_Flags.bits.b2_Reverse = DS_OFF;
    }
    else if(u8_neutral){
        BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
        BM_Buttons_Set_Led_Blink(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_Off(BUTTON_REVERSE);

        t_Buttons.u_Flags.bits.b2_Direct = DS_OFF;
        t_Buttons.u_Flags.bits.b2_Neutral = DS_ON;
        t_Buttons.u_Flags.bits.b2_Reverse = DS_OFF;
    }
    else if(u8_reverse){
        BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
        BM_Buttons_Set_Led_Off(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_Blink(BUTTON_REVERSE);

        t_Buttons.u_Flags.bits.b2_Direct = DS_OFF;
        t_Buttons.u_Flags.bits.b2_Neutral = DS_OFF;
        t_Buttons.u_Flags.bits.b2_Reverse = DS_ON;
    }
    return 1;
}

void Buttons_Clear_Gear(void){
    t_Buttons.u_Flags.bits.b2_Direct = DS_OFF;
    t_Buttons.u_Flags.bits.b2_Neutral = DS_OFF;
    t_Buttons.u_Flags.bits.b2_Reverse = DS_OFF;
    BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
    BM_Buttons_Set_Led_Off(BUTTON_NEUTRAL);
    BM_Buttons_Set_Led_Off(BUTTON_REVERSE);
}

void Buttons_Set_Neutral(void){
    t_Buttons.u_Flags.bits.b2_Direct = DS_OFF;
    t_Buttons.u_Flags.bits.b2_Neutral = DS_ON;
    t_Buttons.u_Flags.bits.b2_Reverse = DS_OFF;
    BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
    BM_Buttons_Set_Led_On(BUTTON_NEUTRAL);
    BM_Buttons_Set_Led_Off(BUTTON_REVERSE);
}

void Buttons_Gear_Fix_Led(void){
    if( t_Buttons.u_Flags.bits.b2_Direct == DS_ON){
        BM_Buttons_Set_Led_On(BUTTON_DIRECT);
        BM_Buttons_Set_Led_Off(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_Off(BUTTON_REVERSE);
    }
    else if( t_Buttons.u_Flags.bits.b2_Neutral == DS_ON){
        BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
        BM_Buttons_Set_Led_On(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_Off(BUTTON_REVERSE);
    }
    else if( t_Buttons.u_Flags.bits.b2_Reverse == DS_ON){
        BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
        BM_Buttons_Set_Led_Off(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_On(BUTTON_REVERSE);
    }
    else{
        BM_Buttons_Set_Led_Off(BUTTON_DIRECT);
        BM_Buttons_Set_Led_Off(BUTTON_NEUTRAL);
        BM_Buttons_Set_Led_Off(BUTTON_REVERSE);
    }
}

sint8 Buttons_Get_Gear(void){
    if( t_Buttons.u_Flags.bits.b2_Direct == DS_ON){
        return 1;
    }
    else if( t_Buttons.u_Flags.bits.b2_Neutral == DS_ON){
        return 0;
    }
    else if( t_Buttons.u_Flags.bits.b2_Reverse == DS_ON){
        return -1;
    }
    else{
        return 0;
    }

}

void Buttons_Handle_Change_Page(uint8 u8_change_page){
    if(u8_change_page){
        t_Buttons.u_Flags.bits.b2_Change_Page = DS_ON;
    }

}

uint8 Buttons_Get_Change_Page(void){
    uint8 u8_res = t_Buttons.u_Flags.bits.b2_Change_Page & 0x1;
    t_Buttons.u_Flags.bits.b2_Change_Page = DS_OFF;
    return u8_res;
}
