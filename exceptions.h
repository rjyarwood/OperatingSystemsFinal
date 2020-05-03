//
// Created by rjyarwood on 4/28/20.
//

#ifndef INC_5806FINAL_EXCEPTIONS_H
#define INC_5806FINAL_EXCEPTIONS_H

#include <exception>

class BadIndex: public std::exception
{
    virtual const char* what() const throw();

};

class BadSuperBlock: public std::exception
{
    virtual const char* what() const throw();
};

#endif //INC_5806FINAL_EXCEPTIONS_H
