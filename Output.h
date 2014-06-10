#ifndef Output_h_
#define Output_h_

#include "Event.h"

template <class T>
class Output {
	public:

	void set(T value){
		if(this->value == value) return;
		this->value = value;
		changed.dispatch(value);
	}
	void operator=(T value){
		set(value);
	}	

	T get(){
		return value;
	}
	operator T(){
		return get();
	}

	Event<T> changed;

	private:

	T value;
		
};

#endif
