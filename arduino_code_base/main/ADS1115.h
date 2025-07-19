#ifndef INC_SENSOR_ADS115_H_
#define INC_SENSOR_ADS115_H_

#define ADS1115_ADDRS 0x48


#define CONVERSION_REG 0x00
#define CONFIG_REG     0x01
#define LO_THRESH_REG  0x02
#define HI_THRESH_REG  0x03

enum comp_que_configuration {
	ASSERT_AFTER_1_CONVERSION,
	ASSERT_AFTER_2_CONVERSION,
	ASSERT_AFTER_4_CONVERSION,
	ASSERT_DISABLE
};

enum comp_latch_configuration {
	NONLATCHING_COMPARATOR,
	LATCHING_COMPARATOR
};

enum comp_polarity_configuration {
	ACTIVE_LOW,
	ACTIVE_HIGH
};

enum comp_mode_configuartion {
	TRADITIONAL_COMPARATOR,
	WINDOW_COMPARATOR
};

enum data_rate_configuration {
	SPS_8,
	SPS_16,
	SPS_32,
	SPS_64,
	SPS_128,
	SPS_250,
	SPS_475,
	SPS_860
};

enum operating_mode_configuration {
	CONTINUOUS_CONVERSION,
	SINGLE_SHORT,
};

enum PGA_conf_configuration {
	FSR_6_144_V,
	FSR_4_096_V,
	FSR_2_048_V,
	FSR_1_024_V,
	FSR_0_512_V,
	FSR_0_256_V = 0X07
};

enum mux_conf_configuration {
	AIN0_and_AIN1,
	AIN0_and_AIN3,
	AIN1_and_AIN3,
	AIN2_and_AIN3,
	AIN0_and_GND,
	AIN1_and_GND,
	AIN2_and_GND,
	AIN3_and_GND
};

enum operational_status_configuraton_read {
	CONVERSION_PERFORMED,
	CONVERSION_NOT_PERFORMED
};

enum operational_status_configuraton_write{
	NO_EFFECT,
	START_SINGLE_CONVERSION,
};

struct __attribute__((packed)) config_reg_field_desc {

	uint8_t operating_mode : 1;
	uint8_t PGA_conf : 3;
	uint8_t mux_conf : 3;
	uint8_t operational_status : 1;

	uint8_t comp_que : 2;
	uint8_t comp_latch : 1;
	uint8_t comp_polarity : 1;
	uint8_t comp_mode : 1;
	uint8_t data_rate : 3;
};

struct __attribute__((packed)) ads1115_conf_reg{
	uint8_t reg_addrs;
	uint8_t reg_MSB;
	uint8_t reg_LSB;
};

#endif