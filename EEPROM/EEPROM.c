#include "OS_Defs.h"

St_EEPROM st_EEPROM;

void *mpv_MsgObjTx_EEPROM;
void *mpv_MsgObjRx_EEPROM;

void EEPROM_Ack(T_x_can_msg * pt_Msg){
    switch (pt_Msg->au8_Data[7]){
        case EEPROM_HVIL_ID:
            EEPROM_Read(HVIL_EEPROM_ADDRESS, HVIL_DATA_SIZE, pt_Msg->au8_Data);
            pt_Msg->t_Id.u32_Id = 0x18021516;
            x_can_obj_send_msg(mpv_MsgObjTx_EEPROM, pt_Msg);
            break;
        case EEPROM_AC_TYPE_ID:
            EEPROM_Read(AC_TYPE_EEPROM_ADDRESS, AC_TYPE_DATA_SIZE, pt_Msg->au8_Data);
            pt_Msg->t_Id.u32_Id = 0x18021516;
            x_can_obj_send_msg(mpv_MsgObjTx_EEPROM, pt_Msg);
            break;
    }
}

void EEPROM_Decode(T_x_can_msg * pt_Msg){
    switch (pt_Msg->au8_Data[7]){
        case EEPROM_HVIL_ID:
            EEPROM_Write(HVIL_EEPROM_ADDRESS, HVIL_DATA_SIZE, pt_Msg->au8_Data);
            break;
        case EEPROM_AC_TYPE_ID:
            EEPROM_Write(AC_TYPE_EEPROM_ADDRESS, AC_TYPE_DATA_SIZE, pt_Msg->au8_Data);
            break;
    }
    EEPROM_Ack(pt_Msg);
}

void EEPROM_Init(uint8 u8_CAN_Chanel){

    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 3u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00000016;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(u8_CAN_Chanel, &t_CanObjInit, &mpv_MsgObjTx_EEPROM);

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 3u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x00000015;          // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;               // use 11bit ID
    x_can_obj_init(u8_CAN_Chanel, &t_CanObjInit, &mpv_MsgObjRx_EEPROM);


    EEPROM_get_HVIL_Enable();
    EEPROM_get_AC_Type();

}

void EEPROM_Rx(void)
{
    sint16 s16_Error_Can;
    T_x_can_msg t_msg;
    s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_EEPROM, &t_msg);
    while(s16_Error_Can == C_NO_ERR){
        switch(t_msg.t_Id.u32_Id){
            case 0x18001615:
                EEPROM_Decode(&t_msg);
                break;
            case 0x18011615:
                EEPROM_Ack(&t_msg);
                break;
        }
        s16_Error_Can = x_can_obj_get_msg(mpv_MsgObjRx_EEPROM, &t_msg);
    }
}

sint16 EEPROM_Write( const uint32 ou32_Address, const uint32 ou32_Count,  uint8 * const opu8_Data)
{
    sint16 s16_error_write = x_eep_write(ou32_Address, ou32_Count, opu8_Data);
    sint16 s16_error_flush = x_eep_flush();

    if (s16_error_write != C_NO_ERR) return s16_error_write;
    if (s16_error_flush != C_NO_ERR) return s16_error_flush;

    return C_NO_ERR;
}

sint16 EEPROM_Read(const uint32 ou32_Address, const uint32 ou32_Count,  uint8 * const opu8_Data)
{
    sint16 s16_error = x_eep_read(ou32_Address, ou32_Count, opu8_Data);
    if (s16_error != C_NO_ERR) return s16_error;

    return C_NO_ERR;
}

uint8 EEPROM_get_HVIL_Enable(void)
{
    uint8 au8_data[HVIL_DATA_SIZE];

    EEPROM_Read(HVIL_EEPROM_ADDRESS, HVIL_DATA_SIZE, au8_data);

    st_EEPROM.u8_EEPROM_HVIL_Enable = au8_data[0];

    return st_EEPROM.u8_EEPROM_HVIL_Enable;
}

uint8 EEPROM_get_AC_Type(void)
{
    uint8 au8_data[AC_TYPE_DATA_SIZE];

    EEPROM_Read(AC_TYPE_EEPROM_ADDRESS, AC_TYPE_DATA_SIZE, au8_data);

    st_EEPROM.u8_EEPROM_AC_Type = au8_data[0];

    return st_EEPROM.u8_EEPROM_AC_Type;
}
