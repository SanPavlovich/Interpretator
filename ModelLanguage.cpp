#include <iostream>
#include "Interpretator.h"

int main()
{
    std::cout << "Hello World!\n";
    /*Scanner a("C:\\Prak\\Cplus\\TestCode.txt");
    int i = 0;
    while (true) {
        Lex lexemma = a.get_lex();
        cout << i << ": " << lexemma << "\n";
        if (lexemma.get_type() == LEX_FIN)
            break;
        i++;
    }
    cout << "our TID:" << "\n";
    vector<Ident> b = a.getTID();
    for (i = 0; i < b.size(); i++)
        cout << b[i].get_name() << " "
        << b[i].get_assign() << " "
        << b[i].get_declare() << " " 
        << b[i].get_type() << " " 
        << b[i].get_value() << "\n";
    */

    Interpretator I("C:\\Prak\\Cplus\\TestCode.txt");
    I.interpretation();
    //I.test_parsing();
    return 0;

    /*try {
        Interpretator I("C:\\Prak\\Cplus\\TestCode.txt");
        //I.interpretation();
        I.test_parsing();
        return 0;
    }
    catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch (const char* source) {
        cout << source << endl;
        return 1;
    }*/
}

