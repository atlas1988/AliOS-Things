/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <k_api.h>
#include <aos/aos.h>
#include "soc_init.h"
#include "lvgl/lvgl.h"
#include "st7789.h"
#include "sensor_display.h"
#include "key_detect.h"
#include "led_test.h"
#include "developerkit-test.h"

extern unsigned int  KEY_STATUS;


test_items_t test_item[] = {
{1, 0, "acc","m/s2", dev_acc_path, acc_test},
{2, 0, "gyro", "rad/s", dev_gyro_path, gyro_test},
{3, 0, "mag", "uT", dev_mag_path, mag_test},
{4, 0, "als", "Lux", dev_als_path, als_test},
{5, 0, "pls", "cm", dev_ps_path, pls_test},
{6, 0, "humi", "%", dev_humi_path, humi_test},
{7, 0, "temp", "C", dev_temp_path, temp_test},
{8, 0, "baro", "hpa", dev_baro_path, baro_test},
{9, 0, "cam", "NC", NULL, cam_test},

//{9, 0, "ir", "NC", dev_ir_path, ir_test},

NULL
};

lv_disp_drv_t dis_drv;

void my_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p);
void my_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void my_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p);

static void littlevgl_refresh_task(void *arg)
{
    while (1) {
        /* this function is used to refresh the LCD */
        lv_task_handler();
        //printf("littlevgl_refresh_task\n");
        krhino_task_sleep(RHINO_CONFIG_TICKS_PER_SECOND / 10);
    }
}

int application_start(int argc, char *argv[])
{
	//char item_num = 0;
	//aos_set_log_level(AOS_LL_DEBUG);
    printf("application_start\n");
	LOG("log application_start\n");

    /* init littlevGL */
    lv_init();

    /* init LCD */
    st7789_init();
	/* init key*/
	key_set_init();
	/* init leds*/
	led_init();
    /* register driver for littlevGL */
    lvgl_drv_register();

    /* create a task to refresh the LCD */
    aos_task_new("littlevgl_refresh_task", littlevgl_refresh_task, NULL, 8192);

    /* int app */
    //app_init();
    test_item[0].test_start(&test_item[0]);

    return 0;
}

void lvgl_drv_register(void)
{
    lv_disp_drv_init(&dis_drv);

    dis_drv.disp_flush = my_disp_flush;
    dis_drv.disp_fill = my_disp_fill;
    dis_drv.disp_map = my_disp_map;
    lv_disp_drv_register(&dis_drv);
}

void my_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p)
{
    int32_t x = 0;
    int32_t y = 0;

    for (y = y1; y <= y2; y++) {
        ST7789H2_WriteLine(x1, y, (uint8_t *)color_p, (x2 - x1 + 1));
        color_p += (x2 - x1 + 1);
    }

    lv_flush_ready();
}

void my_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
    int32_t i = 0;
    int32_t j = 0;

    for (i = x1; i <= x2; i++) {
        for (j = y1; j <= y2; j++) {
            ST7789H2_WritePixel(i, j, color.full);
        }
    }
}

void my_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p)
{
    int32_t x = 0;
    int32_t y = 0;

    for (y = y1; y <= y2; y++) {
        ST7789H2_WriteLine(x1, y, (int16_t *)color_p, (x2 - x1 + 1));
        color_p += (x2 - x1 + 1);
    }
}
