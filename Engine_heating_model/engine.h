#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <sstream>
#include <algorithm>
#include "utilities.h"
#include "world.h"

int gaussian(std::vector<std::vector<double>>, std::vector<double>&);

class Engine {
protected:
	double I;
	double T_current_engine;
	double Vh;
	double Vc;
public:
	virtual void step() = 0;
	virtual double getT() = 0;
};

class Engine_internal_combustion : public Engine {
private:
	const double Hm; //heating coefficient, dependency of torque M				//коэфф-т зав-ти нагрева H от крут.мом-та M
	const double Hv; //heating coefficient, dependency of crankshaft velocity V	//коэфф-т зав-ти нагрева H от ск-ти к.вала V
	const double C;  //cooling coefficient, dependent of T_engine_current and T_environment
	double Vi;
	double Mi;
	class piesewise_linear_function { //интервальная линейная функция, класс описывает один интервал. Имеет вид y = kx + b, y in [a1, a2}.
	public:
		double k;
		double b;
		double a1;
		double a2;
		void print();
	};
	std::vector<piesewise_linear_function> intervals;
protected:
	double getVc();
	double getVi();
	double getVh();
public:
	Engine_internal_combustion(double i, double hm, double hv, double c, std::vector<double> M, std::vector<double> V);

	void step();
	double getT();

};

#endif // _ENGINE_H_