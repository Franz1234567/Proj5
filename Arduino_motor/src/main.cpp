#include <avr/interrupt.h>
#include "global.h"

// basic values that can be resetted in pre-operational state
double Kp = 0.0135;
double Ti = 1.66;
double T = 0.01;

// global variables 
Encoder encA(1);
Encoder encB(2);

Digital_out led(5);
Analog_out analog(4);
PI_Controller control;
Digital_in fault(3);

int max_speed; 
int current_speed;
double ref; // reference speed
double duty_cycle_first; //original duty cycle

bool last_state_A;
bool last_state_B;
bool curr_state_A;
bool curr_state_B;

Timer_msec timer_speed;
Timer_msec timer_pulses;

int command = 0; // for incoming serial data
int command_break = 0;
Context* context;

uint8_t msg[MSG_LEN];
uint8_t id, function_code, my_register1, my_register2, data1, data2, crc1, crc2;

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

void setup()
{
  Serial.begin(115200, SERIAL_8N1); // opens serial port, sets data rate to 9600 bps
  context = new Context(new InitState);
}

void loop()
{

  if (Serial.available() > 0){
    Serial.readBytes(msg, MSG_LEN);
    id = msg[0];
    function_code = msg[1];
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
      if(my_register == 0 ){
        uint16_t computed_crc = ModRTU_CRC(msg, MSG_LEN - 2);
			  if(crc==computed_crc){
          if((function_code == 1) || (function_code == 2) || (function_code == 80) || (function_code == 81) || (function_code == 82)){
            context->event(function_code);
          }
        }
      }
    }
  }
}


