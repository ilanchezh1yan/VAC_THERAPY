
#define DAC_1 25
#define RX_BFR_SIZE 9

extern uint8_t receivedData[RX_BFR_SIZE];

void continuous_mode(void)
{
    int16_t set_pressure = -1 * receivedData[8];

    uint8_t dacValue; 

    dacWrite(DAC_1, dacValue);


}
