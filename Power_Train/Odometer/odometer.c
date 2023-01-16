#include "odometer.h"

#define ODOMETER_EEPROM_ADDRESS ((uint32) 1)
#define ODOMETER_DATA_SIZE ((uint32) 4)

static const float32 HOURS_PER_MS = 1.0f / 3600000;

static uint8 u8_Speed_kmh;
static float32 f32_Odometer_km;
static uint32 u32_LastUpdate_ms;
static float64 f64_Buffer_km;

static uint8 u8_IsLoaded = 0;

/* Private function */
static sint16 Odometer_Load(void)
{
    uint32 u32_km = 0;
    uint8 au8_data[ODOMETER_DATA_SIZE];

    sint16 s16_error = x_eep_read(ODOMETER_EEPROM_ADDRESS, ODOMETER_DATA_SIZE, au8_data);
    if (s16_error != C_NO_ERR) return s16_error;

    u32_km |= au8_data[3];
    u32_km <<= 8;
    u32_km |= au8_data[2];
    u32_km <<= 8;
    u32_km |= au8_data[1];
    u32_km <<= 8;
    u32_km |= au8_data[0];

    f32_Odometer_km = 0.1f * u32_km;
    u8_Speed_kmh = 0;
    f64_Buffer_km = f32_Odometer_km;
    u32_LastUpdate_ms = x_sys_get_time_ms();

    u8_IsLoaded = 1;

    return C_NO_ERR;
}

sint16 Odometer_Save(void)
{
    uint32 u32_km = (uint32) (10 * f32_Odometer_km);

    uint8 au8_data[ODOMETER_DATA_SIZE];
    au8_data[0] = (uint8) ((u32_km >> 0) & 0xFF);
    au8_data[1] = (uint8) ((u32_km >> 8) & 0xFF);
    au8_data[2] = (uint8) ((u32_km >> 16) & 0xFF);
    au8_data[3] = (uint8) ((u32_km >> 24) & 0xFF);

    sint16 s16_error_write = x_eep_write(ODOMETER_EEPROM_ADDRESS, ODOMETER_DATA_SIZE, au8_data);
    sint16 s16_error_flush = x_eep_flush();

    if (s16_error_write != C_NO_ERR) return s16_error_write;
    if (s16_error_flush != C_NO_ERR) return s16_error_flush;

    return C_NO_ERR;
}

sint16 Odometer_Get_Km(float32 *pf32_odometer)
{
    sint16 s16_error = C_NO_ERR;
    if (u8_IsLoaded){
        *pf32_odometer = f32_Odometer_km;
    }
    else{
        s16_error = Odometer_Load();
        if (s16_error == C_NO_ERR) *pf32_odometer = f32_Odometer_km;
    }
    return s16_error;
}

void Odometer_Set_Speed(uint8 u8_kmh)
{
    uint32 u32_now = x_sys_get_time_ms();
    float32 f32_avg_speed = (u8_Speed_kmh + u8_kmh) * 0.5f;
    float64 f64_delta_km = (f32_avg_speed * (u32_now - u32_LastUpdate_ms)) * HOURS_PER_MS;
    f64_Buffer_km = f64_Buffer_km + f64_delta_km;
    u8_Speed_kmh = f32_avg_speed;
    u32_LastUpdate_ms = u32_now;

    f32_Odometer_km = f64_Buffer_km;
}


void Odometer_Set_Km(float32 f32_km)
{
    f32_Odometer_km = f32_km;
    f64_Buffer_km = f32_km;
    Odometer_Save();
}
