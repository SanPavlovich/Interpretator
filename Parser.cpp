#include "Parser.h"

void Parser::analyze() {
	gl();
	P();
	// for (Lex l : poliz) cout << l;
	cout << endl << "OK" << endl;
}

void Parser::P() {
	if (c_type == LEX_PROGRAM) {
		gl();
		if (c_type == LEX_LFPAREN) {
			gl();
		}
		else 
			throw "lfparen { expected";
	}
	else 
		throw "program expected";
	D1();
	S1();
	if (c_type != LEX_RFPAREN)
		throw "expected fin symbol }";
}

void Parser::D1() { // D1 -->  {D ;}
	while (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING || c_type == LEX_REAL) {
		D();
		if (c_type == LEX_SEMICOLON)
			gl();
		else
			throw "semicolon expected";
	}
}

void Parser::D() {
	type_of_lex c_type_tmp;
	if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING || c_type == LEX_REAL) { //����� ����� �������� ��� string, real
		c_type_tmp = c_type;
		st_types.push(c_type);
		gl();
		//�� ����� �������������� ������� ����� ���������� ����� ���
	}
	else
		throw "unadmittable type (expected int, real, bool, or string)";
	V();
	while (c_type == LEX_COMMA) {
		gl();
		V();
	}
	st_types.pop(); //������� �� ����� ��������� ��� ����� ������������� ����������
	dec(c_type_tmp);
};

void Parser::V() {
	if (c_type == LEX_ID) {
		st_idents.push(c_val);
		poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
		gl();
		if (c_type == LEX_EQ) {
			gl();
			C();
			//��������� �������������� ���� C � T
			eq_type_v(); 
			poliz.push_back(Lex(LEX_ASSIGN));

		}
	}
	else
		throw "identifier in V state expected";
}

void Parser::C() {
	if (c_type == LEX_PLUS || c_type == LEX_MINUS) {
		//����� ����������� � ����� ��� ������ � ������� �������� � ����� �������� �� �����, ������ ��� �� �����
		type_of_lex c_type_tmp = c_type;
		gl();
		if (c_type == LEX_INT) {
			st_types.push(LEX_INT);
			
			if (c_type_tmp == LEX_MINUS) {
				curr_lex.invert_int_value();
			}
			poliz.push_back(curr_lex);
			gl();
		}
		else if (c_type == LEX_REAL) {
			st_types.push(LEX_REAL);
			if (c_type_tmp == LEX_MINUS) {
				curr_lex.invert_real_value();
			}
			poliz.push_back(curr_lex);
			gl();
		}
		else
			throw "number expected";
	}
	else if (c_type == LEX_REAL) {
		st_types.push(LEX_REAL);
		poliz.push_back(curr_lex);
		gl();
	}
	else if(c_type == LEX_INT) {
		st_types.push(LEX_INT);
		poliz.push_back(curr_lex);
		gl();
	}
	else if (c_type == LEX_STRING) {
		st_types.push(LEX_STRING);
		poliz.push_back(curr_lex);
		gl();
	}
	else if (c_type == LEX_TRUE) {
		st_types.push(LEX_BOOL);
		poliz.push_back(Lex(LEX_TRUE, 1));
		gl();
	}
	else if (c_type == LEX_FALSE) {
		st_types.push(LEX_BOOL);
		poliz.push_back(Lex(LEX_FALSE, 0));
		gl();
	}
	else
		throw "incorrect constant";
}

void Parser::S1() {
	while (c_type != LEX_RFPAREN) {
		S();
	}
}

/*void Parser::B() {
	if (c_type == LEX_BEGIN) {
		gl();
		S();
		while (c_type == LEX_SEMICOLON) {
			gl();
			S();
		}
	}
	else
		throw "begin expected";
	if (c_type == LEX_END) {
		gl();
		//
	}
	else
		throw "end expected";
};*/

void Parser::S() {
	int pl0, pl1, pl2, pl3;
	if (c_type == LEX_ID) { //I()
		//check_id ���������, ��� ��� ���������� ��������������, ����� �� �������� ������������ �������� ����������
		check_id();
		poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
		gl();
		if (c_type == LEX_EQ) {
			gl();
			AE();
			eq_type();

			//��������� �� ; ����� ��������� ������������
			if (c_type != LEX_SEMICOLON)
				throw "semicolon expected";

			gl();
			poliz.push_back(Lex(LEX_ASSIGN));
			//poliz
		}
		else
			throw "= expected";
	}
	else if (c_type == LEX_IF) {
		gl();
		if (c_type != LEX_LPAREN)
			throw "lparen ( after if expected";
		gl();
		AE();
		eq_bool();
		pl2 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_FGO));

		if (c_type == LEX_RPAREN) {
			gl();
			S();
			pl3 = poliz.size();
			poliz.push_back(Lex());

			poliz.push_back(Lex(POLIZ_GO));
			poliz[pl2] = Lex(POLIZ_LABEL, (int)poliz.size());
			if (c_type == LEX_ELSE)
			{
				gl();
				S();
				poliz[pl3] = Lex(POLIZ_LABEL, (int)poliz.size());
			}
			else
				throw "else expected";
		}
		else
			throw "rparen ) after if expected";
	}
	else if (c_type == LEX_WHILE) {
		st_loop_breaks.push(stack <int>());
		pl0 = poliz.size();
		gl();
		if (c_type != LEX_LPAREN)
			throw "( after while expected";
		gl();
		AE();
		eq_bool();

		pl1 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_FGO));
		if (c_type == LEX_RPAREN) {
			gl();
			S();
			poliz.push_back(Lex(POLIZ_LABEL, pl0));
			poliz.push_back(Lex(POLIZ_GO));
			poliz[pl1] = Lex(POLIZ_LABEL, (int)poliz.size());
		}
		else
			throw ") after while expected"; 
		//����� �������� �� ������ ��������� ����� ���� ���������� ��� ������� break
		stack <int> tmp_breaks = st_loop_breaks.top();
		while (!tmp_breaks.empty()) {
			int tmp_int;
			from_st(tmp_breaks, tmp_int);
			poliz[tmp_int] = Lex(POLIZ_LABEL, (int)poliz.size());
		}
		st_loop_breaks.pop(); //loop_cnt--
	}
	else if (c_type == LEX_BREAK) {
		if (st_loop_breaks.empty())
			throw "break outside of loop";
		gl();
		if (c_type != LEX_SEMICOLON)
			throw "semicolon after break expected";
		gl();
		pl0 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_GO));
		st_loop_breaks.top().push(pl0); //�������� � ������� ���� ����� ���� ������ � ������� �������� �� ������
	}
	else if (c_type == LEX_READ) {
		gl();
		if (c_type == LEX_LPAREN) {
			gl();
			if (c_type == LEX_ID) {
				check_id_in_read();
				poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
				gl();
				if (c_type == LEX_RPAREN) {
					gl();
					if (c_type == LEX_SEMICOLON)
						gl();
					else
						throw "semicolon ; after read expected";
					poliz.push_back(Lex(LEX_READ));
				}
				else
					throw "rparen expected";
			}
			else
				throw "identifier expected";
		}
		else
			throw "lparen expected";
	}
	else if (c_type == LEX_WRITE) {
		gl();
		if (c_type == LEX_LPAREN) {
			gl();
			AE();
			st_types.pop(); //�� ����� ����� ��� ��������, ������� ��� �������� �������� �� ����� ���
			poliz.push_back(Lex(LEX_WRITE));
			while (c_type == LEX_COMMA) {
				gl();
				AE();
				st_types.pop();
				poliz.push_back(Lex(LEX_WRITE));
			}
			if (c_type == LEX_RPAREN) {
				gl();
				if (c_type != LEX_SEMICOLON)
					throw "semicolon after write expected";
				gl();
			}
			else
				throw "rparen after write expected";
		}
		else
			throw "lparen expected";
	}
	else if(c_type == LEX_LFPAREN) {
		gl();
		S1();
		if (c_type != LEX_RFPAREN)
			throw "} expected";
		gl();
	}
	else
		throw "incorrect statement in S";
};

void Parser::AE() {
	E();
	/*if (c_type == LEX_ID) {
		check_id();
		poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
		gl();
		if (c_type == LEX_EQ) {
			gl();
			AE();
			eq_type();
			poliz.push_back(Lex(LEX_ASSIGN));
			//poliz
		}
		else
			throw "= expected";
	}
	else
		E();*/
}

void Parser::E() {
	E1();

	if (c_type == LEX_DOUBLEEQ || c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) {
		st_types.push(c_type);
		gl();
		E1();
		check_op();
	}
};

void Parser::E1() {
	T();
	while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
		st_types.push(c_type);
		gl();
		T();
		check_op();
	}
};

void Parser::T() {
	F();
	while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND)
	{
		st_types.push(c_type);
		gl();
		F();
		check_op();
	}
};

void Parser::F() {
	if (c_type == LEX_ID) {
		check_id();
		poliz.push_back(curr_lex);
		gl();
	}
	else if (c_type == LEX_INT) {
		st_types.push(LEX_INT);
		poliz.push_back(curr_lex);
		gl();
	}
	else if (c_type == LEX_REAL) {
		st_types.push(LEX_REAL);
		poliz.push_back(curr_lex);
		gl();
	}
	else if (c_type == LEX_STRING) {
		st_types.push(LEX_STRING);
		poliz.push_back(curr_lex);
		gl();
	}
	else if (c_type == LEX_TRUE) {
		st_types.push(LEX_BOOL);
		poliz.push_back(Lex(LEX_TRUE, 1));
		gl();
	}
	else if (c_type == LEX_FALSE) {
		st_types.push(LEX_BOOL);
		poliz.push_back(Lex(LEX_FALSE, 0));
		gl();
	}
	else if (c_type == LEX_NOT) {
		gl();
		F();
		check_not();
	}
	//���� ������� ����, �� ���������� (��� ����� ��� ��� ���)
	else if (c_type == LEX_PLUS) {
		gl();
		AE();
	}
	else if (c_type == LEX_MINUS) {
		st_types.push(LEX_INT); //�������� � ���� �������� ��� ����
		st_types.push(LEX_MINUS); //�������� � ���� �������� ����� (-a -> 0-a)
		poliz.push_back(Lex(LEX_INT, 0));
		gl();
		F();
		check_op(); //����� � ����� ��������� ������������� ����� �������� ��������
	}
	else if (c_type == LEX_LPAREN) {
		gl();
		AE();
		if (c_type == LEX_RPAREN) {
			gl();
			//fine
		}
		else
			throw "rparen expected";
	}
	else
		throw "error in F statement";

};
/*void Parser::L() {};
void Parser::I() {};
void Parser::N() {};*/

void Parser::dec(type_of_lex type) {
	int i;
	vector <Ident> *TID = scan.getTID(); //������� ���������� � ���� ���������� � ���������� � �� �������
	while (!st_idents.empty()) {
		from_st(st_idents, i);
		if ((*TID)[i].get_declare()) //���������, ��� ���������� �� ���������������� ������
			throw "variable " + (*TID)[i].get_name() + " described twice";
		else {
			(*TID)[i].put_declare();
			(*TID)[i].put_type(type);
		}
	}
}

void Parser::check_id() {
	vector <Ident> *TID = scan.getTID();
	if ((*TID)[c_val].get_declare()) //st_lex - ���� ��� ���������
		st_types.push((*TID)[c_val].get_type());
	else throw "not declared";
}

void Parser::check_op() {
	type_of_lex t1, t2, op, result;
	from_st(st_types, t2);
	from_st(st_types, op);
	from_st(st_types, t1);
	if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
		if (t1 == LEX_INT && t2 == LEX_INT) {
			//fine
			result = LEX_INT;
		}
		else if (t1 == LEX_STRING && t2 == LEX_STRING && op == LEX_PLUS) {
			result = LEX_STRING;
		}
		else if (t1 == LEX_REAL && t2 == LEX_REAL) {
			result = LEX_REAL;
		}
		else if (t1 == LEX_REAL && t2 == LEX_INT || t1 == LEX_INT && t2 == LEX_REAL)
			result = LEX_REAL;
		else
			throw "shit happened in operarions (+, -, *, :) with strings or integers or reals";
	}
	else if (op == LEX_GTR || op == LEX_LSS || op == LEX_DOUBLEEQ || op == LEX_NEQ || op == LEX_GEQ || op == LEX_LEQ) {
		if ((t1 == LEX_INT || t1 == LEX_REAL) && (t2 == LEX_INT || t2 == LEX_REAL))
			result = LEX_BOOL;
		else if (t1 == LEX_STRING && t2 == LEX_STRING) {
			result = LEX_BOOL;
		}
		else
			throw "shit happened in comparison operarions with strings or integers or reals";
	}
	else if (op == LEX_OR || op == LEX_AND) {
		if (t1 == LEX_BOOL && t2 == LEX_BOOL) {
			//fine
			result = LEX_BOOL;
		}
		else
			throw "not bool argument during the operation (and, or)";
	}
	else 
		throw "wrong types are in operation";
	st_types.push(result);
	poliz.push_back(Lex(op));
}

void Parser::check_not() {
	if (st_types.top() != LEX_BOOL)
		throw "wrong type is in not";
	else {
		poliz.push_back(Lex (LEX_NOT));
	}
}

void Parser::eq_type() {
	eq_type_v();
	st_types.pop();
}

void Parser::eq_type_v() {
	type_of_lex t1, t2;
	from_st(st_types, t1); //������� ��� �������� ����� ������ ����� (�������� � ������ ������ ������� c ������� fromst)
	t2 = st_types.top();
	if ((t1 == LEX_INT || t1 == LEX_REAL) && (t2 == LEX_INT || t2 == LEX_REAL)) {} //� ������� � ��� ��� ���������
	else if (t1 == t2) {}
	else
		throw "wrong types are in =";
	//��������� ���������� �� ������ S1, �� ��������� pop �� �����
}

void Parser::eq_bool() {
	if (st_types.top() != LEX_BOOL)
		throw "expression is not boolean";
	else
		st_types.pop();
}

void Parser::check_id_in_read() {
	//vector <Ident> TID = scan.getTID();
	//if (!TID[c_val].get_declare())
	if(!(*scan.getTID())[c_val].get_declare())
		throw "not declared";
}


