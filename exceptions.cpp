//
// Created by rjyarwood on 4/20/20.
//
// using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class exceptions: public exception
{
    virtual const char* what() const throw()
    {
        return "Invalid Index";
    }
} badIndex;
