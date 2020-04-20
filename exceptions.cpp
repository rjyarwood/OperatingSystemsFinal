//
// Created by rjyarwood on 4/20/20.
//
// using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class BadIndex: public exception
{
    virtual const char* what() const throw()
    {
        return "Invalid Index";
    }
} badIndex;

class BadSuperBlock: public exception
{
    virtual const char* what() const throw()
    {
        return "Not a valid SuperBlock";
    }
} invalidSuperBlock;