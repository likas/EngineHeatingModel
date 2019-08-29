#include "engine.h"

void Engine_internal_combustion::piesewise_linear_function::print() {
	std::cout << "M = " << k << "V + " << b << ", V in [" << a1 << "][" << a2 << "]" << std::endl;
}

Engine_internal_combustion::Engine_internal_combustion(double i, double hm, double hv, double c, std::vector<double> M, std::vector<double> V) :Hm(hm), Hv(hv), C(c), Vi(0), Mi(0) {
	I = i;

	// �������� �������� ������������� ����������� �� ��������� ���
	world& world1 = world::instance();
	T_current_engine = world1.get_env_T();

	/*�������-�������� ������� ����������� ���
										y(x) =  k1 * x + b1, x in [a1, a2}
												k2 * x + b2, x in [a2, a3}
												...
												ki * x + bi, x in [ai, ai+1}
	��� ��� �� ����� ����� �������� � ������, �������� ������������ k � b, ��������������� �������� �������. ��� ����� ����� ���� (������� ������-�������):
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
		}
		else {
			std::stringstream exception;
			exception << "No solutions to the equation on the inteval [" << V[i] << "][" << V[i + 1] << "]! Check the input data." << std::endl;
			throw exception.str();
		}
	}

}

void Engine_internal_combustion::step() {
	Vc = getVc();
	Vh = getVh();
	double deltaT = Vc + Vh;
	T_current_engine += deltaT;
}

double Engine_internal_combustion::getT() {
	return T_current_engine;
}

double Engine_internal_combustion::getVc() {
	world& world1 = world::instance();
	double T_env = world1.get_env_T();
	return C * (T_env - T_current_engine);
}

double Engine_internal_combustion::getVi() {
	return Vi + (Mi / I);
}

double Engine_internal_combustion::getVh() {

	//----------------------- ���������� �������� �� ���� ���� ------------------------
	Vi = getVi();

	//----------------- �������� ��������, � �������� ����������� Vi ------------------
	short i = 0;
	for (size_t length = intervals.size(); i < length; ++i) {
		if (Vi >= intervals[i].a1 && Vi < intervals[i].a2) break;
	}

	//------ ���������� k � b ��� ���������� ��������� � ��������� M = kV + b ---------
	Mi = intervals[i].k * Vi + intervals[i].b;

	//��������� Vh = M * Hm + V^2 * Hv
	return Mi * Hm + (Vi * Vi) * Hv;
}