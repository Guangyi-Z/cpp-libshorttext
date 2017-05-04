#ifndef _CALCULATOR_HPP_
#define _CALCULATOR_HPP_

#include "linear.h"

namespace libshorttext {

    class Calculator {

    public:
        int add(int first, int second);
        int substract(int first, int second);
        int multiply(int first, int second);
        int divide(int first, int second);

    };

    int get_liblinear_version() {
        return liblinear_version;
    }
}
#endif // _CALCULATOR_HPP_
