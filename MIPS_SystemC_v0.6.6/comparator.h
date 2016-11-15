#ifndef COMPARATOR_H
#define COMPARATOR_H

/**
 *
 * ALU module interface.
 */

#include <systemc.h>

/**
 * ALU module.
 * ALU module models the integer ALU of the MIPS. It can perform 6 operations:
 * %add, subtract, %and, or, set on equal
 *   - input ports
 *    - \c sc_uint<32> \c din1  - first operand
 *    - \c sc_uint<32> \c din2  - second operand
 *    - \c sc_uint<3>  \c op  - selects operation
 *   - output ports
 *    - \c sc_unint<32> \c dout - result
 *    - \c bool         \c zero - active if result = 0
 */

SC_MODULE(comparator) {
  
  sc_in< sc_uint<32> >  din1;
  sc_in< sc_uint<32> >  din2;
  sc_out< bool > equal;

  SC_CTOR(comparator)
     {      
      SC_METHOD(entry);
      sensitive << din1 << din2;
    }
  
  void entry();
};

#endif
