#ifndef _IOSTREAM_DEFINE
#include <iostream>
#define _IOSTREAM_DEFINE
#endif
#include <fstream>
#ifndef _VECTOR_INCLUDE
#include <vector>
#define _VECTOR_INCLUDE
#endif
#include <map>
#include "utilities.h"
#include "test_stand.h"

using namespace std;

int gaussian(std::vector<std::vector<double>>, std::vector<double>&);
double getVc(double T_eng_cur, double T_env, double c);
double getVi(double V_prev, double M_prev, double i);
/*
class system_parameters {
	double T_engine_current;
	double Vh;
	double Vc;
	double M_nextstep;
	double M_current;
	double Vi;
public:
	system_parameters(int t_env, double m_cur) {
		T_engine_current = t_env;
		Vh = 0;
		Vc = 0;
		M_current = m_cur;
		M_nextstep = 0;
		Vi = 0;
	}
	
};*/

int main(int args, char *argv[]) {
	//-------- параметры из файла ---------
	int I = 0; // момент инерции
	vector<double> M; //значения M - крутящего момента
	vector<double> V; //значения V - скорости вращения коленвала
	int T_overheat = 0; //температура перегрева
	double Hm = 0; //heating coefficient, dependency of torque M				//коэфф-т зав-ти нагрева H от крут.мом-та M
	double Hv = 0; //heating coefficient, dependency of crankshaft velocity V	//коэфф-т зав-ти нагрева H от ск-ти к.вала V
	double C = 0;  //cooling coefficient, dependent of T_engine_current and T_environment
	//-------- пользовательский ввод -------
	int T_environment = 0;

	//читаем файл

	std::ifstream in("C:/Users/user/source/repos/Forward_development/Engine_heating_model_(test_assignment)/Engine_heating_model/Engine_heating_model/input.txt"); // окрываем файл для чтения
	if (in.is_open())
	{
		/*		while (getline(in, line))
				{
					std::cout << line << std::endl;
				}
			}
			*/
		in.close();     // закрываем файл

	}

	//зададим пока статически
	I = 10;
	M.insert(end(M), { 20,75,100,105,75,0 });
	V.insert(end(V), { 0,75,150,200,250,300 });
	T_overheat = 110;
	Hm = 0.01;
	Hv = 0.0001;
	C = 0.1;
	T_environment = 30;

	//тут будет расчет коэффициентов
	/*vector < vector <double >> matrix = { {0,1,20}, {75,1,75} };
	cout << matrix[0][0] << " " << matrix[0][1] << "\n" << matrix[1][0] << " " << matrix[1][1] << endl;

	vector <double> answer;

	cout << "gaussian ret. " << gaussian(matrix, answer) << endl;
	cout << answer[0] << " " << answer[1] << endl;
	*/
	vector < vector <double >> matrix;
	vector<piesewise_linear_function> intervals;
	vector <double> answer;

	for (short i = 0; i < M.size() - 1 ; ++i) {
		piesewise_linear_function temp_interval;
		temp_interval.x1 = V[i]; temp_interval.x2 = V[i + 1];
		matrix = { {V[i], 1, M[i]}, {V[i + 1], 1, M[i + 1]} };
		if (gaussian(matrix, answer) == 0) {
			temp_interval.k = answer[0]; temp_interval.b = answer[1];
			intervals.push_back(temp_interval);
			intervals[i].print();
		}
		else {
			cout << "No solutions to the equation on the inteval [" << V[i] << "][" << V[i + 1] << "]! Check the input data." << endl;
		}
	}

	double T_engine_current = T_environment;
	double Vh = 0;
	double Vc = 0;
	double M_prev = M[0];
	double M_current = M[0];
	double Vi = V[0];
	//system_parameters system1(30, M[0]);

	for (short t = 0 ; T_engine_current < T_overheat; ) {
		cout << "Testing..." << endl;

		Vi = getVi(Vi, M_current, I);
		Vc = getVc(T_engine_current, T_environment, C);
		//----------- there's your getVh ------

		//-------- выяснить интервал, к которому принадлежит Vi ---------
		short i = 0;
		for ( short length = intervals.size() ; i < length; ++i) {
			if (Vi < intervals[i].x2) break;
		}

		//------ подставить k и b для найденного интервала в уравнение M = kV + b ---------

		M_current = intervals[i].k * Vi + intervals[i].b;
		M_prev = M_current; // вроде прев не нужен - когда находим V, М у нас равна предыдущему; когда мы нашли м текущее, v уже найдено и пред.значение м не нужно

		//посчитать Vh = M * Hm + V^2 * Hv
		Vh = M_current * Hm + (Vi * Vi) * Hv;

		//----------- end -------------

		T_engine_current = T_engine_current + Vh - Vc;
		cout << "step " << t << ": t = " << T_engine_current << endl;
		t++;
	}

	/*if (gaussian(matrix, answer) == 1) {
		cout << answer[0] << " " << answer[1] << endl;
	}*/

	

	//всё статически!! ><

	std::cout << "End of program" << std::endl;
	return EXIT_SUCCESS;
}


double getVc(double T_eng_cur, double T_env, double c) {
	return c * (T_env - T_eng_cur);
}

double getVi(double V_prev, double M_prev, double i) {
	return V_prev + (M_prev / i);
}


//---------------- gaussian --------------------

int gaussian(std::vector<std::vector<double>> a, std::vector<double>& ans) {
	int n = (int)a.size();
	int m = (int)a[0].size() - 1;
	double eps = 0.000001;

	cout << "in gaussian:" << endl;
	cout << a[0][0] << " " << a[0][1] << endl;
	cout << a[1][0] << " " << a[1][1] << endl;

	std::vector<int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; ++col) {
		int sel = row;
		for (int i = row; i < n; ++i)
			if (abs(a[i][col]) > abs(a[sel][col]))
				sel = i;
		if (abs(a[sel][col]) < eps)
			continue;
		for (int i = col; i <= m; ++i)
			std::swap(a[sel][i], a[row][i]);
		where[col] = row;

		for (int i = 0; i < n; ++i)
			if (i != row) {
				double c = a[i][col] / a[row][col];
				for (int j = col; j <= m; ++j)
					a[i][j] -= a[row][j] * c;
			}
		++row;
	}

	ans.assign(m, 0);
	for (int i = 0; i < m; ++i)
		if (where[i] != -1)
			ans[i] = a[where[i]][m] / a[where[i]][i];
	for (int i = 0; i < n; ++i) {
		double sum = 0;
		for (int j = 0; j < m; ++j)
			sum += ans[j] * a[i][j];
		if (abs(sum - a[i][m]) > eps)
			return 0;
	}

	for (int i = 0; i < m; ++i)
		if (where[i] == -1)
			return -1;
	return EXIT_SUCCESS;
}