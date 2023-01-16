#include "OS_Defs.h"
#include "Steering.h"

T_Steering_Vars t_Steering;
T_x_os_mutex t_Mutex_Steering;

uint8 u8_Steering_Msg_Counter;

void *mpv_MsgObjTx_Steering;
void *mpv_MsgObjRx_Steering;


void Steering_Init(void){
    sint16 s16_Error;

    t_Steering.u_Errors.all = 0;
    t_Steering.u8_State = STR_STATE_OFF;
    t_Steering.u8_On = 0;
    t_Steering.u8_T15_Ref = 0;
    t_Steering.u8_Contactor_Ref = 0;
    t_Steering.u32_Timeout_ms = 0;
    t_Steering.u16_Speed_kmh = 0;

    u8_Steering_Msg_Counter = 0;
    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 10u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;        // no callback function

    // init a RX object on CAN node1
    t_CanObjInit.t_Id.u32_Id = STEERING_TX_MASK;  // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;
    s16_Error = x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjTx_Steering);
    if(s16_Error != C_NO_ERR) Error_Beep();

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_HardBuffer = 5u;               // use no hardware FIFO buffer

    // init a RX object on CAN node1
    t_CanObjInit.t_Id.u32_Id = STEERING_RX_MASK;  // RX identifier
    t_CanObjInit.t_Id.u8_Xtd = TRUE;
    s16_Error = x_can_obj_init(X_CAN_BUS_01, &t_CanObjInit, &mpv_MsgObjRx_Steering);
    if(s16_Error != C_NO_ERR) Error_Beep();

    s16_Error = x_os_mutex_init(&t_Mutex_Steering);
    if(s16_Error != C_NO_ERR) Error_Beep();

}

void Steering_Set_Vehicle_Speed_kmh(sint16 s16_Speed_kmh){
    uint16 u16_Speed_kmh;

    if(s16_Speed_kmh >= 0) u16_Speed_kmh = s16_Speed_kmh;
    else u16_Speed_kmh = -s16_Speed_kmh;

    if(x_os_mutex_get(&t_Mutex_Steering,10) == C_NO_ERR){
        t_Steering.u16_Speed_kmh = u16_Speed_kmh;
        x_os_mutex_release(&t_Mutex_Steering);
    }
}

sint16 Steering_Send_Vehicle_Speed(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x18FEF121u;
    t_Msg.t_Id.u8_Xtd = TRUE;
    t_Msg.u8_Dlc = 8;

    uint16 u16_Speed = t_Steering.u16_Speed_kmh * 256;

    t_Msg.au8_Data[0] = 0;
    t_Msg.au8_Data[1] = (uint8) (u16_Speed & 0xFF);
    t_Msg.au8_Data[2] = (uint8) ((u16_Speed >> 8) & 0xFF);
    t_Msg.au8_Data[3] = 0;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    return x_can_obj_send_msg(mpv_MsgObjTx_Steering, &t_Msg);
}

void Steering_Send_Msg(void){
    if(t_Steering.u8_State == STR_STATE_OFF) return;
    uint16 u16_Send_Error;
    u8_Steering_Msg_Counter++;
    if(u8_Steering_Msg_Counter >= 11){
        u16_Send_Error = Steering_Send_Vehicle_Speed();
        u8_Steering_Msg_Counter = 0;
        if(u16_Send_Error != C_NO_ERR) t_Steering.u_Errors.bits.b2_CAN_Send = 0x3;
        else t_Steering.u_Errors.bits.b2_CAN_Send = 0x0;
    }
}

void Steering_State_Off(void){
    GPIO_set_T15_Steering(DS_OFF);
    GPIO_set_Steering_Contactor(DS_OFF);
    if(t_Steering.u8_On == DS_ON){
        t_Steering.u32_Timeout_ms = Timeout_Set_ms(1000);
        t_Steering.u8_State = STR_STATE_ENABLE;
    }
}
void Steering_State_Enable(void){
    GPIO_set_T15_Steering(DS_ON);
    GPIO_set_Steering_Contactor(DS_OFF);
    if(t_Steering.u8_On == DS_ON){
        if(Timeout_Check(t_Steering.u32_Timeout_ms)){
            t_Steering.u32_Timeout_ms = Timeout_Set_ms(1000);
            t_Steering.u8_State = STR_STATE_CLOSE;
        }
    }else{
        t_Steering.u8_State = STR_STATE_OFF;
    }
}
void Steering_State_Close(void){
    GPIO_set_T15_Steering(DS_ON);
    GPIO_set_Steering_Contactor(DS_ON);
    if(t_Steering.u8_On == DS_ON){
        if(Timeout_Check(t_Steering.u32_Timeout_ms)){
            t_Steering.u8_State = STR_STATE_OPERATION;
        }
    }else{
        t_Steering.u8_State = STR_STATE_DISABLE;
        t_Steering.u32_Timeout_ms = Timeout_Set_ms(1000);
    }
}
void Steering_State_Operation(void){
    GPIO_set_T15_Steering(DS_ON);
    GPIO_set_Steering_Contactor(DS_ON);
    if(t_Steering.u8_On == DS_OFF){
        t_Steering.u32_Timeout_ms = Timeout_Set_ms(1000);
        t_Steering.u8_State = STR_STATE_DISABLE;
    }
}
void Steering_State_Disable(void){
    GPIO_set_T15_Steering(DS_OFF);
    GPIO_set_Steering_Contactor(DS_ON);
    if(Timeout_Check(t_Steering.u32_Timeout_ms)){
        t_Steering.u8_State = STR_STATE_OFF;
    }
}

void Steering_State_Fault(void){

}

void Steering_Machine(void){
    switch(t_Steering.u8_State){
        case STR_STATE_OFF:
            Steering_State_Off();
            break;
        case STR_STATE_ENABLE:
            Steering_State_Enable();
            break;
        case STR_STATE_CLOSE:
            Steering_State_Close();
            break;
        case STR_STATE_OPERATION:
            Steering_State_Operation();
            break;
        case STR_STATE_DISABLE:
            Steering_State_Disable();
            break;
        case STR_STATE_FAULT:
            Steering_State_Fault();
            break;
    }
}

inline void Steering_set_On(void){
    t_Steering.u8_On = DS_ON;
}

inline void Steering_set_Off(void){
    t_Steering.u8_On = DS_OFF;
}

uint8 Steering_is_On(void){
    return (t_Steering.u8_State == STR_STATE_OPERATION);
}

void Steering_Config_Monitoring_msg(T_x_can_msg * t_Msg){
    (*t_Msg).u8_Dlc = 8;
    (*t_Msg).au8_Data[0] = t_Steering.u8_State;
    (*t_Msg).au8_Data[1] = t_Steering.u8_On;
    (*t_Msg).au8_Data[2] = t_Steering.u8_T15_Ref;
    (*t_Msg).au8_Data[3] = t_Steering.u8_Contactor_Ref;
    (*t_Msg).au8_Data[4] = (uint8) (t_Steering.u16_Speed_kmh & 0xFF);
    (*t_Msg).au8_Data[5] = (uint8) ((t_Steering.u16_Speed_kmh >> 8) & 0xFF);
    (*t_Msg).au8_Data[6] = t_Steering.u_Errors.all;
    (*t_Msg).au8_Data[7] = 0;
}
