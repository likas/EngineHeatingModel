#ifndef _STAND_H_
#define _STAND_H_

#include "engine.h"
#include <cmath>

//класс Stand и его наследник Stand_heating_stand
class Stand {
protected:
	int time;				//врем€ симул€ции
	void step(Engine* e);	//на каждом шаге значение температуры измен€етс€ в соответствии со внутренним состо€нием объекта
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