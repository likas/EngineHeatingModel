#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <stdexcept>      // std::invalid_argument

#include "utilities.h"
#include "engine.h"

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

int main(int args, char *argv[]) {

	try {
		//-------- параметры из файла ---------
		double I = 0; // момент инерции
		vector<double> M; //значения M - крутящего момента
		vector<double> V; //значения V - скорости вращения коленвала
		double T_overheat = 0; //температура перегрева
		double Hm = 0; //heating coefficient, dependency of torque M				//коэфф-т зав-ти нагрева H от крут.мом-та M
		double Hv = 0; //heating coefficient, dependency of crankshaft velocity V	//коэфф-т зав-ти нагрева H от ск-ти к.вала V
		double C = 0;  //cooling coefficient, dependent of T_engine_current and T_environment
		double eps;
		//-------- пользовательский ввод -------
		I = 10;
		M.insert(end(M), { 20,75,100,105,75,0 });
		V.insert(end(V), { 0,75,150,200,250,300 });
		T_overheat = 110;
		Hm = 0.01;
		Hv = 0.0001;
		C = 0.1;
		eps = 0.00001;
		//---------- с клавиатуры ------------ //TODO проверить, не меньше ли текущая температура чем перегрева
		double T_environment = 0;
		//T_environment = 13;

		//--------------- мини-валидация -------------------

		if (I < 0) throw "I must be positive";
		if (Hm < 0) throw "Hm must be positive";
		if (Hv < 0) throw "Hv must be positive";
		if (C < 0) throw "C must be positive";
		if (eps < 0) throw "Epsilon must be positive";

		if ( M.size() != V.size() && M.size() != 0 && V.size() != 0 ) throw ("M and V sizes must be equal and greater than 1"); //check sizes

		for (auto i : M) {
			if (i < 0) throw ((char)i + " element of M array is negative; should be equal to zero or greater");
		}

		for (auto i : V) {
			if (i < 0) throw ((char)i + " element of V array is negative; should be equal to zero or greater");
		}

		for (size_t i = 1; i < V.size(); ++i) {
			if (V[i] < V[i - 1]) {
				//cout << "Lo: There the exception be" << endl;
				std::stringstream exception;
				exception << "Your intervals have an intersection: " << V[i - 1] << " is greater than " << V[i] << std::endl;
				throw exception.str();
			}
		}

		//------------- пользовательский ввод -------------

		string user_input = "";
		cin >> user_input;
		std::string::size_type sz;
		try {
			T_environment = stof(user_input, &sz);
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << '\n';
			std::cerr << "Incorrect input : environment temperature should be integer or float";
			return EXIT_FAILURE;
		}

		if (is_null_pointer<decltype(sz)>::value) {
			throw "Incorrect input: environment temperature should be integer or float";
		}
		
		Engine_internal_combustion engine(I, T_environment, Hm, Hv, C, M, V);
		cout << "T = " << engine.getT(T_environment) << endl; 
		
		std::cout << "End of program" << std::endl;
		return EXIT_SUCCESS;
	}
	catch(string exception){
		cout << "Excepion occured: " << exception << endl;
		return EXIT_FAILURE;
	}
}