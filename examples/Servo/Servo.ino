#include <QuirkBug.h>

#include <Wave.h>
#include <Servo.h>
#include <Logger.h>


Wave osc;
Servo servo;

SerialPrint print;

void setup() {	

	osc.type = OSC_RAMP_UP;
	osc.duration = 2;
	
	servo.pin = 12;
	servo.input.connect(osc.output);
}


void loop(){

}