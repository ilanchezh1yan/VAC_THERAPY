#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>

int32_t cs_status(uint8_t, uint8_t);
int GPIO_init(void);

#define CS_PIN 5

#define CS_HIGH 1
#define CS_LOW 0

const static struct device *rfid_rc522;

const static struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) |	SPI_FRAME_FORMAT_MOTOROLA  |SPI_FULL_DUPLEX | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER |
		     SPI_MODE_CPOL | SPI_MODE_CPHA,
    	.frequency =2000000,
};

int32_t SPI_init(void)
{
	int ret;
	spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi_3));

	ret = device_is_ready(rfid_rc522);
    	if (!ret) {
		printk("%d\n", ret);
        	return ret;
    	}
	return 0;
};

int rc522_init(void)
{
	uint8_t conf_data[2] = {CommandReg, 0x0f}; 
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0])); \\SoftReset
	
	conf_data[0] = TxModeReg;
	conf_data[1] = 0x00;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] = RxModeReg;
	conf_data[1] = 0x00;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] = ModWidthReg;
	conf_data[1] = 0x26;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] =  TModeReg;
	conf_data[1] = 0x80;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] = TPrescalerReg;
	conf_data[1] = 0xA9;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] = TReloadRegH;
	conf_data[1] = 0x03;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] = TReloadRegL;
	conf_data[1] = 0xxE8;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] =  TxASKReg;
	conf_data[1] = 0x40;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] = ModeReg;
	conf_data[1] = 0x3D;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));

	conf_data[0] =  TxControlReg;
	RFID_RC522_spi_read(conf_data, sizeof(conf_data)/sizeof(conf_data[0]));
	conf_data[1] |= 0x03;
	RFID_RC522_spi_send(conf_data, sizeof(conf_data)/sizeof(conf_data[0])); 	
}

static int32_t RFID_RC522_spi_read(uint8_t *data,uint8_t length)
{
	int32_t ret;
  	struct spi_buf bufs = {
            .buf = data,
            .len = length
    	};

	struct spi_buf_set rx = {
            .buffers = &bufs,
	    .count = 1,
    	};
	
	ret = spi_read(rfid_rc522, &spi_cfg, &rx);
	if (ret) {
		return ret;
	}

	return 0;
    
}

static int32_t RFID_RC522_spi_send(uint8_t *data, uint8_t length)
{
	int32_t ret;
  	struct spi_buf bufs = {
            .buf = data,
            .len = length
    	};

	struct spi_buf_set tx = {
            .buffers = &bufs,
	    .count = 1,
    	};
	
	ret = spi_write(rfid_rc522, &spi_cfg, &tx);
	if (ret) {
		return ret;
	}

	return 0;
    
}

int32_t Canistor_check(void) 
{
	
	
}




