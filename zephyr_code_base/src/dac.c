#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/dac.h>

#define DAC_CHANNEL_1  0
#define DAC_RESOLUTION 8 

const struct device *dac_dev;

struct dac_channel_cfg dac_cfg = {
	.channel_id = DAC_CHANNEL_1,
	.resolution = DAC_RESOLUTION,
};

int DAC_init(void)
{
	int ret;

	dac_dev = DEVICE_DT_GET(DT_NODELABEL(dac));
	if (!device_is_ready(dac_dev)) {
		return 0;
	}

	ret = dac_channel_setup(dac_dev, &dac_cfg);
	if (ret) {
		return ret;
	}
}

int motor_speed(unsigned char dac_data)
{
	ret = dac_write_value(dac_dev, DAC_CHANNEL_1, dac_data);
	if (ret) {
		return ret;
	}
}

int main(void)
{
	DAC_init();
	motor_speed(210);
	
	
}
