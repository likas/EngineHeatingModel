#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include "utilities.h"
#include <sstream>
//#include <utility>
#include <algorithm>

int gaussian(std::vector<std::vector<double>>, std::vector<double>&);

class Engine {
protected:
	double I;
	double T_current_engine;
	double Vh;
	double Vc;
};

class Engine_internal_combustion : public Engine {
protected:
	double getVc(double T_env) {
		return C * (T_env - T_current_engine);
	}
	double getVi() {
		return min(300.0, Vi + (Mi / I));
	}
	double getVh() {

		//-------- выяснить интервал, к которому принадлежит Vi ---------
		short i = 0;
		for (size_t length = intervals.size(); i < length; ++i) {
			if (Vi < intervals[i].a2) break;
		}

		//------ подставить k и b для найденного интервала в уравнение M = kV + b ---------

		Mi = intervals[i].k * Vi + intervals[i].b;

		//посчитать Vh = M * Hm + V^2 * Hv
		return Mi * Hm + (Vi * Vi) * Hv;
	}
private:
	const double Hm; //heating coefficient, dependency of torque M				//коэфф-т зав-ти нагрева H от крут.мом-та M
	const double Hv; //heating coefficient, dependency of crankshaft velocity V	//коэфф-т зав-ти нагрева H от ск-ти к.вала V
	const double C;  //cooling coefficient, dependent of T_engine_current and T_environment
	double Vi;
	double Mi;
	class piesewise_linear_function { //интервальная линейная функция. имеет вид y = kx + b, y in [a1, a2}
	public:
		double k;
		double b;
		double a1;
		double a2; //y это M, x это V
		void print() {
			std::cout << "M = " << k << "V + " << b << ", V in [" << a1 << "][" << a2 << "]" << std::endl;
		}
	};
	std::vector<piesewise_linear_function> intervals;
public:
	//конструктор
	Engine_internal_combustion(double i, double t_cur, double hm, double hv, double c, std::vector<double> M, std::vector<double> V):Hm(hm), Hv(hv), C(c), Vi(0), Mi(0) {
		I = i;
		T_current_engine = t_cur;

		/*кусочно-линейная функция описывается как 
											y(x) =  k1 * x + b1, x in [a1, a2}
													k2 * x + b2, x in [a2, a3}
													...
													ki * x + bi, x in [ai, ai+1]
		так как на входе имеем значения в точках, вычислим коэффициенты k и b, характеризующие линейную функцию. Для этого решим СЛАУ (методом Гаусса-Жордана):
		*/
		std::vector < std::vector <double >> matrix;
		std::vector <double> answer;

		for (short i = 0; i < M.size() - 1; ++i) {
			piesewise_linear_function temp_interval;
			temp_interval.a1 = V[i]; temp_interval.a2 = V[i + 1];
			matrix = { {V[i], 1, M[i]}, {V[i + 1], 1, M[i + 1]} };
			if (gaussian(matrix, answer) == 0) {
				temp_interval.k = answer[0]; temp_interval.b = answer[1];
				intervals.push_back(temp_interval);
				intervals[i].print(); //d
			}
			else {
				std::stringstream exception;
				exception << "No solutions to the equation on the inteval [" << V[i] << "][" << V[i + 1] << "]! Check the input data." << std::endl;
				throw exception.str();
			}
		}
		
	}

	//методы
	
	double getT(double t_env) {
		Vc = getVc(t_env);
		Vh = getVh();
		double deltaT = Vc + Vh;
		T_current_engine += deltaT;
		return T_current_engine;
	}


};

#endif