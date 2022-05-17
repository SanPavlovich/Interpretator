#pragma once
#include "Scanner.h"

const vector<string> Scanner::TW = {
	"","and","begin","bool","do","else","end",
	// 0 1 2 3 4 5 6
	"if","false","int","not","or","program","read",
	// 7 8 9 10 11 12 13
	"then","true","var","while","write", "string", "real", "break", "case", "of"
	// 14 15 16 17 18 19 20 21 22 23
};

const vector<string> Scanner::TD = {
	"",";", "@", ",", ":", ":=", "(", ")", //порядок в TD совпадает с type_of_lex после LEX_FIN
	// 0 1 2 3 4 5 6 7
	"=","<",">","+","-","*","/","<=","!=",">=", "{", "}", "==",
	// 8 9 10 11 12 13 14 15 16 17 18 19 20
};


Lex Scanner::get_lex() {
	enum state { H, IDENT, STRING, NUMB, REAL_NUMB, COMMENT, ALE, DELIM, NEQ };
	state CS = H;
	string buf;
	int d, j, pow_cnt = 1;
	float d1;
	do {
		gc();
		switch (CS) { //CS - current state
		case H:
			if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {} // /r - линуксовый перенос строки, /t - табуляция
			else {
				if (isalpha(c)) { buf.push_back(c); CS = IDENT; }
				else {
					if (isdigit(c)) { d = c - '0'; CS = NUMB; }
					else {
						if (c == '"') {
							CS = STRING;
						}
						else {
							 if (c == '=' ||  c == '<' || c == '>') { //нужно доработать автомат на случай == 
							  	 buf.push_back(c); CS = ALE;
							 }
							 else {
								 if (c == '@') return Lex(LEX_FIN);
								 else {
								 	 if (c == '!') { buf.push_back(c); CS = NEQ; }
									 else {
										 //делаю развилку для случая комментария: / или /*
										 buf.push_back(c);
										 if (j = look(buf, TD)) {
											 if (c == '/') {
												 gc();
												 if (c == '*') {
													 CS = COMMENT;
													 buf.clear();
												 }
												 else {
													 fin.unget();
													 return Lex((type_of_lex)(j + (int)LEX_FIN), j);
												 }
											 }
											 else
												return Lex((type_of_lex)(j + (int)LEX_FIN), j);
										 }
										 else throw c;
										 break;
									 }
								 }
							 }
						}
					}
				}
			}
			break;
		case IDENT:
			if (isalpha(c) || isdigit(c)) { buf.push_back(c); }
			else {
				fin.unget();
				if (j = look(buf, TW)) //два случая - если слово зарезервированное или нет
					return Lex((type_of_lex)j, j);
				else {
					j = putID(buf); //записываем пользовательское слово в TID и возвращаем Lex с номером j в TID
					return Lex(LEX_ID, j);
				}
				break;
			}
			break;
		case NUMB: //считываем число, если встретилась не цифра, то возвращаем лексемму со значением d - число
			if (isdigit(c))
				d = d * 10 + (c - '0');
			else if (c == '.') {
				d1 = 0;
				pow_cnt = 1;
				CS = REAL_NUMB;
			}
			else {
				fin.unget(); 
				return Lex(LEX_INT, d); 
			}
			break;

		case REAL_NUMB:
			if (isdigit(c)) {
				d1 = d1 + (float)(c - '0') / (pow(10, pow_cnt)); // .935 -> 0.9 + 0.03 + 0.005
				pow_cnt++;
			}
			else {
				fin.unget();
				return Lex(LEX_REAL, (float)d + d1);
			}

		case STRING:
			if (c != '"')
				buf.push_back(c);
			else
				return Lex(LEX_STRING, buf);
			break;

		case COMMENT:
			if (c == '*') {
				gc();
				if (c == '/') {
					CS = H;
					buf.clear();
				}
			}
			break;

		case ALE:
			if (c == '=') {
				buf.push_back(c); 
				j = look(buf, TD);
				return Lex((type_of_lex)(j + (int)LEX_FIN), j);
			}
			else {
				fin.unget();
				j = look(buf, TD);
				return Lex((type_of_lex)(j + (int)LEX_FIN), j);
			}
			break;

		case NEQ:
			if (c == '=') {
				buf.push_back(c); j = look(buf, TD);
				return Lex(LEX_NEQ, j);
			}
			else throw '!';
			break;
		} //end of switch
	} while (true);
}// end of getlex()
