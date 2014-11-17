#ifndef HAZARDMOD_H
#define HAZARDMOD_H

/**
 *
 * hazard module interface.
 */

#include <systemc.h>

/**
 * hazard module.
 * hazard module is the hazard detection unit.
 *
 *   - input ports
 *   	- \c sc_uint<5> \c rs	   - first register being read
 *   	- \c sc_uint<5> \c rt	   - second register being read
 *   	- \c sc_uint<5> \c WriteReg_exe	   - register to be written (EXE)
 *   	- \c sc_uint<5> \c WriteReg_mem	   - register to be written (MEM)
 *   	- \c bool \c RegWrite_exe	   - control signal of writing registers (EXE)
 *   	- \c bool \c RegWrite_mem	   - control signal of writing registers (MEM)
 *   - output ports
 *   	- \c bool \c enable_pc      - enables PC update
 *   	- \c bool \c enable_ifid    - enables IF/ID update
 *   	- \c bool \c reset_idexe    - resets IF/EXE
 */

SC_MODULE( hazard )
{
  public:
    sc_in< sc_uint<5> >  rs;
    sc_in< sc_uint<5> >  rt;
    sc_in< sc_uint<5> >  WriteReg_exe, WriteReg_mem1, WriteReg_mem2;
    sc_in< bool >  RegWrite_exe, RegWrite_mem1, RegWrite_mem2, MemRead_mem1, MemRead_mem2, BranchTaken, Branch, Jump;
    sc_out< bool >  enable_pc, enable_ifid, reset_ifid, enable_idexe, reset_idexe, enable_exemem1, reset_exemem1, reset_mem1mem2;

    SC_CTOR(hazard)
    {
        SC_METHOD(detect_hazard);
        sensitive << rs << rt
		        << WriteReg_exe << WriteReg_mem1 << WriteReg_mem2
            << RegWrite_exe << RegWrite_mem1 << RegWrite_mem2
		        << MemRead_mem1 << MemRead_mem2
            << BranchTaken << Branch << Jump;
   }

    void detect_hazard();
};

#endif
