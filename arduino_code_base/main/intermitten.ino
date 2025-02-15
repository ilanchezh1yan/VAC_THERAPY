#define DAC_1 25
#define RX_BFR_SIZE 9

void intermitten_mode(void)
{
    int16_t set_max_pressure;
    int16_t set_min_pressure;

    uint8_t pLowDAC;
    uint8_t pHighDAC; 
    uint8_t Thigh;
    uint8_t Tlow;

    dacWrite(DAC_1, pHighDAC);
    vTaskDelay(pdMS_TO_TICKS(Thigh * 1000));
    dacWrite(DAC_1, pLowDAC);
    vTaskDelay(pdMS_TO_TICKS(Tlow * 1000));

}