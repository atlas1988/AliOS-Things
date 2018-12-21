NAME := developerkit-test

$(NAME)_SOURCES := developerkit-test.c AliOS_Things_logo.c sensor_display.c acc_test.c gyro_test.c mag_test.c als_test.c \
     pls_test.c humidity_test.c temp_test.c baro_test.c key_detect.c camera_test.c led_test.c
GLOBAL_DEFINES += AOS_NO_WIFI CONFIG_AOS_FATFS_SUPPORT_MMC

$(NAME)_COMPONENTS := yloop cli kernel.modules.fs.fatfs board.developerkit.camera_hal board.developerkit.irda_hal sensor

ifeq ($(BENCHMARKS),1)
$(NAME)_COMPONENTS  += benchmarks
GLOBAL_DEFINES      += CONFIG_CMD_BENCHMARKS
endif

$(NAME)_INCLUDES    += .

$(NAME)_COMPONENTS += framework.GUI.littlevGL
GLOBAL_INCLUDES     += ../framework/GUI/littlevGL/

GLOBAL_DEFINES += LITTLEVGL_DEVELOPERKIT
