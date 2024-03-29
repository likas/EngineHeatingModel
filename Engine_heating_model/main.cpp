#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <stdexcept>

#include "utilities.h"
#include "engine.h"
#include "world.h"
#include "stand.h"

using namespace std;

int main(int argc, char *argv[]) {

	try {
		//-------- ��������� �� ����� ---------
		double I = 0; // ������ �������
		vector<double> M; //�������� M - ��������� �������
		vector<double> V; //�������� V - �������� �������� ���������
		double T_overheat = 0; //����������� ���������
		double Hm = 0; //heating coefficient, dependency of torque M				//�����-� ���-�� ������� H �� ����.���-�� M
		double Hv = 0; //heating coefficient, dependency of crankshaft velocity V	//�����-� ���-�� ������� H �� ��-�� �.���� V
		double C = 0;  //cooling coefficient, dependent of T_engine_current and T_environment
		double eps;
		//------------ ���������������� ���� ---------------
		I = 10;
		M.insert(end(M), { 20,75,100,105,75,0 });
		V.insert(end(V), { 0,75,150,200,250,300 });
		T_overheat = 110;
		Hm = 0.01;
		Hv = 0.0001;
		C = 0.1;
		eps = 0.00001;
		//---------------- � ���������� -------------------- 
		double T_environment = 0;

		//--------------- error handling -------------------
		if (Hm < 0) throw "Hm should be positive";
		if (Hv < 0) throw "Hv should be positive";
		if (C < 0) throw "C should be positive";

		if (I < 0) throw "I must be positive";
		if (eps < 0) throw "Epsilon must be positive";

		if (M.size() != V.size() && M.size() != 0 && V.size() != 0) throw ("M and V sizes must be equal and greater than 1");

		for (auto i : M) {
			if (i < 0) throw ((char)i + " element of M array is negative; must be equal to zero or greater");
		}

		for (auto i : V) {
			if (i < 0) throw ((char)i + " element of V array is negative; should be equal to zero or greater");
		}

		for (size_t i = 1; i < V.size(); ++i) {
			if (V[i] < V[i - 1]) {
				std::stringstream exception;
				exception << "Your intervals have an intersection: " << V[i - 1] << " is greater than " << V[i] << std::endl;
				throw exception.str();
			}
		}

		//---------------- ���������������� ���� ---------------
		string user_input = "";
		cin >> user_input;

		//---- error handling ����������������� ����� ----------
		std::string::size_type sz;
		try {
			T_environment = stof(user_input, &sz);
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << '\n';
			throw "Incorrect input : environment temperature should be integer or float";
		}

		if (is_null_pointer<decltype(sz)>::value) {
			throw "Incorrect input: environment temperature should be integer or float";
		}

		if (T_environment > T_overheat) {
			std::stringstream exception;
			exception << "Overheat temp. (" << T_overheat << ") is less than the environment temp. (" << T_environment << "). Looks like the engine is already overheated!" << endl;
			throw exception.str();
		}
		// ------------------ ������������� ���� ---------------
		world& world1 = world::instance();
		world1.set_env_T(T_environment);
		
		Engine_internal_combustion engine1(I, Hm, Hv, C, M, V);					// ��������� � �����������
		Stand_heating_stand stand1(eps, T_overheat);							// �������� �����
		cout << "Overheat took " << stand1.test(&engine1) << " sec." << endl;	// ��������� ��������� �� ������ � ������� ����������
		
		return EXIT_SUCCESS;
	}
	catch(string exception){
		cout << "Excepion occured: " << exception << endl;
		return EXIT_FAILURE;
	}
}