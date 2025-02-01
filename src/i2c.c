#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

const struct device *i2c_dev;

int32_t i2c_init(void)
{
	int32_t ret;
	i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c_1));

	ret = device_is_ready(i2c_dev);
    	if (!ret) {
        	return ret;
    	}
	return 0;
}

int32_t main(void)
{
	uint8_t dummy_data = 0;
	int32_t ret;
	uint8_t add = 0;
	ret = i2c_init();
	if(ret) {
		return ret;
	}
		
	for (add = 0x03; add <= 0x77; add++) {
        ret = i2c_write(i2c_dev, &dummy_data, 1, add);
	printk("%hhx, %d\n",add, ret);
		if(!ret){ 
			printk("%hhu is the address", add);
			return ret;
		}
	k_sleep(K_SECONDS(1));
	}
}
