#include <assert.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
// #include <string.h>


int main(void){
    mqd_t measurement_queue = mq_open("/measurements", O_WRONLY);
    if (measurement_queue < 0){
        fprintf(stderr, "Queue open failed.\n");
        return EXIT_FAILURE;
    }
    uint32_t measurement = 12;
    char data[256];
    snprintf(data, sizeof(data), "%d,%d", 10, measurement);

    if (mq_send(measurement_queue, data, sizeof(data), 0)) {
            fprintf(stderr, "Measurement sending failed.\n");
            return EXIT_FAILURE;
    }
}