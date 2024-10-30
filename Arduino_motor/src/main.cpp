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
        if((function_code == 1) || (function_code == 2) || (function_code == 80) || (function_code == 81) || (function_code == 82)){
          msg[5] = function_code;
          // Serial.write(msg, MSG_LEN);
          context->event(function_code);
        }
      }
    }
  }
}


