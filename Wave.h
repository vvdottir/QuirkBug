#ifndef Wave_h_
#define Wave_h_

#include <Arduino.h>

#include "Bot.h"
#include "Node.h"
#include "HasInterval.h"
#include "Input.h"
#include "Output.h"
#include "Streams.h"
#include "WaveTables.h"
#include "ContainsInputs.h"


#define	WAVE_SINE 0
#define	WAVE_SQUARE 1
#define	WAVE_TRIANGLE 2
#define	WAVE_PULSE 3
#define	WAVE_RAMP_UP 4
#define	WAVE_RAMP_DOWN 5



class Wave :
public Node,
public HasInterval,
public Contains2Inputs<float, float>,
public OutputStream<float>
{
	public:

	Wave():
	HasInterval
		(this),
	Contains2Inputs<float, float>
		(min, max),
	OutputStream<float>
		(value){
		registerInput(min);
		registerInput(max);
		registerInput(duration);
		registerInput(offset);
		registerInput(type);

		interval = 0.033;

		position = 0;
		offset = 0.0;
		min = 0.0;
		max = 1.0;
		type = WAVE_SINE;		
		duration = 1.0;
	};

	void onInterval();

	Input<float> min;
	Input<float> max;

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

void Wave::onInternalInputChange(BaseInput &input){
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

		float currentTime = fmod(Bot::seconds, duration);
		float diff = currentTime/duration - basePosition;
		adjust = diff * duration;
	}
};

void Wave::onInterval(){
	float timeSeconds = fmod(Bot::seconds -adjust + offset * duration, duration);
	position = timeSeconds / duration;
	int index = position * 256.0;
	float base = (float)(pgm_read_word_near(table + index)) * 0.001;
	value = mapFloat(base, 0, 1.0, min, max);
}

#endif
