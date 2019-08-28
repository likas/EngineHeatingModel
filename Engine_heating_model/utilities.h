//#pragma once
#ifndef _VECTOR_INCLUDE
#include <vector>
#define _VECTOR_INCLUDE
#endif
#include <utility>
#include <iostream>

class piesewise_linear_function { //������������ �������� �������. ����� ��� y = kx + b, y � 
public:
	double k;
	double b;
	int x1;
	int x2; //y ��� M, x ��� V
	void print(){
		std::cout << "M = " << k << "V + " << b << ", V in [" << x1 << "][" << x2 << "]" << std::endl;
	}
};
