#include "OS_Defs.h"
#include "Service_Air.h"

T_Service_Air_Vars t_Service_Air;

// ------- Private --------

void Service_Air_Mq_Sleep(void){
    t_Service_Air.t_Refs.u8_Enable = FALSE;
    t_Service_Air.t_Refs.u16_Speed_rpm = 0;

    if(t_Service_Air.u8_Mq_ref){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_WAIT;
        t_Service_Air.u32_Timeout = Timeout_Set_ms(300);
    }
}
void Service_Air_Mq_Wait(void){
    t_Service_Air.t_Refs.u8_Enable = TRUE;
    t_Service_Air.t_Refs.u16_Speed_rpm = 0;

    if(Timeout_Check(t_Service_Air.u32_Timeout)){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_START;
        t_Service_Air.u32_Timeout = Timeout_Set_ms(10000);
    }
}

void Service_Air_Mq_Start(void){
    t_Service_Air.t_Refs.u8_Enable = TRUE;
    t_Service_Air.t_Refs.u16_Speed_rpm = SERVICE_AIR_NOMINAL_SPEED_RPM;

    if(t_Service_Air.t_Feedback.u16_Speed_rpm > SERVICE_AIR_MINIMUN_SPEED_RPM){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_RUN;
        t_Service_Air.u8_Start_Error = FALSE;
    }

    if(Timeout_Check(t_Service_Air.u32_Timeout)){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_FAULT;
        t_Service_Air.u8_Start_Error = TRUE;
        t_Service_Air.u32_Timeout = Timeout_Set_ms(5000);
    }
}
void Service_Air_Mq_Run(void){
    t_Service_Air.t_Refs.u8_Enable = TRUE;
    t_Service_Air.t_Refs.u16_Speed_rpm = SERVICE_AIR_NOMINAL_SPEED_RPM;

    if(t_Service_Air.u8_Mq_ref == FALSE){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_STOP;
    }

    if(t_Service_Air.t_Feedback.e_Status == SERVICE_AIR_ERROR){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_FAULT;
    }
}
void Service_Air_Mq_Stop(void){
    t_Service_Air.t_Refs.u8_Enable = TRUE;
    t_Service_Air.t_Refs.u16_Speed_rpm = 0;

    if(t_Service_Air.t_Feedback.u16_Speed_rpm < 200){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_SLEEP;
    }
}
void Service_Air_Mq_Fault(void){
    t_Service_Air.t_Refs.u8_Enable = FALSE;
    t_Service_Air.t_Refs.u16_Speed_rpm = 0;

    if(t_Service_Air.u8_Start_Error && (!Timeout_Check(t_Service_Air.u32_Timeout))){
        return;
    }

    t_Service_Air.u8_Start_Error = FALSE;

    if(t_Service_Air.t_Feedback.e_Status != SERVICE_AIR_ERROR){
        t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_SLEEP;
    }
}


// ------- Public --------
void Service_Air_Init(void){
    t_Service_Air.t_Refs.u8_Enable = FALSE;
    t_Service_Air.t_Refs.u16_Speed_rpm = 0;

    t_Service_Air.e_Mq_State = SERVICE_AIR_MQ_SLEEP;
    t_Service_Air.u8_Mq_ref = FALSE;

    t_Service_Air.t_Feedback.e_Status = SERVICE_AIR_OFF;
    t_Service_Air.t_Feedback.u16_Speed_rpm = 0;

    t_Service_Air.u32_Timeout = 0;

}

void Service_Air_Set_Ref(uint8 u8_ref){
    t_Service_Air.u8_Mq_ref = u8_ref;
}

void Service_Air_Monitoring(T_x_can_msg * pt_Msg){
    (*pt_Msg).t_Id.u32_Id = 0x18168814;
    (*pt_Msg).t_Id.u8_Xtd = TRUE;
    (*pt_Msg).u8_Dlc = 8;
    (*pt_Msg).au8_Data[0] = t_Service_Air.u8_Mq_ref;
    (*pt_Msg).au8_Data[1] = (uint8) t_Service_Air.e_Mq_State;
    (*pt_Msg).au8_Data[2] = t_Service_Air.t_Refs.u8_Enable;
    (*pt_Msg).au8_Data[3] = (uint8) (t_Service_Air.t_Refs.u16_Speed_rpm & 0xFF);
    (*pt_Msg).au8_Data[4] = (uint8) ((t_Service_Air.t_Refs.u16_Speed_rpm>>8) & 0xFF);
    (*pt_Msg).au8_Data[5] = ((uint8) t_Service_Air.t_Feedback.e_Status) | (t_Service_Air.u8_Start_Error << 7);
    (*pt_Msg).au8_Data[6] = (uint8) (t_Service_Air.t_Feedback.u16_Speed_rpm & 0xFF);
    (*pt_Msg).au8_Data[7] = (uint8) ((t_Service_Air.t_Feedback.u16_Speed_rpm>>8) & 0xFF);
}

void Service_Air_set_Feedback(uint16 u16_Speed_rpm, E_Service_Air_Status e_Status){
    t_Service_Air.t_Feedback.e_Status = e_Status;
    t_Service_Air.t_Feedback.u16_Speed_rpm = u16_Speed_rpm;
}

uint8 Service_Air_Get_Enable(void){
    return t_Service_Air.t_Refs.u8_Enable;
}

uint16 Service_Air_Get_Speed(void){
    return t_Service_Air.t_Refs.u16_Speed_rpm;
}

void Service_Air_Run(void){
    switch(t_Service_Air.e_Mq_State){
        case SERVICE_AIR_MQ_SLEEP:
            Service_Air_Mq_Sleep();
            break;
        case SERVICE_AIR_MQ_WAIT:
            Service_Air_Mq_Wait();
            break;
        case SERVICE_AIR_MQ_START:
            Service_Air_Mq_Start();
            break;
        case SERVICE_AIR_MQ_RUN:
            Service_Air_Mq_Run();
            break;
        case SERVICE_AIR_MQ_STOP:
            Service_Air_Mq_Stop();
            break;
        case SERVICE_AIR_MQ_FAULT:
            Service_Air_Mq_Fault();
            break;
    }

}
