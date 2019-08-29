#include "engine.h"
#include <cmath>

class Stand {
	int time;
public:
	void step(Engine* e) {
		e->step();
	}
};

class Stand_heating_stand : public Stand {
private:
	double epsilon;
	double T_overheat;
	double T_cur;
	double T_prev;
public:
	Stand_heating_stand(double eps, double overheat) :epsilon(eps), T_overheat(overheat), T_cur(0), T_prev(0) {};
	int test(Engine* e) {
		T_cur = e->getT();
		float delta_T = T_cur;
		int t = 0;
		for (; T_cur < T_overheat && delta_T > epsilon; ++t) {
			T_prev = T_cur;
			step(e);
			T_cur = e->getT();
			//cout << 
			cout << "Temp. now: " << T_cur << endl;
			delta_T = abs(T_prev - T_cur);
			cout << "Delta T now: " << delta_T << endl;
		}
		return t;
	}
};