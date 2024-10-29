#include <Arduino.h>
#include <stdint.h>
#include <util/delay.h>

#define MSG_LEN 8
#define ID_MOTOR 0x01
#define ID_LIGHT_SENSOR 0x02
#define WRITE 0x06
#define READ 0x03

/* Arduino Serial Command Processor */

int ledPin = 11;          // LED with PWM brightness control
int sensorPin = A7;

void setup() {            // called once on start up
   // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
   Serial.begin(115200, SERIAL_8N1);
   pinMode(ledPin, OUTPUT);         // the LED is an output
   pinMode(sensorPin, INPUT);
}

void loop() {              // loops forever
   uint8_t buffer[MSG_LEN];       // stores the return buffer on each loop
   uint8_t msg[MSG_LEN];
   uint8_t id, function, my_register1, my_register2, data1, data2, crc1, crc2;

   if (Serial.available()>0){                 // bytes received
      Serial.readBytes(msg, MSG_LEN); //message with fixed length
      id = msg[0];
      function = msg[1];
      my_register1 = msg[2];
      my_register2 = msg[3];
      uint16_t my_register = (my_register1 << 8) | my_register2;
      data1 = msg[4];
      data2 = msg[5];
      uint16_t data = (data1 << 8) | data2;
      crc1 = msg[6];
      crc2 = msg[7];
      uint16_t crc = (crc1 << 8) | crc2;
      if(id == ID_MOTOR){
         if(my_register == 0x01){   // right register ?
            if(data>=0 && data<=255){          // is it in range?
               analogWrite(ledPin, data);       // yes, write out
               for(int i; i<MSG_LEN; i++){
                  buffer[i] = msg[i];
               }
               Serial.write(buffer, MSG_LEN);
            }
         }
      } 
   }
}