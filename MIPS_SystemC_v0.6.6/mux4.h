#ifndef MUX4MOD_H
#define MUX4MOD_H


#include <systemc.h>


template <class T> class mux4: public sc_module
{
public:
  sc_in< T >  din0;
  sc_in< T >  din1;
  sc_in< T >  din2;
  sc_in< T >  din3;       
  sc_in< bool >  sel0;
  sc_in< bool >  sel1;     
  sc_out< T > dout;

  SC_CTOR(mux4)
     {      
      SC_METHOD(entry);
      sensitive << din0 << din1 << din2 << din3 << sel0 << sel1;
    }
  
  void entry();
};


template <class T> void mux4<T>::entry()
{
    if (!sel1.read() && !sel0.read())
        dout.write(din0.read());
    if (!sel1.read() && sel0.read())
	     dout.write(din1.read());
    if (sel1.read() && !sel0.read())
	     dout.write(din2.read());
    if (sel1.read() && sel0.read())
  	   dout.write(din3.read());
}

#endif
