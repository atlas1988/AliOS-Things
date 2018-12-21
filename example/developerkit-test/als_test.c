/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <aos/aos.h>
#include "lvgl/lvgl.h"
#include <k_api.h>
#include "als_test.h"
#include <hal/sensor.h>
#include "developerkit-test.h"
#include "sensor_display.h"

extern aos_timer_t refresh_timer;
extern unsigned int  KEY_STATUS;
static char item_select = 0;

/* als sensor fd */
int fd_als = -1;
static test_items_t *test_item;

static void sns_als_refresh_task(void *arg);
 static int get_als_data(float *x, float *y, float *z);

 char als_test(void *item)
{
	char ret = 0, i = 0;
	aos_msleep(100);
	test_item =(test_items_t *) item;
	for(i = 0;test_item[i].id != NULL ;i++){
		if(strcmp(test_item[i].name,"als") == 0){
			item_select = i;
			break;
		}
	}
    /* open als sensor */
    fd_als = aos_open(test_item[item_select].device_path, O_RDWR);
	printf("device path %s \n",test_item[item_select].device_path);
    if (fd_als < 0) {
        printf("als sensor open failed !\n");
		return -1;
    }
	aos_timer_stop(&refresh_timer);
	aos_timer_free(&refresh_timer); // free old timer
	aos_msleep(100);
	test_item[item_select].stat = 1;
    /* create a timer to refresh sensor data */
    aos_timer_new(&refresh_timer, sns_als_refresh_task, NULL, 350, 1);
	return ret;
}




static int get_als_data(float *x, float *y, float *z)
{
    als_data_t data = {0};
    ssize_t size = 0;

    size = aos_read(fd_als, &data, sizeof(data));
    if (size != sizeof(data)) {
        return -1;
    }
    *x = (float)data.lux   ;
    *y = 0  ;
    *z = 0  ;

    return 0;
}
static void sns_als_refresh_task(void *arg){
	static char task_count = 0;
	float x = 0, y = 0, z = 0;
	int item_num = 0;
	char i =0;
	//printf("refresh_task\n");
	for(i = 0;test_item[i].id != NULL ;i++){
		if(test_item[i].stat == 1){
			item_num = test_item[i].id -1;
			break;
		}
	}
	if(task_count == 1){
		clean_labels();
		chart_label_init_one(2000);
	}else if(task_count >= 2){
		get_als_data(&x, &y, &z);
		printf("als sensor data x = %.4f, y = %.4f,z = %.4f \n",x,y,z);
		//printf("acc_item[0].name = %s \n",test_item[1].name);
		refresh_chart_data(&x, NULL, NULL);
		refresh_label_data(&x, NULL, NULL, test_item[item_select].name, test_item[item_select].unit );
	}
	if(task_count < 7)
		task_count++;
	
	while(KEY_STATUS){
		switch(KEY_STATUS){
			case 1:// key A
				printf("key A pressed! %d \n", item_num);
				if(item_num == 0){
					printf("This is the first one! \n");
				}else{
					aos_close(fd_als);
					test_item[item_num--].stat = 2; // test is 
					test_item[item_num].test_start((void *)test_item);
				}
				KEY_STATUS = 0;
				break;
			case 4: // key M
				printf("key m pressed! %d \n", item_num);
				//test_item[0].test_start((void *)test_item);
				KEY_STATUS = 0;
				break;
			case 8: // key B
				printf("key B pressed! %d \n", item_num);
				if(test_item[item_num + 1].id != 0){
					aos_close(fd_als);
					test_item[item_num].stat = 2; // test is 
					item_num++;
					test_item[item_num].test_start((void *)test_item);
				}else{
					printf("This is the last one! \n");
				}
				KEY_STATUS = 0;
				break;
			default:
				KEY_STATUS = 0;
		}
	}
}

