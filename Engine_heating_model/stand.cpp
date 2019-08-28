#include "engine.h"

class Stand {
	int time;
	void step(Engine e) {
		e.step();

	}
};

class heating_stand : public Stand {

};