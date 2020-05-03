//
// Created by rjyarwood on 4/20/20.
//
// using standard exceptions

#include "exceptions.h"
using namespace std;



const char *BadIndex::what() const throw() {
    return exception::what();
}

const char *BadSuperBlock::what() const throw() {
    return exception::what();
}
