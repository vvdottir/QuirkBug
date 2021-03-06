#ifndef Time_h_
#define Time_h_

#include "Bot.h"
#include "Node.h"
#include "HasInterval.h"
#include "Input.h"
#include "Output.h"
#include "ContainsInputs.h"
#include "Streams.h"

class Time :
public Node,
public HasInterval,
public OutputStream<float>
{
	public:

	Time():
	HasInterval
		(this),
	OutputStream<float>
		(seconds){
		interval = 0.001;
	};

	void onInterval();

	Output<float> seconds;
	Output<float> millis;
	Output<float> micros;
	Output<float> frames;
};

void Time::onInterval(){
	seconds.set(Bot::seconds);
	millis.set(Bot::millis);
	micros.set(Bot::micros);
	frames.set(Bot::frames);
}

#endif
