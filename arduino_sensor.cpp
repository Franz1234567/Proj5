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

uint16_t ModRTU_CRC(uint8_t buf[], int len){
   uint16_t crc = 0xFFFF;
   for (int pos = 0; pos < len; pos++) {
      crc ^= (uint16_t)buf[pos];
      for (int i = 8; i != 0; i--) { // Loop over each bit
         if ((crc & 0x0001) != 0) { // If the LSB is set
            crc >>= 1; // Shift right and XOR 0xA001
            crc ^= 0xA001;
         }
         else // Else LSB is not set
            crc >>= 1; // Just shift right
      }
   }
   return crc;
}

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
      if(id == ID_LIGHT_SENSOR){
         if(my_register == 0x01){   // right register ?
		   uint16_t computed_crc = ModRTU_CRC(msg, MSG_LEN - 2);
		   if(crc==computed_crc){
			data = analogRead(sensorPin);
			int led_intensity = (int)data*255/1024.0;
			analogWrite(ledPin, led_intensity);
			for(int i; i<MSG_LEN; i++){
				buffer[i] = msg[i];
			}
			buffer[4] = data >> 8;
			buffer[5] = data & 0xFF;
			uint16_t crc = ModRTU_CRC(buffer, MSG_LEN - 2);
			uint8_t crc1 = crc >> 8;
			uint8_t crc2 = crc & 0xFF;
			buffer[6] = crc1;
			buffer[7] = crc2;
			Serial.write(buffer, MSG_LEN);               // send the buffer to the RPi
			}
		 }
      }
   }
}