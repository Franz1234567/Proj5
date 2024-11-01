#include "global.h"

void StoppedState::on_entry(){
    msg[4] = 0;
	msg[5] = function_code;
	uint16_t crc = ModRTU_CRC(msg, MSG_LEN - 2);
	crc1 = crc >> 8;
	crc2 = crc & 0xFF;
	msg[6] = crc1;
	msg[7] = crc2;

    Serial.write(msg, MSG_LEN);
	// Serial.println("Entering Stopped State");
    led.set_hi();
    analog.pin_digi.set_lo();
	on_do();
}

void StoppedState::on_exit(){
	// Serial.println("Exiting Stopped State");
}

void StoppedState::on_do(){
    command_break = 0;
    while(1){
        led.toggle();
        _delay_ms(250);
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
                if(my_register == 0){
                    uint16_t computed_crc = ModRTU_CRC(msg, MSG_LEN - 2);
                    if(crc == computed_crc){
                        if((function_code == 81) || (function_code == 1)||(function_code == 80)){
                            break;
                        }
                    }
                }
            }
        }
    }
    // Serial.print("---------> I received: ");
    // Serial.println((char)command_break);
    context_->event(function_code);
    }

void StoppedState::on_event(int event)
{
    if (event == 81){
	this->context_->transition_to(new InitState);
    }
    else if (event == 1){
        this->context_->transition_to(new OperationalState);
    }
    else if (event == 80){
        this->context_->transition_to(new PreOpState);
    }
}