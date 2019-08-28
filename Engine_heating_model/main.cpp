#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>

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
		//---------- с клавиатуры ------------
		double T_environment = 0;
		//T_environment = 13;


		//читаем файл
		json j;
		std::ifstream in("input.json"); // окрываем файл для чтения
		if (in.is_open())
		{
			in >> j;
			in.close(); // закрываем файл
		}

		cout << "nn " << j["H_v"].is_number() << endl;
		//return 0;

		//if (j["I"].is_number() && j["I"] >= 0 ) I = j["I"]; //standard check: number, positive
		//if (j["H_m"].is_number() && j["H_m"] >= 0) Hm = j["H_m"]; // --//--
		if (j["H_v"].is_number() && j["H_v"] >= 0) {  // --//--
			cout << "sdfsf" << endl;
			Hv = j["H_v"];
		}
		else {
			throw "H_v is incorrect"; 
		}
		//if (j["C"].is_number() && j["C"] >= 0) C = j["C"]; // --//--
		//if (j["overheat"].is_number() && j["overheat"] >= 0) T_overheat = j["overheat"]; // --//--
		//if (j["epsilon"].is_number() && j["epsilon"] >= 0) eps = j["epsilon"]; // --//--

		//check sizes

		if (j["M"].size() != j["V"].size()) throw ("M and V sizes must be equal");

		//check M
		if (j["M"].is_structured()) {
			for (size_t size = j["M"].size(), i = 0; i < size; ++i) {
				if (size < 1) {
					throw "Size of M array is less than 1";
				}
				if (j["M"][i].is_number() && j["M"][i] >= 0) { //standard check: number, positive
					M.push_back(j["M"][i]);
				}
				else throw ((char)i + " element of M array is NAN or is negative");
			}
		}
		else {
			throw "M is not a correct array\n";
		}

		//check V
		if (j["V"].is_structured()) {
			//for (size_t size = j["V"].size(), i = 0; i < size; ++i) {
			//for (auto [size, i] = std::tuple{ size_t::j["V"].size(), int::0 }; i < size; ++i) {
			for (size_t size = j["V"].size(), i = 0; i < size; ++i) {
				if (size < 1) {
					throw "Size of V array is less than 1";
				}
				if ( j["V"][i].is_number() && j["V"][i] >= 0 ) { //standard check: number, positive
					if (i == 0) {
					}
					else {
						//intervals are not correct if they are intersecting; if the next is lesser than previous, they do, throw an exeption
						if (j["V"][i] < j["V"][i - 1]) {
							//cout << "Lo: There the exception be" << endl;
							std::stringstream exception;
							exception << "Your intervals have an intersection: " << j["V"][i - 1] << " is greater than " << j["V"][i] << std::endl;
							throw exception.str();
						}
					}
					V.push_back(j["V"][i]);
				}
				else throw ((char)i + " element of V array is NAN or is negative");
			}
		}
		else {
			throw "V is not a correct array\n";
		}

		/*зададим пока статически
		I = 10;
		M.insert(end(M), { 20,75,100,105,75,0 });
		V.insert(end(V), { 0,75,150,200,250,300 });
		T_overheat = 110;
		Hm = 0.01;
		Hv = 0.0001;
		C = 0.1;
		T_environment = 13;
		eps = 0.00001;
		*/

		string user_input = "";
		cin >> user_input;

		/*if (is_float_number(user_input)) {
			istringstream ss(user_input);
			float f;
			ss >> f;
			T_environment = f;
		}
		else {
			throw "Input is NAN";
		}*/

		//for (short i = 0; i < user_input.length(); ++i ) {
		/*for(auto i : user_input)
			if (isdigit(user_input[i])) {
				continue;
			}
			else if (user_input[i] == '.'){
				cont
			}
		}*/

		
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


