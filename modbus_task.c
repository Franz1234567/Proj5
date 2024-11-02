#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define ID_MOTOR 1
#define ID_LIGHT_SENSOR 2
#define WRITE_REGISTER 0x06
#define READ_REGISTER 0x03

int read_light_sensor(int file, uint8_t device_id, uint8_t function_code, uint16_t reg);
int write_motor_speed(int file, uint8_t device_id, uint8_t function_code, uint16_t reg, uint16_t value);

const size_t MSG_LEN = 8;

uint8_t device_id;
uint8_t function_code;
uint16_t reg;
uint16_t value;
uint16_t crc;

int main(int argc, char *argv[]) {
    int file;

    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        perror("UART: Failed to open the file.\n");
        return -1;
    }

    struct termios options;
    tcgetattr(file, &options);
    cfmakeraw(&options);

    options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;
    options.c_cc[VMIN]=8;
    tcflush(file, TCIFLUSH);
    tcsetattr(file, TCSANOW, &options);
    int counter = 0;
    int light_sensor_value = read_light_sensor(file, ID_LIGHT_SENSOR, READ_REGISTER, 1);
    int motor_speed_ref = light_sensor_value*3000/1024.0;
    int speed_measured = write_motor_speed(file, ID_MOTOR, WRITE_REGISTER, 1, motor_speed_ref);
    while(1){
        light_sensor_value = read_light_sensor(file, ID_LIGHT_SENSOR, READ_REGISTER, 1);
        if (light_sensor_value < 0 || light_sensor_value > 1023) {
            // printf("Failed to read light sensor value\n");
        }
        else {
            // printf("Light sensor value: %d\n", light_sensor_value);
        }
        if(counter == 32)
        {
            motor_speed_ref = light_sensor_value*3000/1024.0;
            speed_measured = write_motor_speed(file, ID_MOTOR, WRITE_REGISTER, 1, motor_speed_ref);
            counter = 0;
        }
        else{
            speed_measured = write_motor_speed(file, ID_MOTOR, WRITE_REGISTER, 1, motor_speed_ref);
        }
        
        // if(counter == 32)
        // {
        //     counter = 0;
        //     // printf("RESET VALUE \n");
        //     // printf("Reference speed: %d ------", motor_speed_ref);
        //     // printf("Measured speed: %d\n", speed_measured);   
        // }
        printf("Reference speed: %d ------", motor_speed_ref);
        printf("Measured speed: %d\n", speed_measured);   

        counter++;    
        usleep(250000);
    }
}

// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t buf[], int len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) { crc ^= (uint16_t)buf[pos];

        for(int i=8;i!=0;i--){ if ((crc & 0x0001) != 0) {
            crc >>= 1;
            crc ^= 0xA001;
            }
        else
        crc >>= 1;
        // XOR byte into least sig. by
        } }
        // Note, this number has low and high bytes swapped, so use it accor dingly (or swap bytes)
    return crc;
    }

int read_light_sensor(int file, uint8_t device_id, uint8_t function_code, uint16_t reg){
    // for now, we just want to send a message to check communication by sending message like 2 3 0 0 and sent back the same message
    uint8_t msg[MSG_LEN];
    uint8_t rsp[MSG_LEN];

    msg[0] = device_id;
    msg[1] = function_code;
    msg[2] = reg >> 8; // take first high bits
    msg[3] = reg & 0xFF; // take low bits
    msg[4] = 0;
    msg[5] = 0;

    uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
    uint8_t crc1 = crc >> 8;
    uint8_t crc2 = crc & 0xFF;

    msg[6] = crc1;
    msg[7] = crc2;

    // print the message
    //printf("send sensor: %x %x %x %x %x %x %x %x\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7]);
    ssize_t count = write(file, msg, MSG_LEN);
    if (count < 0) {
        perror("Failed to write to the output\n");
        return -1;
    }
    usleep(200000);

    // get response
    count = read(file, (void*)rsp, MSG_LEN);
    if (count < 0) {
        perror("Failed to read from the input\n");
        return -1;
    }

    //printf("received sensor %x %x %x %x %x %x %x %x\n", rsp[0], rsp[1], rsp[2], rsp[3], rsp[4], rsp[5], rsp[6], rsp[7]);

    // add CRC check using ModRTU_CRC function
    crc = ModRTU_CRC(rsp, MSG_LEN - 2);
    crc1 = crc >> 8;
    crc2 = crc & 0xFF;

    // verify the crc of message
    if (crc1 != rsp[6] || crc2 != rsp[7]) {
        printf("CRC check failed!\n");
        return -1;
    }
    else {
        // printf("CRC check passed!\n");
    }

    if (count == 8) {
        if(rsp[1] != msg[1]){
            printf("ERROR: ");
            if(rsp[5] == 1){
                printf("Unsupported function code\n");
            }
            if(rsp[5] == 2){
                printf("Unsupported register\n");
            }
            return -1;
        }
        else{
            // printf("Light sensor value: %u\n", (rsp[4] << 8) | rsp[5]);
            int light_sensor_value = (rsp[4] << 8) | rsp[5];
            return light_sensor_value;
        }
        
    }
    else if (count == 0) {
        printf("There was no data available to read!\n");
        return -1;
    }
    else {
        printf("Unexpected response length: %zd\n", count);
        return -1;
    }
}

int write_motor_speed(int file, uint8_t device_id, uint8_t function_code, uint16_t reg, uint16_t value) {
    uint8_t msg[MSG_LEN];
    uint8_t rsp[MSG_LEN];

    msg[0] = device_id;
    msg[1] = function_code;
    msg[2] = reg >> 8;
    msg[3] = reg & 0xFF;
    msg[4] = value >> 8;
    msg[5] = value & 0xFF;

    uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
    uint8_t crc1 = crc >> 8;
    uint8_t crc2 = crc & 0xFF;

    msg[6] = crc1;
    msg[7] = crc2;

    ssize_t count = write(file, msg, MSG_LEN);
    if (count < 0) {
        perror("Failed to write to the output\n");
        return -1;
    }
    // printf("send motor: %x %x %x %x %x %x %x %x\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7]);

    usleep(200000);

    //get response

    count = read(file, (void*)rsp, MSG_LEN);
    if (count < 0) {
        perror("Failed to read from the input\n");
        return -1;
    }

    // printf("resp motor: %x %x %x %x %x %x %x %x\n", rsp[0], rsp[1], rsp[2], rsp[3], rsp[4], rsp[5], rsp[6], rsp[7]);


    // add CRC check using ModRTU_CRC function
    crc = ModRTU_CRC(rsp, MSG_LEN - 2);
    crc1 = crc >> 8;
    crc2 = crc & 0xFF;

    // verify the crc of message
    if (crc1 != rsp[6] || crc2 != rsp[7]) {
        printf("CRC check failed!\n");
        return -1;
    }
    else {
        // printf("CRC check passed!\n");
    }

    if (count == 0) {
        printf("There was no data available to read!\n");
        return -1;
    }
    else if (count == 8) {
        if(rsp[1] != msg[1]){
            printf("ERROR: ");
            if(rsp[5] == 1){
                printf("Unsupported function code\n");
            }
            if(rsp[5] == 2){
                printf("Unsupported register\n");
            }
            if(rsp[5] == 3){
                printf("No possible transition\n");
            }
            return -1;
        }
        //printf("motor speed set \n");
    }
    else {
        printf("Unexpected response length: %zd\n", count);
        return -1;
    }
    int motor_speed = (rsp[4] << 8) | rsp[5];
    return motor_speed;
}


