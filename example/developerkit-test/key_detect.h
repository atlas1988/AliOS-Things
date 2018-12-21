/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef KEY_DETECT_H
#define KEY_DETECT_H

#ifdef __cplusplus
extern "C" {
#endif
#include <hal/soc/gpio.h>

typedef struct{
	char *name;
	char id;
	gpio_dev_t config;
}key_info_t;

//init key set
void key_set_init();

#ifdef __cplusplus
}
#endif

#endif
