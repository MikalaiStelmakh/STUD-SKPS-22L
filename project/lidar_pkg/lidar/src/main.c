#include <assert.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "vl53l0x_api.h"
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"

#define PORT     8080
#define MAXLINE 1024

#define MQ_PATH "/measurements"

#define PWM "/sys/class/pwm/pwmchip0"


mqd_t measurement_queue_write = 0;
mqd_t measurement_queue_read = 0;
VL53L0X_Dev_t sensor = {0};

int sockfd = 0;
struct sockaddr_in servaddr, cliaddr;


void abort_if_error(VL53L0X_Error Status){
    if (Status == VL53L0X_ERROR_NONE) return;
    char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    fprintf(stderr, "Sensor failure: %s\n", buf);
    exit(EXIT_FAILURE);
}

void sensor_init(){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    VL53L0X_DeviceInfo_t                DeviceInfo;


    sensor.I2cDevAddr      = 0x29;

    sleep(0.5);

    //choose between i2c-0 and i2c-1; On the raspberry pi zero, i2c-1 are pins 2 and 3
    sensor.fd = VL53L0X_i2c_init("/dev/i2c-1", sensor.I2cDevAddr);
    if (sensor.fd<0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        abort_if_error(Status);
    }

    Status = VL53L0X_GetDeviceInfo(&sensor, &DeviceInfo);
    printf("VL53L0X_GetDeviceInfo:\n");
    printf("Device Name : %s\n", DeviceInfo.Name);
    printf("Device Type : %s\n", DeviceInfo.Type);
    printf("Device ID : %s\n", DeviceInfo.ProductId);
    printf("ProductRevisionMajor : %d\n", DeviceInfo.ProductRevisionMajor);
    printf("ProductRevisionMinor : %d\n", DeviceInfo.ProductRevisionMinor);
    abort_if_error(Status);

    printf ("Call of VL53L0X_StaticInit\n");
    Status = VL53L0X_StaticInit(&sensor);
    abort_if_error(Status);

    printf ("Call of VL53L0X_PerformRefCalibration\n");
    Status = VL53L0X_PerformRefCalibration(&sensor, &VhvSettings, &PhaseCal);
    abort_if_error(Status);

    printf ("Call of VL53L0X_PerformRefSpadManagement\n");
    Status = VL53L0X_PerformRefSpadManagement(&sensor, &refSpadCount, &isApertureSpads);
    abort_if_error(Status);

    printf ("Call of VL53L0X_SetDeviceMode\n");
    Status = VL53L0X_SetDeviceMode(&sensor, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    abort_if_error(Status);

    printf ("Call of VL53L0X_StartMeasurement\n");
    Status = VL53L0X_StartMeasurement(&sensor);
    abort_if_error(Status);
}

VL53L0X_Error WaitStopCompleted() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t StopCompleted=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetStopCompletedStatus(&sensor, &StopCompleted);
            if ((StopCompleted == 0x00) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(&sensor);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }

    }

    return Status;
}

void sensor_stop(){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    printf ("Call of VL53L0X_StopMeasurement\n");
    Status = VL53L0X_StopMeasurement(&sensor);
    abort_if_error(Status);

    printf ("Wait Stop to be competed\n");
    Status = WaitStopCompleted();
    abort_if_error(Status);

    Status = VL53L0X_ClearInterruptMask(&sensor,
		VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    // abort_if_error(Status);
    VL53L0X_i2c_close();
}

VL53L0X_Error wait_measurement_data_ready(){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t NewDataReady=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetMeasurementDataReady(&sensor, &NewDataReady);
            if ((NewDataReady == 0x01) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(&sensor);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
    }

    return Status;
}

uint32_t measure(){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;

    Status = wait_measurement_data_ready(&sensor);
    abort_if_error(Status);

    Status = VL53L0X_GetRangingMeasurementData(&sensor, &RangingMeasurementData);
    abort_if_error(Status);

    VL53L0X_ClearInterruptMask(&sensor, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    VL53L0X_PollingDelay(&sensor);

    return RangingMeasurementData.RangeMilliMeter;
}

// Servo

void write_to_txt(char* const filename, char* const text) {
    int fd = open(filename, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "Opening servo failed.\n");
        exit(EXIT_FAILURE);
    }

    ssize_t status = write(fd, text, strlen(text));
    if (status < 0) {
        fprintf(stderr, "Closing servo failed.\n");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void pwm_open(void){
    write_to_txt(PWM "/export", "0\n");
}

void pwm_close(void){
    write_to_txt(PWM "/unexport", "0\n");
}

void servo_rotate(uint16_t angle){
    // TODO: calculate duty_cycle
    uint32_t duty_cycle = 2000000;

    char duty_cycle_str[32];
    snprintf(duty_cycle_str, sizeof(duty_cycle_str), "%u\n", duty_cycle);

    // Set the PWM signal
    write_to_txt(PWM "/pwm0/period", "20000000\n");
    write_to_txt(PWM "/pwm0/duty_cycle", duty_cycle_str);
    write_to_txt(PWM "/pwm0/enable", "1\n");

    // Wait for the servo to rotate
    usleep(250000);
    write_to_txt(PWM "/pwm0/enable", "0\n");

}

// UDP
void socket_open(void){
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void socket_setup(){
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void receive_udp(char *buffer){
    int length, n;
    length = sizeof(cliaddr);  //len is value/result
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &length);
    buffer[n] = '\0';
}

void send_udp(char *buffer){
    int length = sizeof(cliaddr);
    int sent = sendto(sockfd, buffer, strlen(buffer),
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            length);
    if (sent < 0){
        fprintf(stderr, "UDP send failed.\n");
        exit(EXIT_FAILURE);
    }
}

void send_measurement_update_udp(){
    printf("Send\n");
    struct pollfd poll_descriptor = {
        .fd = measurement_queue_read,
        .events = POLLIN
    };
    int ready = poll(&poll_descriptor, 1, -1);
    if(ready < 0)
    {
        fprintf(stderr, "Poll failed.\n");
        exit(EXIT_FAILURE);
    }
    else if(poll_descriptor.revents & POLLIN){
        struct mq_attr attr;
        mq_getattr(measurement_queue_read, &attr);
        char queue_buffer[attr.mq_msgsize];
        ssize_t received = mq_receive(measurement_queue_read, queue_buffer, attr.mq_msgsize, NULL);
        if (received < 0){
            fprintf(stderr, "mq receive failed.\n");
            exit(EXIT_FAILURE);
        }

        send_udp(queue_buffer);

    }
}

void clean(){
    pwm_close();
    sensor_stop();
    if (measurement_queue_write > 0){
        mq_close(measurement_queue_write);
    }
    if (measurement_queue_read > 0){
        mq_close(measurement_queue_read);
    }
    printf("Cleanup success.\n");
}

int main(void){
    char buffer[MAXLINE];


    if (atexit(clean)) {
        printf("Atexit cleanup failed.\n");
        return EXIT_FAILURE;
    }


    mq_unlink(MQ_PATH);
    measurement_queue_read = mq_open(MQ_PATH, O_CREAT | O_RDONLY | O_EXCL, 0700, NULL);
    if (measurement_queue_read < 0){
        fprintf(stderr, "Queue opening failed.\n");
        return EXIT_FAILURE;
    }

    measurement_queue_write = mq_open("/measurements", O_WRONLY);
    if (measurement_queue_write < 0){
        fprintf(stderr, "Queue opening failed.\n");
        return EXIT_FAILURE;
    }

    sensor_init();
    uint16_t angle = 0;

    socket_open();
    socket_setup();

    receive_udp(buffer);
    printf("Connected.\n");



    while (1){
        // TODO: change angle every iteration

        angle++;
        servo_rotate(angle);
        uint32_t measurement = measure(&sensor);
        char data[256];
        snprintf(data, sizeof(data), "%u,%u", angle, measurement);
        if (mq_send(measurement_queue_write, data, sizeof(data), 0)) {
            fprintf(stderr, "Measurement sending failed.\n");
            return EXIT_FAILURE;
        }
        send_measurement_update_udp();
    }
    return 0;
}
