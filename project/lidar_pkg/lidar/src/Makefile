CC = $(CROSS_COMPILE)gcc

CFLAGS += -Wall -Wextra -Wpedantic

all: main

VL53L0X_rasp/bin/libVL53L0X_Rasp.a:
	API_DIR=../VL53L0X_API $(MAKE) -C VL53L0X_rasp


main: main.c VL53L0X_rasp/bin/libVL53L0X_Rasp.a
	$(CC) -I VL53L0X_rasp/platform/inc -I VL53L0X_API/Api/core/inc $(CFLAGS) -o main main.c -L VL53L0X_rasp/bin -l VL53L0X_Rasp -lrt



clean:
	rm main
