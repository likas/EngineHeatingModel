#ifndef _STAND_H_
#define _STAND_H_

#include "engine.h"
#include <cmath>

//абстрактный класс Stand и его наследник Stand_heating_stand
class Stand {
protected:
	int time; //время симуляции
	void step(Engine* e);
};

class Stand_heating_stand : public Stand {
private:
	double epsilon;
	double T_overheat;
	double T_cur;
public:
	Stand_heating_stand(double eps, double overheat) :epsilon(eps), T_overheat(overheat), T_cur(0) {};
	int test(Engine* e);
};

#endif // _STAND_H_