#pragma once
#include "Executer.h"

class Interpretator
{
	Parser pars;
	Executer E;
public:
	Interpretator(const char* program) : pars(program) {}
	void interpretation();
	void test_parsing();
};

