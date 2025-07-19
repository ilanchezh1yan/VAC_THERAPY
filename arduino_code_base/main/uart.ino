//#include "main.h"
#include <driver/uart.h>
#include <HardwareSerial.h>

#include "data_frame.h"

#define ACK 0x4B4F
#define UART_NUM UART_NUM_2
#define BUF_SIZE (1024)
QueueHandle_t uartQueue;

extern TaskHandle_t Response_handler;
struct data_frame receivedData;

void IRAM_ATTR uartEventTask(void *pvParameters) {
    uart_event_t event;
    for (;;) {
        if (xQueueReceive(uartQueue, (void *)&event, portMAX_DELAY)) {
            if (event.type == UART_DATA && event.size >= RX_BFR_SIZE) {
                uart_read_bytes(UART_NUM, (void *)&receivedData, RX_BFR_SIZE, portMAX_DELAY);
                uart_flush(UART_NUM);
                vTaskResume(Response_handler);
            }
        }
    }
}

void uart_init(void) {

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 10, &uartQueue, 0);
    xTaskCreate(uartEventTask, "uart_event_task", 1024, NULL, 5, NULL);
}

void send_data(uint8_t * data, uint8_t size)
{
  struct response_frame rsp_data; 
	uint8_t i = 0;
  int ret = 0;
	while(i < size) {
		ret = uart_write_bytes(UART_NUM, (uint8_t *)data, size);
		i += ret;
	}
  
}