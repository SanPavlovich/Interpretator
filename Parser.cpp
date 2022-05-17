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
	if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING || c_type == LEX_REAL) { //нужно будет добавить тип string, real
		c_type_tmp = c_type;
		st_types.push(c_type);
		gl();
		//на этапе семинтического анализа будем дописывать здесь код
	}
	else
		throw "unadmittable type (expected int, real, bool, or string)";
	V();
	while (c_type == LEX_COMMA) {
		gl();
		V();
	}
	st_types.pop(); //забрали из стека начальный тип перед перечислением переменных
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
			//проверить приравнимаемые типы C и T
			eq_type_v(); 
			poliz.push_back(Lex(LEX_ASSIGN));

		}
	}
	else
		throw "identifier in V state expected";
}

void Parser::C() {
	if (c_type == LEX_PLUS || c_type == LEX_MINUS) {
		//перед склдыванием в полиз для случая с минусом поменять у числа значение на минус, достав его из стека
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
		//check_id проверяет, что тип переменной задекларирован, далее мы выполним присваивание значения переменной
		check_id();
		poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
		gl();
		if (c_type == LEX_EQ) {
			gl();
			AE();
			eq_type();

			//проверяем на ; после оператора присваивания
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
		//после проверки на скобки вставляем место куда переходить при наличии breakg
		check_breaks();
	}
	else if (c_type == LEX_DO) {
		st_loop_breaks.push(stack <int>());
		pl0 = poliz.size();
		gl();
		S();
		if (c_type == LEX_WHILE) {
			gl();
			if (c_type == LEX_LPAREN) {
				gl();
				AE();
				eq_bool();
				poliz.push_back(Lex(POLIZ_LABEL, pl0));
				poliz.push_back(Lex(POLIZ_TGO));
				if (c_type != LEX_RPAREN)
					throw ") after while expected";
				gl();
				if (c_type != LEX_SEMICOLON)
					throw "; after () in while expected";
				gl();
			}
			else
				throw "( after while expected";
		}
		else
			throw "while after do expected";
		//аналогичная проверка стека брейков, как и в while {}
		check_breaks();
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
		st_loop_breaks.top().push(pl0); //положили в верхний стек адрес фейк ячейки с адресом перехода по брейку
	}
	else if (c_type == LEX_CASE) {
		CASE();
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
			st_types.pop(); //не важно какой тип печатаем, поэтому без проверок вынимаем из стека тип
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

void Parser::CASE() {
	variant_next = 0;
	while(!st_variant_exit.empty()) //очищаем стеки при входе
		st_variant_exit.pop();
	while (!st_variant_true.empty())
		st_variant_true.pop();
	const_values_set.clear();

	gl();
	if (c_type != LEX_LPAREN)
		throw "( after case expected";
	gl();
	AE();
	eq_int();//проверка выражения на целочисленность в операторе case
	if (c_type != LEX_RPAREN)
		throw ") after case expected";
	gl();
	if (c_type != LEX_OF)
		throw "of after case expected";
	gl();
	VL();
	if (c_type != LEX_END)
		throw "end after case expected";
	gl();
	if (c_type != LEX_SEMICOLON)
		throw "semicolon after end (in case) expected";

	poliz.push_back(Lex(POLIZ_ERRCASE));
	while (!st_variant_exit.empty()) { //проставили значение 41 по адресам 17, 38 из примера в тетради
		int exit_address = st_variant_exit.top();
		poliz[exit_address] = Lex(POLIZ_LABEL, (int)poliz.size());
		st_variant_exit.pop();
	}
	poliz.push_back(Lex(POLIZ_POP));

	gl();
}

void Parser::VL() {
	VR();
	while (c_type != LEX_END) {
		VR(); //список вариантов закончится при попадании на лексемму END
	}
}

void Parser::VR() {
	poliz.push_back(Lex(POLIZ_DUP));
	C(); //константа в полиз положится в С
	eq_int(); //проверка константы на целочисленность в операторе case
	int constant_value = poliz[poliz.size() - 1].get_value();
	if (const_values_set.find(constant_value) == const_values_set.end()) //проверка повторяющихся вариантов в case
		const_values_set.insert(constant_value);
	else
		throw "duplicate in case";
		
	poliz.push_back(Lex(LEX_DOUBLEEQ));
	st_variant_true.push((int)poliz.size()); //положили в стек адрес ячейки для перехода по case true
	poliz.push_back(Lex());
	poliz.push_back(Lex(POLIZ_TGO));

	while (c_type == LEX_COMMA) {
		gl();
		poliz.push_back(Lex(POLIZ_DUP));
		C();
		eq_int();
		int constant_value = poliz[poliz.size() - 1].get_value();
		if (const_values_set.find(constant_value) == const_values_set.end()) //проверка повторяющихся вариантов в case
			const_values_set.insert(constant_value);
		else
			throw "duplicate in case";

		poliz.push_back(Lex(LEX_DOUBLEEQ));
		st_variant_true.push((int)poliz.size()); //положили в стек адрес ячейки для перехода по case true
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_TGO));
	}

	variant_next = poliz.size(); //запомнили адрес 13(для значения 19) из примера в тетради
	poliz.push_back(Lex());
	poliz.push_back(Lex(POLIZ_GO));

	if (c_type != LEX_COLON)
		throw ": after case in VR expected";
	gl();

	while (!st_variant_true.empty()) { //заполнили все 15 по адресам 6 и 11 (из примера в тетради)
		int true_address = st_variant_true.top();
		poliz[true_address] = Lex(POLIZ_LABEL, (int)poliz.size());
		st_variant_true.pop();
	}
	S();

	st_variant_exit.push((int)poliz.size()); //запомнили в стек адрес 17(для значения 41) из примера в тетради
	poliz.push_back(Lex());
	poliz.push_back(Lex(POLIZ_GO));

	poliz[variant_next] = Lex(POLIZ_LABEL, (int)poliz.size()); //проставили в адрес 13 значение 19 из примера в тетради
}

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
	//если унарный плюс, то пропускаем (все равно что его нет)
	else if (c_type == LEX_PLUS) {
		gl();
		AE();
	}
	else if (c_type == LEX_MINUS) {
		st_types.push(LEX_INT); //положили в стек операций тип нуля
		st_types.push(LEX_MINUS); //положили в стек операций минус (-a -> 0-a)
		poliz.push_back(Lex(LEX_INT, 0));
		gl();
		F();
		check_op(); //минус в полиз положится автоматически после проверки операции
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
	vector <Ident> *TID = scan.getTID(); //заносим информацию о типе переменной и информацию о ее наличии
	while (!st_idents.empty()) {
		from_st(st_idents, i);
		if ((*TID)[i].get_declare()) //проверяем, что переменная не инициализирована дважды
			throw "variable " + (*TID)[i].get_name() + " described twice";
		else {
			(*TID)[i].put_declare();
			(*TID)[i].put_type(type);
		}
	}
}

void Parser::check_id() {
	vector <Ident> *TID = scan.getTID();
	if ((*TID)[c_val].get_declare()) //st_lex - стэк для выражений
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
	from_st(st_types, t1); //сравним два операнда между знаком равно (достанем и уберем первый элемент c помощью fromst)
	t2 = st_types.top();
	if ((t1 == LEX_INT || t1 == LEX_REAL) && (t2 == LEX_INT || t2 == LEX_REAL)) {} //и сравним с тем что останется
	else if (t1 == t2) {}
	else
		throw "wrong types are in =";
	//поскольку сравниваем не внутри S1, то последний pop не нужен
}

void Parser::eq_bool() {
	if (st_types.top() != LEX_BOOL)
		throw "expression is not boolean";
	else
		st_types.pop();
}

void Parser::eq_int() {
	if (st_types.top() != LEX_INT)
		throw "expression or constant aren't int";
	else
		st_types.pop();
}

void Parser::check_id_in_read() {
	//vector <Ident> TID = scan.getTID();
	//if (!TID[c_val].get_declare())
	if(!(*scan.getTID())[c_val].get_declare())
		throw "not declared";
}

void Parser::check_breaks() {
	//после проверки на скобки вставляем место куда переходить при наличии breakg
	stack <int> tmp_breaks = st_loop_breaks.top();
	while (!tmp_breaks.empty()) {
		int tmp_int;
		from_st(tmp_breaks, tmp_int);
		poliz[tmp_int] = Lex(POLIZ_LABEL, (int)poliz.size());
	}
	st_loop_breaks.pop(); //loop_cnt--
}
