#ifndef FORWARD_H
#define FORWARD_H

#include <systemc.h>

SC_MODULE( forward )
{
  public:
    sc_in <sc_uint<5> > WriteReg_exe, WriteReg_mem1, WriteReg_mem2, WriteReg_wb;
    sc_in <sc_uint<5> > rs, rt, rs_exe, rt_exe;
    sc_in <bool> Branch;
    // Control signals
    sc_in<bool> RegWrite_exe, RegWrite_mem1, RegWrite_mem2, RegWrite_wb, MemRead_exe, MemRead_mem1, MemRead_mem2, MemWrite_exe, MemWrite;

    sc_out<bool> forward_idexe_rs0, forward_idexe_rs1, forward_idexe_rt0, forward_idexe_rt1;
    sc_out<bool> forward_idexe_rtActive, forward_idexe_rsActive;
    sc_out<bool> forward_ifid_rs0, forward_ifid_rs1, forward_ifid_rt0, forward_ifid_rt1;
    sc_out<bool> forward_ifid_rtActive, forward_ifid_rsActive;
    sc_out<bool> forward_exemem1_regb0, forward_exemem1_regb1;

    SC_CTOR(forward)
    {
        SC_METHOD(detect_forward);
        sensitive << WriteReg_exe << WriteReg_mem1 << WriteReg_mem2 << WriteReg_wb
                  << rs << rt << rs_exe << rt_exe
                  << Branch
                  << RegWrite_exe << RegWrite_mem1 << RegWrite_mem2 << RegWrite_wb
                  << MemRead_exe << MemRead_mem1 << MemRead_mem2
                  << MemWrite_exe << MemWrite;
   }

    void detect_forward();
};

#endif