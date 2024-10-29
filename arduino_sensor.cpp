// #include <Arduino.h>
// #include <stdint.h>
// #include <util/delay.h>

// #define MSG_LEN 8
// #define ID_MOTOR 0x01
// #define ID_LIGHT_SENSOR 0x02
// #define WRITE 0x06
// #define READ 0x03

// /* Arduino Serial Command Processor */

// int ledPin = 11;          // LED with PWM brightness control
// int sensorPin = A7;

// void setup() {            // called once on start up
//    // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
//    Serial.begin(115200, SERIAL_8N1);
//    pinMode(ledPin, OUTPUT);         // the LED is an output
//    pinMode(sensorPin, INPUT);
// }

// void loop() {              // loops forever
//    uint8_t buffer[MSG_LEN];       // stores the return buffer on each loop
//    uint8_t msg[MSG_LEN];
//    uint8_t id, function, my_register1, my_register2, data1, data2, crc1, crc2;

//    if (Serial.available()>0){                 // bytes received
//       Serial.readBytes(msg, MSG_LEN); //message with fixed length
//       id = msg[0];
//       function = msg[1];
//       my_register1 = msg[2];
//       my_register2 = msg[3];
//       uint16_t my_register = (my_register1 << 8) | my_register2;
//       data1 = msg[4];
//       data2 = msg[5];
//       uint16_t data = (data1 << 8) | data2;
//       crc1 = msg[6];
//       crc2 = msg[7];
//       uint16_t crc = (crc1 << 8) | crc2;
//       if(id == ID_LIGHT_SENSOR){
//          if(my_register == 0x01){   // right register ?
//             //data = analogRead(sensorPin);
//             data = 15;
//             int led_intensity = (int)data*255/1024.0;
//             analogWrite(ledPin, led_intensity);
//          }
//          buffer[0] = (uint8_t)0;
//          buffer[1] = (uint8_t)1;
//          buffer[2] = (uint8_t)2;
//          buffer[3] = (uint8_t)3;
//          buffer[4] = (uint8_t)4;
//          buffer[5] = (uint8_t)5;
//          buffer[6] = (uint8_t)6;
//          buffer[7] = (uint8_t)7;
//          Serial.write(buffer, MSG_LEN);               // send the buffer to the RPi
//          _delay_ms(100);
//       }
//       // for(int i; i<MSG_LEN; i++){
//       //    buffer[i] = msg[i];
//       // }
//    }
// }