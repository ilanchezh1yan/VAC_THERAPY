#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

/* ADC configuration */
#define ADC_NODE        DT_NODELABEL(adc_0)  /* ADC1 node in devicetree */
#define ADC_CHANNEL     7                   /* ADC channel to read */
#define ADC_RESOLUTION  12                  /* ADC resolution (bits) */
#define ADC_GAIN        ADC_GAIN_1
#define ADC_REFERENCE   ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME_DEFAULT

const struct device *adc_dev;

/* Buffer to hold ADC sample */
static int16_t sample_buffer;

/* ADC channel configuration */
static const struct adc_channel_cfg adc_channel_cfg = {
    .gain             = ADC_GAIN,
    .reference        = ADC_REFERENCE,
    .acquisition_time = ADC_ACQUISITION_TIME,
    .channel_id       = ADC_CHANNEL,
    .differential     = 0,
};

struct adc_sequence sequence = {
        .channels    = BIT(ADC_CHANNEL),
        .buffer      = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution  = ADC_RESOLUTION,
};

int ADC_init(void)
{
    int ret;

    adc_dev = DEVICE_DT_GET(ADC_NODE);
    
    ret = device_is_ready(adc_dev);
    if (ret) {
        return ret;
    }

    ret = adc_channel_setup(adc_dev, &adc_channel_cfg);
    if(ret) {
        return ret;
    }

}

int ADC_Read_raw(void){

        int ret;
	ret = adc_read(adc_dev, &sequence);
	if(ret){ 
		return ret;
	}

	return sample_buffer;	
}
