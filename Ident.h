#pragma once
#include "Lex.h" 
#include <vector>

#ifndef IDENT_H
#define IDENT_H

class Ident { //множество параметров переменной для хранения в таблице TID
	string name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
	float f_value;
	string str_value;
public:
	Ident() { 
		declare = false;
		assign = false;
		this->name = "";
	}
	Ident(string name)
	{
		//Ident(); не работает так
		declare = false;
		assign = false;
		this->name = name;
	}

	bool operator == (const Ident& other)
	{
		if (this->name == other.name)
			return true;
		else
			return false;
	}

	string get_name() { return name; }
	void put_name(string n)
	{
		name = n;
	}
	bool get_declare() { return declare; }
	void put_declare() { declare = true; }
	type_of_lex get_type() { return type; }
	void put_type(type_of_lex t) { type = t; }
	bool get_assign() { return assign; }
	void put_assign() { assign = true; }
	int get_value() { return value; }
	void put_value(int v) { value = v; }
	void put_real_value(float v) { f_value = v; }

	string get_str_value() {
		return str_value;
	}

	float get_real_value() {
		return f_value;
	}

	void put_str_value(string str) {
		str_value = str;
	}
};

#endif

