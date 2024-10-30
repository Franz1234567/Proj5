#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define ID_MOTOR 1
#define ID_LIGHT_SENSOR 2

const size_t MSG_LEN = 8;

uint8_t device_id;
uint8_t function_code;
uint16_t reg;
uint16_t value;
uint16_t crc;

int send_modbus_message(int file, uint8_t device_id, uint8_t function_code, uint16_t reg, uint16_t value);

uint16_t ModRTU_CRC(uint8_t buf[], int len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];
        for (int i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int main(int argc, char *argv[]) {
    int file;

    if (argc != 5) {
        printf("Usage: %s <device_id> <function_code> <register> <value>\n", argv[0]);
        return -2;
    }

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
    options.c_cc[VMIN] = 8;
    tcflush(file, TCIFLUSH);
    tcsetattr(file, TCSANOW, &options);

    int result = send_modbus_message(file, device_id, function_code, reg, value);
    close(file);
    return result;
}

int send_modbus_message(int file, uint8_t device_id, uint8_t function_code, uint16_t reg, uint16_t value) {
    uint8_t msg[MSG_LEN];
    uint8_t rsp[MSG_LEN];

    msg[0] = device_id;
    msg[1] = function_code;
    msg[2] = reg >> 8;
    msg[3] = reg & 0xFF;
    msg[4] = value >> 8;
    msg[5] = value & 0xFF;

    uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
    msg[6] = crc >> 8;
    msg[7] = crc & 0xFF;

    printf("send: %x %x %x %x %x %x %x %x\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7]);

    ssize_t count = write(file, msg, MSG_LEN);
    if (count < 0) {
        perror("Failed to write to the output\n");
        return -1;
    }

    usleep(200000);
    count = read(file, (void*)rsp, MSG_LEN);
    if (count < 0) {
        perror("Failed to read from the input\n");
        return -1;
    }

    printf("received %x %x %x %x %x %x %x %x\n", rsp[0], rsp[1], rsp[2], rsp[3], rsp[4], rsp[5], rsp[6], rsp[7]);

    crc = ModRTU_CRC(rsp, MSG_LEN - 2);
    if ((crc >> 8) != rsp[6] || (crc & 0xFF) != rsp[7]) {
        printf("CRC check failed!\n");
        return -1;
    } else {
        printf("CRC check passed!\n");
    }

    if (count == 0) {
        printf("No data available to read!\n");
        return -1;
    } else if (count == 8) {
        if (device_id == ID_LIGHT_SENSOR) {
            printf("Light sensor value: %u\n", (rsp[4] << 8) | rsp[5]);
        } else if (device_id == ID_MOTOR) {
            printf("Motor speed set\n");
        }
    } else {
        printf("Unexpected response length: %zd\n", count);
        return -1;
    }
    return 0;
}
