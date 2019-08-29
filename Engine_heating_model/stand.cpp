#include "stand.h"

void Stand::step(Engine* e) { //шаг симул€ции. ƒвигатель рассчитывает свою температуру, использу€ знани€ о внутреннем представлении.
	e->step();
}

int Stand_heating_stand::test(Engine* e) {
	T_cur = e->getT();											//температура в выключенном виде
	time = 0;
	for (; T_cur < T_overheat; ++time) {
		step(e);
		if (abs(e->getT() - T_cur) < epsilon) {
			std::stringstream exception;
			exception << "Temperature change became very little. It should be because M tends to zero due to the features of piesewise linear function." << endl;
			exception << "Some values for reference: temperature was " << T_cur << ", time was " << time << " sec, overheat temp. " << T_overheat << endl;
			throw exception.str();
		}
		T_cur = e->getT();
	}
	return time;
}