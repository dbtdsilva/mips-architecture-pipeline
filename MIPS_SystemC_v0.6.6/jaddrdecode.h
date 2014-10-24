#ifndef JADDRDECODE_H
#define JADDRDECODE_H

#include <systemc.h>

SC_MODULE(jaddrdecode) {

  sc_in < sc_uint<32> > inst;
  sc_in < sc_uint<32> > PC_id;
  sc_in < sc_uint<32> > data1;
  sc_in < bool > JumpOnRegister;
  sc_out< sc_uint<32> > out;

  SC_CTOR(jaddrdecode)
     {
      SC_METHOD(entry);
      sensitive << inst << PC_id << data1 << JumpOnRegister;
    }

  void entry();
};

#endif
