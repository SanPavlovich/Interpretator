#include "Interpretator.h"

void Interpretator::interpretation() {
    pars.analyze();
    E.execute(pars.poliz, pars.getTID());
}

void Interpretator::test_parsing() {
    pars.analyze();
}