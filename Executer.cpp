#include "Executer.h"
#include "Parser.h"

void Executer::execute(vector<Lex>& poliz, vector <Ident>& TID) {
    Lex pc_el;
    stack < Lex > args;
    int i, j, index = 0, size = poliz.size();
    Lex tmp_lex, tmp_lex2;
    while (index < size) {
        pc_el = poliz[index];
        switch (pc_el.get_type()) {
        case LEX_TRUE: case LEX_FALSE: case LEX_INT: case LEX_REAL: case LEX_STRING: case POLIZ_ADDRESS: case POLIZ_LABEL:
            args.push(pc_el);
            break;

        case LEX_ID: //различные случаи при складывании значений в переменную на этапе интерпретации
            i = pc_el.get_value();
            if (TID[i].get_assign()) {
                if (TID[i].get_type() == LEX_STRING)
                    args.push(Lex(TID[i].get_type(), TID[i].get_str_value()));
                else if(TID[i].get_type() == LEX_REAL)
                    args.push(Lex(TID[i].get_type(), TID[i].get_real_value()));
                else
                    args.push(Lex(TID[i].get_type(), TID[i].get_value()));
            }
            else
                throw "POLIZ: uninitialized variable "; //+ TID[i].get_name();
            break;

        case LEX_NOT:
            from_st(args, tmp_lex);
            args.push(Lex(tmp_lex.get_type(), !tmp_lex.get_value()));
            break;

        case LEX_OR:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            args.push(Lex(tmp_lex.get_type(), tmp_lex.get_value() || tmp_lex2.get_value()));
            break;

        case LEX_AND:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            args.push(Lex(tmp_lex.get_type(), tmp_lex.get_value() && tmp_lex2.get_value()));
            break;

        case POLIZ_GO:
            from_st(args, tmp_lex);
            index = tmp_lex.get_value() - 1;
            break;
            
        case POLIZ_FGO:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (!tmp_lex2.get_value()) 
                index = tmp_lex.get_value() - 1;
            break;

        case LEX_WRITE:
            from_st(args, tmp_lex);
            if (tmp_lex.get_type() == LEX_STRING)
                cout << tmp_lex.get_str_value() << endl;
            else if (tmp_lex.get_type() == LEX_REAL)
                cout << tmp_lex.get_real_value() << endl;
            else
                cout << tmp_lex.get_value() << endl;
            break;

        case LEX_READ:
            int k;
            from_st(args, tmp_lex);
            i = tmp_lex.get_value();
            if (TID[i].get_type() == LEX_INT) {
                cout << "Input int value for " << TID[i].get_name() << ":"<< endl;
                cin >> k;
                TID[i].put_value(k);
            }
            else if (TID[i].get_type() == LEX_STRING) {
                string str;
                cout << "Input str value for " << TID[i].get_name() << ":" << endl;
                cin >> str;
                TID[i].put_str_value(str);
            }
            else if (TID[i].get_type() == LEX_REAL) {
                float fval;
                cout <<"Input float value for " << TID[i].get_name() << ":" << endl;
                cin >> fval;
                TID[i].put_real_value(fval);
            }
            else {
                string j;
                while (1) {
                    cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
                    cin >> j;
                    if (j != "true" && j != "false") {
                        cout << "Error in input:true/false" << endl;
                        continue;
                    }
                    k = (j == "true") ? 1 : 0;
                    break;
                }
                TID[i].put_value(k);
            }
            TID[i].put_assign();
            break;

        case LEX_PLUS:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_STRING, tmp_lex2.get_str_value() + tmp_lex.get_str_value()));
            }
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_INT, tmp_lex2.get_value() + tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() + tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() + tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_REAL, tmp_lex2.get_value() + tmp_lex.get_real_value()));
            else //случай bool bool
                args.push(Lex(tmp_lex.get_type(), tmp_lex2.get_value() + tmp_lex.get_value()));
            break;

        case LEX_TIMES:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_INT, tmp_lex2.get_value() * tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() * tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() * tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_REAL, tmp_lex2.get_value() * tmp_lex.get_real_value()));
            break;

        case LEX_MINUS:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_INT, tmp_lex2.get_value() - tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() - tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() - tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_REAL, tmp_lex2.get_value() - tmp_lex.get_real_value()));
            break;

        case LEX_SLASH:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT && tmp_lex.get_value() != 0)
                args.push(Lex(LEX_INT, tmp_lex2.get_value() / tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL && tmp_lex.get_real_value() != (float)0)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() / tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL && tmp_lex.get_value() != 0)
                args.push(Lex(LEX_REAL, tmp_lex2.get_real_value() / tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT && tmp_lex.get_real_value() != (float)0)
                args.push(Lex(LEX_REAL, tmp_lex2.get_value() / tmp_lex.get_real_value()));
            else
                throw "POLIZ:divide by zero";
            break;

        case LEX_DOUBLEEQ: 
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() == tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() == tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() == tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() == tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_BOOL, tmp_lex2.get_str_value() == tmp_lex.get_str_value()));
            }
            else
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() == tmp_lex.get_value()));
            break;

        case LEX_LSS:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() < tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() < tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() < tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() < tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_BOOL, tmp_lex2.get_str_value() < tmp_lex.get_str_value()));
            }
            else
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() < tmp_lex.get_value()));
            break;

        case LEX_GTR:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() > tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() > tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() > tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() > tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_BOOL, tmp_lex2.get_str_value() > tmp_lex.get_str_value()));
            }
            else
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() > tmp_lex.get_value()));
            break;

        case LEX_LEQ:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() <= tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() <= tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() <= tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() <= tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_BOOL, tmp_lex2.get_str_value() <= tmp_lex.get_str_value()));
            }
            else
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() <= tmp_lex.get_value()));
            break;

        case LEX_GEQ:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() >= tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() >= tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() >= tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() >= tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_BOOL, tmp_lex2.get_str_value() >= tmp_lex.get_str_value()));
            }
            else
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() >= tmp_lex.get_value()));
            break;

        case LEX_NEQ:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() != tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() != tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_INT && tmp_lex2.get_type() == LEX_REAL)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_real_value() != tmp_lex.get_value()));
            else if (tmp_lex.get_type() == LEX_REAL && tmp_lex2.get_type() == LEX_INT)
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() != tmp_lex.get_real_value()));
            else if (tmp_lex.get_type() == LEX_STRING) { //равенство типов 2 переменных проверится на семантическом анализе
                args.push(Lex(LEX_BOOL, tmp_lex2.get_str_value() != tmp_lex.get_str_value()));
            }
            else
                args.push(Lex(LEX_BOOL, tmp_lex2.get_value() != tmp_lex.get_value()));
            break;

        case LEX_ASSIGN:
            from_st(args, tmp_lex);
            from_st(args, tmp_lex2);
            i = tmp_lex.get_value();
            j = tmp_lex2.get_value();
            if (tmp_lex.get_type() == LEX_STRING)
                TID[j].put_str_value(tmp_lex.get_str_value());
            else if(TID[j].get_type() == LEX_INT && tmp_lex.get_type() == LEX_INT)
                TID[j].put_value(tmp_lex.get_value());
            else if (TID[j].get_type() == LEX_REAL && tmp_lex.get_type() == LEX_REAL)
                TID[j].put_real_value(tmp_lex.get_real_value());
            else if (TID[j].get_type() == LEX_INT && tmp_lex.get_type() == LEX_REAL)
                TID[j].put_value((int)tmp_lex.get_real_value());
            else if (TID[j].get_type() == LEX_REAL && tmp_lex.get_type() == LEX_INT)
                TID[j].put_real_value((float)tmp_lex.get_value());
            else //для случая bool = bool
                TID[j].put_value(i);
            TID[j].put_assign();
            break;

        default:
            throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}

