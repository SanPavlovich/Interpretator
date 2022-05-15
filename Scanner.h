#pragma once

#include "Lex.h"
#include "Ident.h"
#include <string>
#include <fstream>
#include <iostream>

#ifndef SCANNER_H
#define SCANNER_H

//#define _CRT_SECURE_NO_WARNINGS

using namespace std;
class Scanner {
	ifstream fin;
	vector <Ident> TID;
	char c;
	int look(const string& buf, const vector<string> list) { //look - получаем v_lex из таблицы TW или TD
		int i = 0;
		while (i < list.size()) {
			if (buf.compare(list[i]) == 0)
				return i;
			i++;
		}
		return 0;
	}
	void gc() 
	{
		if (!fin.eof())
		{
			fin.get(c);
		}
		else
			throw "end of file, but didn't find end of program";
	} //gc - getchar (получаем символ из входного потока)
public:
	static const vector<string> TW, TD;
	Scanner(const char* program) {
		fin.open(program);
		if (!fin.is_open()) throw "canТt open file";
	}
	Lex get_lex();

	int putID(string buf) {
		//vector <Ident>::iterator k;
		Ident* var = new Ident(buf);
		/*if ((k = find(TID.begin(), TID.end(), *var)) != TID.end()) {
			vector <Ident>::iterator tidBegin = TID.begin();
			int distance = std::distance(k, tidBegin);
			return distance;
		}*/
		for (int i = 0; i < TID.size(); i++) {
			if (TID[i] == *var)
				return i;
		}
		TID.push_back(*var);
		return TID.size() - 1;
	}

	vector<Ident> *getTID() {
		return &TID;
	}

	void printTID() {
		for (int i = 0; i < TID.size(); i++)
			cout << TID[i].get_name() << " "
			<< TID[i].get_assign() << " "
			<< TID[i].get_declare() << " "
			<< TID[i].get_type() << " "
			<< TID[i].get_value() << "\n";
	}

};

#endif //endif scanner_h