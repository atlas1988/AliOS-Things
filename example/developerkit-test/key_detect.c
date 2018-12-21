/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <aos/aos.h>
//#include <k_api.h>
#include "key_detect.h"
#include <hal/soc/soc.h>
#include "led_test.h"

#define KEY_A 78
#define KEY_M 75
#define KEY_B 74
#define KEY_MAX 3
unsigned int  KEY_STATUS;

//    /* gpio port */  /* gpio config */   /* priv data */
key_info_t key_set[KEY_MAX]={
{"A",1,{KEY_A,IRQ_MODE,0}},
{"M",2,{KEY_M,IRQ_MODE,0}},
{"B",3,{KEY_B,IRQ_MODE,0}},
};

static void gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    uint32_t value = 0;
	char key_num = 0 , i = 0;
	for(i= 0; i < KEY_MAX; i++){
		if(gpio_num ==key_set[i].config.port ){
			key_num = key_set[i].id;
			break;
		}
	}
	switch(gpio_num ){
		case KEY_A:// key idle is high
			hal_gpio_input_get(&key_set[key_num -1].config, &value);
			led_set_brightness(key_num,!value);// key idle is high
			if((KEY_STATUS != 1) && (value == 0))
				KEY_STATUS = 1;
			break;
		case KEY_M:
			hal_gpio_input_get(&key_set[key_num -1].config, &value);
			led_set_brightness(key_num,!value);
			if((KEY_STATUS != 4) && (value == 0))
				KEY_STATUS = 4;
			break;
		case KEY_B:
			hal_gpio_input_get(&key_set[key_num -1].config, &value);
			led_set_brightness(key_num,!value);
			if((KEY_STATUS != 8) && (value == 0)) // when key is pressed,modify the status.
				KEY_STATUS = 8;
			break;
		default:
			printf("The key is unused!\n");
	}
    //hal_gpio_output_toggle(&led);
} 

void key_set_init(){
	int i = 0;
	KEY_STATUS = 0;
	for(i; i < KEY_MAX; i++){
		LOG("GPIO[%d] intr, val: %d\n", key_set[i].config.port, key_set[i].config.config);
		/* configure GPIO with the given settings */
		hal_gpio_init(&key_set[i].config);
		/* gpio interrupt config */
		hal_gpio_enable_irq(&key_set[i].config, IRQ_TRIGGER_BOTH_EDGES, gpio_isr_handler, (void *) key_set[i].config.port);
	}
}



