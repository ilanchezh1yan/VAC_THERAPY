#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_PIN 5

const struct device *gpio_dev;

static struct gpio_callback gpio_cb;
static volatile uint32_t edge_count = 0;

void gpio_callback_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    edge_count++;
}

int GPIO_init(void)
{
    int ret;

    gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio_0));

    ret = device_is_ready(gpio_dev);
    if(!ret) {
	printk("device is not ready");
        return ret;
    }

    ret = gpio_pin_configure(gpio_dev, GPIO_PIN, GPIO_INPUT | GPIO_INT_EDGE_BOTH);
    if (ret) {
        return ret;
    }
    return 0;
}

int32_t cs_status(uint8_t pin, uint8_t status)
{
	int32_t ret;
	ret = gpio_pin_set(gpio_dev, pin, status);
	if(ret) {
		return ret;
	}
	return 0;
}

#if 0
int main(void) {
	int ret;
	uint32_t count_snapshot = 0;
    	uint32_t frequency = 0;

	ret = GPIO_init();
	if(ret) {
		return ret;
	}
	
	gpio_init_callback(&gpio_cb, gpio_callback_handler, BIT(GPIO_PIN));
    	ret = gpio_add_callback(gpio_dev, &gpio_cb);
    	if (ret < 0) {
        	printk("Failed to add GPIO callback\n");
        	return ret;
    	}

    	ret = gpio_pin_interrupt_configure(gpio_dev, GPIO_PIN, GPIO_INT_EDGE_BOTH);
    	if (ret < 0) {
        	printk("Failed to configure GPIO interrupt\n");
        	return ret;
	}

    	while(1){
		k_sleep(K_SECONDS(1));
        
        	count_snapshot = edge_count;
        	edge_count = 0;
   
        	frequency = count_snapshot;
        	printk("Frequency: %u Hz\n", frequency);
	}
}
#endif