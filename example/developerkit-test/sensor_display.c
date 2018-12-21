/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <aos/aos.h>
#include "lvgl/lvgl.h"
#include <k_api.h>
#include "sensor_display.h"
#include <hal/sensor.h>


LV_IMG_DECLARE(AliOS_Things_logo);
static lv_obj_t *img_src = NULL;

#define MAX_MSG_BYTES 100
//#define MAX_NUM_BYTES 20

char msg_buffer[MAX_MSG_BYTES];
//char number_buf[MAX_NUM_BYTES];


/* gui object definition */
lv_obj_t *scr = NULL;
lv_obj_t *chart1 = NULL;
lv_obj_t *label1 = NULL;
lv_obj_t *label2 = NULL;
lv_obj_t *label3 = NULL;
lv_chart_series_t *dl1_1 = NULL;
lv_chart_series_t *dl1_2 = NULL;
lv_chart_series_t *dl1_3 = NULL;
static int count = 0; // some operation just one time
static char init_stat = 0; // 1 -> one label  3 -> three labels

void show_logo(){
	
    scr = lv_scr_act();

    img_src = lv_img_create(scr, NULL);  /*Crate an image object*/
    lv_img_set_src(img_src, &AliOS_Things_logo);  /*Set the created file as image (a red fl  ower)*/
    lv_obj_set_pos(img_src, 60, 60);      /*Set the positions*/
    lv_obj_set_drag(img_src, true);
	aos_msleep(100);
	// clean screen
    lv_obj_del(img_src);
	
}
static lv_style_t style;

void chart_label_init_one(int range){
	
	init_stat = 1;
	/* hide alios logo and display sensor data */
	//printf("chart_label_init\n");
	if(count == 0){
		scr = lv_scr_act();
	}else{
		//count = 0;
	}
	
   /* create a label to display acc_x */
   label1 = lv_label_create(scr, NULL);
   lv_obj_set_pos(label1, 20, 20);
   lv_style_copy(&style, &lv_style_plain);
   style.text.color = LV_COLOR_BLUE;
   lv_label_set_style(label1, &style);

   /* create a chart to display all data */
   chart1 = lv_chart_create(scr, NULL);
   lv_chart_set_type(chart1, LV_CHART_TYPE_LINE);
   lv_obj_set_size(chart1, 200, 120);
   lv_obj_set_pos(chart1, 20, 100);
   lv_chart_set_range(chart1, 0, 40);
   lv_chart_set_point_count(chart1, 30);

   /* add acc_x acc_y acc_z to the chart */
   dl1_1 = lv_chart_add_series(chart1, LV_COLOR_BLUE);

}

void chart_label_init(){
	//static lv_style_t style;
	static lv_style_t style2;
	static lv_style_t style3;
	init_stat = 3;
	/* hide alios logo and display sensor data */
	//printf("chart_label_init\n");
	if(count == 0){
   		scr = lv_scr_act();
	}else{
		//count = 0;
	}

   /* create a label to display acc_x */
   label1 = lv_label_create(scr, NULL);
   lv_obj_set_pos(label1, 20, 20);
   lv_style_copy(&style, &lv_style_plain);
   style.text.color = LV_COLOR_BLUE;
   lv_label_set_style(label1, &style);


   /* create a label to display acc_y */
   label2 = lv_label_create(scr, NULL);
   lv_obj_set_pos(label2, 20, 45);
   lv_style_copy(&style2, &lv_style_plain);
   style2.text.color = LV_COLOR_RED;
   lv_label_set_style(label2, &style2);

   /* create a label to display acc_z */
   label3 = lv_label_create(scr, NULL);
   lv_obj_set_pos(label3, 20, 70);
   lv_style_copy(&style3, &lv_style_plain);
   style3.text.color = LV_COLOR_BLACK;
   lv_label_set_style(label3, &style3);

   /* create a chart to display all data */
   chart1 = lv_chart_create(scr, NULL);
   lv_chart_set_type(chart1, LV_CHART_TYPE_LINE);
   lv_obj_set_size(chart1, 200, 120);
   lv_obj_set_pos(chart1, 20, 100);
   lv_chart_set_range(chart1, 0, 40);
   lv_chart_set_point_count(chart1, 30);

   /* add acc_x acc_y acc_z to the chart */
   dl1_1 = lv_chart_add_series(chart1, LV_COLOR_BLUE);
   dl1_2 = lv_chart_add_series(chart1, LV_COLOR_RED);
   dl1_3 = lv_chart_add_series(chart1, LV_COLOR_BLACK);

}
void clean_labels_all(){
	lv_obj_del(label1);
	lv_obj_del(chart1);
	lv_obj_del(scr);
}
void clean_labels(){
	printf("clean_labels \n");
	if(init_stat == 1){
		lv_obj_del(label1);
		lv_obj_del(chart1);
	}
	if(init_stat == 3){
		lv_obj_del(label1);
		lv_obj_del(label2);
		lv_obj_del(label3);
		lv_obj_del(chart1);
	}

	init_stat = 0;
	//lv_obj_del(scr);
	
}
//void refresh_label_data(lv_obj_t *label1, lv_obj_t *label2, lv_obj_t *label3, float *x, float *y, float *z){
void refresh_label_data(float *x, float *y, float *z, char *name, char *unit){

	 /* refresh label */
	if(x != NULL){
		sprintf(msg_buffer, "%s_x is %.4f %s", name, *x , unit);
	    lv_label_set_text(label1, msg_buffer);
	}

	if(y != NULL){
		sprintf(msg_buffer, "%s_y is %.4f %s", name,  *y , unit);
	    lv_label_set_text(label2, msg_buffer);
	}

	if(z != NULL){
		sprintf(msg_buffer, "%s_z is %.4f %s", name,  *z, unit);
		lv_label_set_text(label3, msg_buffer);
	}
}

//void refresh_chart_data(lv_chart_series_t *dl1_1,lv_chart_series_t *dl1_2, lv_chart_series_t *dl1_3, lv_obj_t *chart1, float *x, float *y, float *z){
void refresh_chart_data(float *x, float *y, float *z){
	int i = 0;
	//printf("refresh_chart_data\n");
	/* refresh chart data */
    if (count == 0) {
        /* to prevent sensor data from dithering at the first time */
        for (i = 0; i <= 29; i++) {
            dl1_1->points[i] = *x + 20;
			if(y != NULL)
            	dl1_2->points[i] = *y + 20;
			if(z != NULL)
            	dl1_3->points[i] = *z + 20;
        }
		count++; // just run one time
    } else {
        for (i = 28; i >= 0; i--) {
            dl1_1->points[i + 1] = dl1_1->points[i];
			if(y != NULL)
            	dl1_2->points[i + 1] = dl1_2->points[i];
			if(z != NULL)
            	dl1_3->points[i + 1] = dl1_3->points[i];
        }

        dl1_1->points[0] = *x + 20;
		if(y != NULL)
        	dl1_2->points[0] = *y + 20;
		if(y != NULL)
         dl1_3->points[0] = *z + 20;
    }

    /* refresh chart */
    lv_chart_refresh(chart1);
	//refresh_label_data(x,y,z);
}

	
