/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <aos/aos.h>
//#include <k_api.h>
#include "led_test.h"

#define LED_MAX 3
#define LED1_PIN 22
#define LED2_PIN 67
#define LED3_PIN 63
//    /* gpio port */  /* gpio config */   /* priv data */
led_info_t led_set[LED_MAX]={
{"led1",1,{LED1_PIN,OUTPUT_OPEN_DRAIN_NO_PULL,0}},
{"led2",2,{LED2_PIN,OUTPUT_OPEN_DRAIN_NO_PULL,0}},
{"led3",3,{LED3_PIN,OUTPUT_OPEN_DRAIN_NO_PULL,0}},
};
int led_set_brightness(char led,unsigned char brightness){
	int ret = 0;
	if(brightness){ // led on
		ret = hal_gpio_output_low(&led_set[led - 1].config);
	}else{ // led off
		ret = hal_gpio_output_high(&led_set[led - 1].config);
	}

	return ret;

}
void led_init(void){
	int i = 0;
	for(i = 0; i< LED_MAX; i++){
	    /* configure GPIO with the given settings */
	    hal_gpio_init(&led_set[i].config);
	    LOG("GPIO[%d] intr, val: %d\n", led_set[i].config.port, led_set[i].config.config);
	    //hal_gpio_output_low(&led_set[i].config);
	}
}


