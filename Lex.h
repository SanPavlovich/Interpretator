#pragma once

#include <ostream>

#ifndef LEX_H
#define LEX_H
using namespace std;

enum type_of_lex {
	LEX_NULL,
	LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF,
	LEX_FALSE, LEX_INT, LEX_NOT, LEX_OR,
	LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE,
	LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_STRING, LEX_REAL, LEX_BREAK, LEX_CASE, LEX_OF,//
	LEX_FIN,
	LEX_SEMICOLON, LEX_AT, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN,
	LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS,
	LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ,
	LEX_NEQ, LEX_GEQ, LEX_LFPAREN, LEX_RFPAREN, LEX_DOUBLEEQ, //добавил {} ==
	//LEX_NUM,
	LEX_ID,

	POLIZ_LABEL,
	POLIZ_GO,
	POLIZ_FGO,
	POLIZ_TGO,
	POLIZ_ADDRESS, 
	POLIZ_DUP, //дублируем x на верх стека для операции сравнения: dup, 3, =
	POLIZ_POP, //достаем продублированный x обратно в самом конце полиза
	POLIZ_ERRCASE //ошибка, если ни один вариант из case не подошел
};
/* 
NULL, "and", "begin", "bool", "do", "else", "end",
// 0 1 2 3 4 5 6
"if", "false", "int", "not", "or", "program", "read",
// 7 8 9 10 11 12 13
"then", "true", "var", "while", "write"
// 14 15 16 17 18
*/

/*char* Scanner::TD[] = {
	NULL,";", "@", ",", ":", ":=", "(", ")",
	// 0 1 2 3 4 5 6 7
	"=","<",">","+","-","*","/","<=","!=",">=", "{", "}", "==", "."
	// 8 9 10 11 12 13 14 15 16 17 18 19 20 21
};*/

//порядок в enum и TD, TW совпадает

class Lex {
	type_of_lex t_lex;
	int v_lex;
	float f_lex;
	string str_lex;
public:
	Lex(type_of_lex t = LEX_NULL, int v = 0) {
		t_lex = t;
		v_lex = v;
	}

	Lex(type_of_lex t, float f) {
		t_lex = t;
		f_lex = f;
	}

	Lex(type_of_lex t, string s) {
		t_lex = t;
		str_lex = s;
	}

	type_of_lex get_type() { return t_lex; }
	int get_value() { return v_lex; }
	float get_real_value() { return f_lex; }
	string get_str_value() { return str_lex; }

	void invert_int_value() {
		v_lex = 0-v_lex;
	}

	void invert_real_value() {
		f_lex = -f_lex;
	}

	friend ostream& operator << (ostream& s, Lex l)
	{
		s << '(' << l.t_lex << ',' << l.v_lex << ");";
		return s;
	}
};

#endif

