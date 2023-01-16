#include "OS_Defs.h"
#include "AC_Compressor.h"

T_AC_Comp_Vars t_AC_Comp;

void *mpv_MsgObjTx_AC_Comp; // module global defined TX object for AC_Compressor
//void *mpv_MsgObjRx_AC_Comp; // module global defined RX object for AC_Compressor


// ------- Public --------
void AC_Compressor_Init(uint8 u8_CAN_Chanel){

    t_AC_Comp.t_Feedback.u_Errors.all = AC_COMP_NO_ERROR;
    t_AC_Comp.t_Feedback.e_Status = AC_COMP_OFF;
    t_AC_Comp.t_Feedback.u8_Temp_C = 0;
    t_AC_Comp.t_Feedback.u16_Speed_rpm = 0;
    t_AC_Comp.t_Feedback.u16_Input_Voltage_V = 0;
    t_AC_Comp.t_Feedback.f32_Max_Power_kW = 0;
    t_AC_Comp.t_Feedback.f32_Input_Current_A = 0;

    t_AC_Comp.t_Refs.u8_Enable = FALSE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = 0;

    t_AC_Comp.e_Mq_State = AC_COMP_MQ_SLEEP;
    t_AC_Comp.u8_Mq_ref = FALSE;

    t_AC_Comp.u8_Start_Error = FALSE;

    t_AC_Comp.u8_Counter = 0;
    t_AC_Comp.u32_Timeout = 0;

    T_x_can_obj_config t_CanObjInit;

    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_TX;    // this is a TX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 3u;              // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x238;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    x_can_obj_init(u8_CAN_Chanel, &t_CanObjInit, &mpv_MsgObjTx_AC_Comp);

/*
    t_CanObjInit.u8_MsgType = X_CAN_OBJ_TYPE_RX;    // this is a RX object
    t_CanObjInit.u16_Pause = 0u;                    // TX pause function not used
    t_CanObjInit.u16_SoftBuffer = 0u;               // no software FIFO buffer
    t_CanObjInit.u16_HardBuffer = 3u;               // use no hardware FIFO buffer
    t_CanObjInit.u32_CallbackParameter = 0u;        // no callbackparameter
    t_CanObjInit.pr_Callback = NULL;                // no callback function

    //init a TX object on CAN node1 Error Box
    t_CanObjInit.t_Id.u32_Id = 0x248u;        // TX identifier
    t_CanObjInit.t_Id.u8_Xtd = FALSE;               // use 11bit ID
    x_can_obj_init(u8_CAN_Chanel, &t_CanObjInit, &mpv_MsgObjRx_AC_Comp);
*/

}

void AC_Comp_Set_Enable(uint8 u8_ref){
    t_AC_Comp.u8_Mq_ref = u8_ref;
}
void AC_Comp_Send(void){
    t_AC_Comp.u8_Counter++;
    if(t_AC_Comp.u8_Counter >= AC_COMP_COUNTER_LIMIT){
        t_AC_Comp.u8_Counter = 0;
        AC_Comp_TX_Control();
    }

}

void AC_Comp_Recived(void){

}


void AC_Comp_Feedback_Interface(T_x_can_msg * pt_Msg){
    AC_Comp_RX_Feedback(pt_Msg);
}

void AC_Comp_Monitoring(T_x_can_msg * pt_Msg){
    (*pt_Msg).t_Id.u32_Id = 0x18178814;
    (*pt_Msg).t_Id.u8_Xtd = TRUE;
    (*pt_Msg).u8_Dlc = 8;
    (*pt_Msg).au8_Data[0] = t_AC_Comp.u8_Mq_ref;
    (*pt_Msg).au8_Data[1] = (uint8) t_AC_Comp.e_Mq_State;
    (*pt_Msg).au8_Data[2] = t_AC_Comp.t_Refs.u8_Enable;
    (*pt_Msg).au8_Data[3] = (uint8) (t_AC_Comp.t_Refs.u16_Speed_rpm & 0xFF);
    (*pt_Msg).au8_Data[4] = (uint8) ((t_AC_Comp.t_Refs.u16_Speed_rpm>>8) & 0xFF);
    (*pt_Msg).au8_Data[5] = ((uint8) t_AC_Comp.t_Feedback.e_Status) | (t_AC_Comp.u8_Start_Error << 7);
    (*pt_Msg).au8_Data[6] = (uint8) (t_AC_Comp.t_Feedback.u16_Speed_rpm & 0xFF);
    (*pt_Msg).au8_Data[7] = (uint8) ((t_AC_Comp.t_Feedback.u16_Speed_rpm>>8) & 0xFF);
}

// ------- Private --------

void AC_Comp_RX_Feedback(T_x_can_msg * pt_Msg){
    uint16 u16_aux;
    sint16 s16_aux;
    if((*pt_Msg).t_Id.u32_Id == 0x248){

        u16_aux = (*pt_Msg).au8_Data[1];
        u16_aux = u16_aux << 8;
        u16_aux = u16_aux + (*pt_Msg).au8_Data[0];

        t_AC_Comp.t_Feedback.u16_Speed_rpm = u16_aux;
        t_AC_Comp.t_Feedback.f32_Max_Power_kW = (*pt_Msg).au8_Data[2] * 0.1f;
        t_AC_Comp.t_Feedback.e_Status = (*pt_Msg).au8_Data[3];
        t_AC_Comp.t_Feedback.u_Errors.all = (*pt_Msg).au8_Data[4];

        s16_aux = (*pt_Msg).au8_Data[5];
        t_AC_Comp.t_Feedback.u8_Temp_C = s16_aux - 40;

        t_AC_Comp.t_Feedback.f32_Input_Current_A = (*pt_Msg).au8_Data[6] * 0.2f;
        u16_aux = (*pt_Msg).au8_Data[7];
        t_AC_Comp.t_Feedback.u16_Input_Voltage_V = u16_aux * 4;
    }
}
void AC_Comp_TX_Control(void){
    T_x_can_msg t_Msg;
    t_Msg.t_Id.u32_Id = 0x238;
    t_Msg.t_Id.u8_Xtd = FALSE;
    t_Msg.u8_Dlc = 8;

    t_Msg.au8_Data[0] = (uint8) (t_AC_Comp.t_Refs.u16_Speed_rpm & 0xFF);
    t_Msg.au8_Data[1] = (uint8) ((t_AC_Comp.t_Refs.u16_Speed_rpm>>8) & 0xFF);
    t_Msg.au8_Data[2] = 0;
    t_Msg.au8_Data[3] = t_AC_Comp.t_Refs.u8_Enable & 0xFF;
    t_Msg.au8_Data[4] = 0;
    t_Msg.au8_Data[5] = 0;
    t_Msg.au8_Data[6] = 0;
    t_Msg.au8_Data[7] = 0;

    x_can_obj_send_msg(mpv_MsgObjTx_AC_Comp, &t_Msg);
}

void AC_Comp_Mq_Sleep(void){
    t_AC_Comp.t_Refs.u8_Enable = FALSE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = 0;

    if(t_AC_Comp.u8_Mq_ref){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_WAIT;
        t_AC_Comp.u32_Timeout = Timeout_Set_ms(300);
    }
}

void AC_Comp_Mq_Wait(void){
    t_AC_Comp.t_Refs.u8_Enable = TRUE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = 0;

    if(Timeout_Check(t_AC_Comp.u32_Timeout)){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_START;
        t_AC_Comp.u32_Timeout = Timeout_Set_ms(60000);
    }
}

void AC_Comp_Mq_Start(void){
    t_AC_Comp.t_Refs.u8_Enable = TRUE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = AC_COMP_NOMINAL_SPEED_RPM;

    if(t_AC_Comp.t_Feedback.u16_Speed_rpm > AC_COMP_MINIMUN_SPEED_RPM){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_RUN;
        t_AC_Comp.u8_Start_Error = FALSE;
    }

    if(Timeout_Check(t_AC_Comp.u32_Timeout)){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_FAULT;
        t_AC_Comp.u8_Start_Error = TRUE;
        t_AC_Comp.u32_Timeout = Timeout_Set_ms(5000);
    }
}
void AC_Comp_Mq_Run(void){
    t_AC_Comp.t_Refs.u8_Enable = TRUE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = AC_COMP_NOMINAL_SPEED_RPM;

    if(t_AC_Comp.u8_Mq_ref == FALSE){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_STOP;
    }

    if(t_AC_Comp.t_Feedback.u_Errors.all != 0){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_FAULT;
    }
}
void AC_Comp_Mq_Stop(void){
    t_AC_Comp.t_Refs.u8_Enable = TRUE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = 0;

    if(t_AC_Comp.t_Feedback.u16_Speed_rpm < 3500){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_SLEEP;
    }
}
void AC_Comp_Mq_Fault(void){
    t_AC_Comp.t_Refs.u8_Enable = FALSE;
    t_AC_Comp.t_Refs.u16_Speed_rpm = 0;

    if(t_AC_Comp.u8_Start_Error && (!Timeout_Check(t_AC_Comp.u32_Timeout))){
        return;
    }

    t_AC_Comp.u8_Start_Error = FALSE;

    if(t_AC_Comp.t_Feedback.u_Errors.all == 0){
        t_AC_Comp.e_Mq_State = AC_COMP_MQ_SLEEP;
    }
}

void AC_Comp_Run(void){
    switch(t_AC_Comp.e_Mq_State){
        case AC_COMP_MQ_SLEEP:
            AC_Comp_Mq_Sleep();
            break;
        case AC_COMP_MQ_WAIT:
            AC_Comp_Mq_Wait();
            break;
        case AC_COMP_MQ_START:
            AC_Comp_Mq_Start();
            break;
        case AC_COMP_MQ_RUN:
            AC_Comp_Mq_Run();
            break;
        case AC_COMP_MQ_STOP:
            AC_Comp_Mq_Stop();
            break;
        case AC_COMP_MQ_FAULT:
            AC_Comp_Mq_Fault();
            break;
    }
    AC_Comp_Send();
}
