/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <aos/aos.h>
#include "lvgl/lvgl.h"
#include <k_api.h>
#include "mag_test.h"
#include <hal/sensor.h>
#include "developerkit-test.h"
#include "sensor_display.h"

extern aos_timer_t refresh_timer;
extern unsigned int  KEY_STATUS;
static char item_select = 0;
static char magtask_count = 0;

/* mag sensor fd */
int fd_mag = -1;
static test_items_t *test_item;

static void sns_mag_refresh_task(void *arg);
static int get_mag_data(float *x, float *y, float *z);

 char mag_test(void *item)
{
	char ret = 0, i = 0;
	float mag_tmp[3] = {0};
	magtask_count = 0;
	aos_msleep(100);
	test_item =(test_items_t *) item;
	for(i = 0;test_item[i].id != NULL ;i++){
		if(strcmp(test_item[i].name,"mag") == 0){
			item_select = i;
			break;
		}
	}
    /* open mag sensor */
    fd_mag = aos_open(test_item[item_select].device_path, O_RDWR);
	printf("device path %s \n",test_item[item_select].device_path);
    if (fd_mag < 0) {
        printf("mag sensor open failed !\n");
		return -1;
    }
	aos_timer_stop(&refresh_timer);
	aos_timer_free(&refresh_timer); // free old timer
	aos_msleep(100);
#if defined(CONFIG_MAG_USE_CAL)
	for(i = 0; i < 8; i++){
			get_mag_data(&mag_tmp[0],&mag_tmp[1],&mag_tmp[2]);
			mag_biase[0] += mag_tmp[0];
			mag_biase[1] += mag_tmp[1];
			mag_biase[2] += mag_tmp[2];
		}
		
	//printf("1 mag sensor data mag_biase[0] = %.4f, mag_biase[0] = %.4f,mag_biase[0] = %.4f \n",mag_biase[0],mag_biase[1],mag_biase[2]);
	mag_biase[0] =mag_biase[0] / 8.0f;
	mag_biase[1] =mag_biase[1] / 8.0f;
	mag_biase[2] =mag_biase[2] / 8.0f;
	//printf("2 mag sensor data mag_biase[0] = %.4f, mag_biase[0] = %.4f,mag_biase[0] = %.4f \n",mag_biase[0],mag_biase[1],mag_biase[2]);
#endif
	test_item[item_select].stat = 1;
    /* create a timer to refresh sensor data */
    aos_timer_new(&refresh_timer, sns_mag_refresh_task, NULL, 350, 1);
	return ret;
}




static int get_mag_data(float *x, float *y, float *z)
{
    mag_data_t data = {0};
    ssize_t size = 0;

    size = aos_read(fd_mag, &data, sizeof(data));
    if (size != sizeof(data)) {
        return -1;
    }
	printf("mag_data data.data[0] = %d %x data.data[1] =%x data.data[2]=%x \n",data.data[0],data.data[0],data.data[1],data.data[2]);
    *x = (float)data.data[0] /10000.0f  ;
    *y = (float)data.data[1] /10000.0f  ;
    *z = (float)data.data[2] /10000.0f  ;

    return 0;
}
static void sns_mag_refresh_task(void *arg){
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
	if(magtask_count == 1){
		clean_labels();
		chart_label_init();
	}else if(magtask_count >= 2){
		get_mag_data(&x, &y, &z);
	#if defined(CONFIG_MAG_USE_CAL)
		x -= mag_biase[0] ;
		y -= mag_biase[1] ;
		z -= mag_biase[2] ;		
	#endif
		printf("mag sensor data x = %.4f, y = %.4f,z = %.4f \n",x,y,z);
		//printf("acc_item[0].name = %s \n",test_item[1].name);
		refresh_chart_data(&x, &y, &z);
		refresh_label_data(&x, &y, &z, test_item[item_select].name, test_item[item_select].unit );
	}
	if(magtask_count < 7)
		magtask_count++;
	
	while(KEY_STATUS){
		switch(KEY_STATUS){
			case 1:// key A
				printf("key A pressed! %d \n", item_num);
				if(item_num == 0){
					printf("This is the first one! \n");
				}else{
					aos_close(fd_mag);
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
					aos_close(fd_mag);
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

