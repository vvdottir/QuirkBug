#ifndef Oscillator_h_
#define Oscillator_h_

#include <Arduino.h>

#include "Bot.h"
#include "IntervalNode.h"
#include "Input.h"
#include "Output.h"
#include "ContainsInputs.h"
#include "ContainsOutputs.h"
#include "Streams.h"
#include "OscillatorTables.h"

enum OscillatorType{
	WAVE_SINE = 0,
	WAVE_SQUARE,
	WAVE_TRIANGLE,
	WAVE_PULSE,
	WAVE_RAMP_UP,
	WAVE_RAMP_DOWN
};


class Oscillator :
public IntervalNode,
public OutputStream<float>,
public Contains6Inputs<float,float,float,float,float,float>,
public Contains1Output<float>
{
	public:

	Oscillator():
	OutputStream<float>
		(value),
	Contains6Inputs<float,float,float,float,float,float>
		(begin, end, duration, offset, type, interval),
	Contains1Output<float>
		(value){
		registerInput(begin);
		registerInput(end);
		registerInput(duration);
		registerInput(offset);
		registerInput(type);

		begin = 0.0;
		end = 1.0;
		type = WAVE_SINE;
		duration = 1.0;
		interval = 33;
		offset = 0.0;

		table = WAVE_SINE_TABLE;
		adjust = 0;
		position = 0;
	};

	void onInterval();

	Input<float> begin;
	Input<float> end;

	Input<float> duration;
	Input<float> offset;
	Input<float> type;
	
	Output<float> value;

	protected:

	const int16_t * table;
	
	void onInternalInputChange(BaseInput &input);

	float adjust;
	float position;
};
typedef Oscillator Wave; 

void Oscillator::onInternalInputChange(BaseInput &input){
	if(&input == &type){
		switch ((int)type){
			case WAVE_SQUARE:
				table = WAVE_SQUARE_TABLE;
				break;
			case WAVE_PULSE:
				table = WAVE_PULSE_TABLE;
				break;
			case WAVE_TRIANGLE:
				table = WAVE_TRIANGLE_TABLE;
				break;
			case WAVE_RAMP_UP:
				table = WAVE_RAMP_UP_TABLE;
				break;
			case WAVE_RAMP_DOWN:
				table = WAVE_RAMP_DOWN_TABLE;
				break;
			case WAVE_SINE:
			default:
				table = WAVE_SINE_TABLE;
				break;
		}
	}
	else if(&input == &duration){
		float basePosition = position - offset;
		if(basePosition < 0) basePosition += 1;

		float currentTime = fmod(Bot::millis, duration);
		float diff = currentTime/duration - basePosition;
		adjust = diff * duration;
	}
};

void Oscillator::onInterval(){
	float timeMillis = fmod(Bot::millis -adjust + offset * duration, duration);
	position = timeMillis / duration;
	int index = position * 256.0;
	float base = (float)(pgm_read_word_near(table + index)) * 0.001;
	value = mapFloat(base, 0, 1.0, begin, end);
}

#endif
