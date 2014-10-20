
// comparator.cpp

#include "comparator.h"

/**
 * \c comparator module callback function.
 */
void comparator::entry()
{
    sc_uint<32> a=din1.read();
    sc_uint<32> b=din2.read();
    //sc_uint<32> res;
    equal.write(a == b);
}
