/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef DEVELOPERKIT_TEST_H
#define DEVELOPERKIT_TEST_H

#ifdef __cplusplus
extern "C" {
#endif
#include "acc_test.h"
#include "gyro_test.h"
#include "mag_test.h"
#include "als_test.h"
#include "pls_test.h"
#include "humidity_test.h"
#include "baro_test.h"
#include "temp_test.h"
#include "camera_test.h"

//#include "ir_test.h"

typedef struct {
	unsigned char id;
	char stat; // 0 init  1 on 2 del
	char *name;
	char *unit;
	char *device_path;
	char (*test_start)(void *item);
	void *arg;
}test_items_t;

#ifdef __cplusplus
}
#endif

#endif
