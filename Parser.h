#pragma once
#include <stack>
#include "Lex.h"
#include "Scanner.h"

class Parser
{
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
	Scanner scan;
	stack <int> st_idents; //храним здесь адреса переменных в TID, которым надо присвоить тип
	stack <type_of_lex> st_types; //храним здесь типы выражений в процессе разбора Е()
	stack <stack <int>> st_loop_breaks; //внутренний стэк хранит адреса точек перехода по брейку, внешний стек хранит стэки брейков для циклов
	//int - это адрес ячейки(пустой) с адресом (адрес - это куда переходить после брейка)


	void P(); void D1(); void D(); /*void B();*/ void S();
	void E(); void E1(); void T(); void F(); void L();
	void V(); void AE(); void S1(); void C();
	//void I(); void N();
	void dec(type_of_lex type); void check_id();
	void check_op(); void check_not(); void eq_type(); void eq_type_v();
	void eq_bool(); void check_id_in_read();
	void gl() {
		curr_lex = scan.get_lex();
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
	}
public:
	vector <Lex> poliz; //вектор из объектов класса Lex

	Parser(const char* program) :scan(program) {}

	void analyze();

	void printTID() {
		scan.printTID();
	}

	vector <Ident>& getTID() {
		return (*scan.getTID());
	}
};

template <class T, class T_EL>
void from_st(T& t, T_EL& x) { //положили в икс значение элемента на верху стека и убрали верхний элемент стэка
	x = t.top();
	t.pop();
}