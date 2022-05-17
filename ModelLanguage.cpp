#include <iostream>
#include "Interpretator.h"

int main()
{
    try {
        Interpretator I("C:\\Prak\\Cplus\\TestCode.txt");
        I.interpretation();
        //I.test_parsing();
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
    }
}


