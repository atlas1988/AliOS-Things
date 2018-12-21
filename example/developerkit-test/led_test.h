/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef LED_TEST_H
#define LED_TEST_H

#ifdef __cplusplus
extern "C" {
#endif
#include <hal/soc/gpio.h>

typedef struct{
	char *name;
	char id;
//  char brightness
	gpio_dev_t config;
}led_info_t;

int led_set_brightness(char led,unsigned char brightness);
void led_init(void);

#ifdef __cplusplus
}
#endif

#endif
