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

int read_light_sensor(int file);
int ask_motor_speed(int file);

const size_t MSG_LEN = 8;

uint8_t device_id;
uint8_t function_code;
uint16_t reg;
uint16_t value;
uint16_t crc;

int main(int argc, char *argv[]) {
    int file;

    if (argc != 5) {
        // need input like ./modbus 1 6 1 128
        printf("Usage: %s <device_id> <function_code> <register> <value>\n", argv[0]);
        return -2;
    }

    // store the values in the arguments of code, first byte is device id, second byte is function code, third and fourth byte are register, fitth and sixth byte are value, seventh and eighth are CRC

    device_id = (uint8_t)atoi(argv[1]);
    function_code = (uint8_t)atoi(argv[2]);
    reg = (uint16_t)atoi(argv[3]);
    value = (uint16_t)atoi(argv[4]);

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

    if (device_id == ID_LIGHT_SENSOR) {
        read_light_sensor(file);
    }
    else if (device_id == ID_MOTOR) {
        ask_motor_speed(file);
    }
    else {
        printf("Unknown device ID: %d\n", device_id);
        return -1;
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

int read_light_sensor(int file){
    // for now, we just want to send a message to check communication by sending message like 2 3 0 0 and sent back the same message
    uint8_t msg[MSG_LEN];
    uint8_t rsp[MSG_LEN];

    msg[0] = device_id;
    msg[1] = function_code;
    msg[2] = reg >> 8; // take first high bits
    msg[3] = reg & 0xFF; // take low bits
    msg[4] = value >> 8;
    msg[5] = value & 0xFF;

    uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
    uint8_t crc1 = crc >> 8;
    uint8_t crc2 = crc & 0xFF;

    msg[6] = crc1;
    msg[7] = crc2;
    // msg[6] = 0; // you can uncomment those lines to check if the arduino is ignoting a wrong message
    // msg[7] = 0;

    // printf the message
    printf("send: %x %x %x %x %x %x %x %x\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7]);
    ssize_t count = write(file, msg, MSG_LEN);
    if (count < 0) {
        perror("Failed to write to the output\n");
        return -1;
    }

    // printf("Sent %zd bytes: Device ID: %d, Function Code: %d, Register: %d, Value: %d, CRC: %d\n", count, device_id, function_code, reg, value, crc);
    usleep(200000);

    // get response
    count = read(file, (void*)rsp, MSG_LEN);
    if (count < 0) {
        perror("Failed to read from the input\n");
        return -1;
    }

    printf("count is %d\n", count);

    printf("received %x %x %x %x %x %x %x %x\n", rsp[0], rsp[1], rsp[2], rsp[3], rsp[4], rsp[5], rsp[6], rsp[7]);

    // add CRC check using ModRTU_CRC function
    crc = ModRTU_CRC(rsp, MSG_LEN - 2);
    crc1 = crc >> 8;
    crc2 = crc & 0xFF;

    rsp[6] = crc1;
    rsp[7] = crc2;

    // verify the crc of message
    if (crc1 != rsp[6] || crc2 != rsp[7]) {
        printf("CRC check failed!\n");
        return -1;
    }
    else {
        printf("CRC check passed!\n");
    }


    if (count == 0) {
        printf("There was no data available to read!\n");
    }
    else if (count == 8) {
        printf("Light sensor value: %u\n", (rsp[4] << 8) | rsp[5]);
    }
    else {
        printf("Unexpected response length: %zd\n", count);
    }

    return 0;
}

int ask_motor_speed(int file){
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
    printf("crc1: %u, crc2: %u\n", crc1, crc2);

    msg[6] = crc1;
    msg[7] = crc2;
    // msg[6] = 0; // you can uncomment those lines to check if the arduino is ignoting a wrong message
    // msg[7] = 0;

    ssize_t count = write(file, msg, MSG_LEN);
    if (count < 0) {
        perror("Failed to write to the output\n");
        return -1;
    }

    printf("Sent %zd bytes: Device ID: %d, Function Code: %d, Register: %d, Value: %d, CRC: %d\n", count, device_id, function_code, reg, value, crc);
    printf("send: %x %x %x %x %x %x %x %x\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7]);

    usleep(200000);

    //get response

    count = read(file, (void*)rsp, MSG_LEN);
    if (count < 0) {
        perror("Failed to read from the input\n");
        return -1;
    }

    printf("resp: %x %x %x %x %x %x %x %x\n", rsp[0], rsp[1], rsp[2], rsp[3], rsp[4], rsp[5], rsp[6], rsp[7]);


    // add CRC check using ModRTU_CRC function
    crc = ModRTU_CRC(rsp, MSG_LEN - 2);
    crc1 = crc >> 8;
    crc2 = crc & 0xFF;

    rsp[6] = crc1;
    rsp[7] = crc2;

    // verify the crc of message
    if (crc1 != rsp[6] || crc2 != rsp[7]) {
        printf("CRC check failed!\n");
        return -1;
    }
    else {
        printf("CRC check passed!\n");
    }

    if (count == 0) {
        printf("There was no data available to read!\n");
        return -1;
    }
    else if (count == 8) {
        printf("motor speed set \n");
    }
    else {
        printf("Unexpected response length: %zd\n", count);
        return -1;
    }
    return 0;
}
