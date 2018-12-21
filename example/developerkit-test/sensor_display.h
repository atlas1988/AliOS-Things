/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef SENSOR_DISPLAY_H
#define SENSOR_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

void show_logo();

void clean_labels_all();

void chart_label_init_one(int range);

void chart_label_init(void);

void clean_labels(void);

void refresh_label_data(float *x, float *y, float *z, char *name, char *unit);

void refresh_chart_data(float *x, float *y, float *z);


#ifdef __cplusplus
}
#endif

#endif
