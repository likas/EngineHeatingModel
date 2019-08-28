//#pragma once
#ifndef _VECTOR_INCLUDE
#include <vector>
#define _VECTOR_INCLUDE
#endif
#include <utility>
#include <iostream>

class piesewise_linear_function { //интервальная линейная функция. имеет вид y = kx + b, y э 
public:
	double k;
	double b;
	int x1;
	int x2; //y это M, x это V
	void print(){
		std::cout << "M = " << k << "V + " << b << ", V in [" << x1 << "][" << x2 << "]" << std::endl;
	}
};
