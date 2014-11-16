#ifndef REG_EXE_MEM1_H
#define REG_EXE_MEM1_H

/**
 *
 * reg_exe_mem_t module interface.
 */

#include <systemc.h>

#include "regT.h"

/**
 * reg_exe_mem_t module.
 * reg_exe_mem_t module is the EXE/MEM pipeline register.
 */

SC_MODULE(reg_exe_mem1_t) {

	// Ports

	sc_in  < bool > clk;
	sc_in  < bool > reset;
	sc_in  < bool > enable;

	sc_in  < sc_uint<32> > aluOut_exe, regb_exe;// BranchTarget_exe;
	sc_out < sc_uint<32> > aluOut_mem1, regb_mem1;// BranchTarget_mem;

	sc_in  < sc_uint<5> > WriteReg_exe, rt_exe;
	sc_out < sc_uint<5> > WriteReg_mem1, rt_mem1;

	sc_in  < bool > MemRead_exe, MemWrite_exe, MemtoReg_exe, RegWrite_exe, Branch_exe, JumpOnRegister_exe;
	sc_out < bool > MemRead_mem1, MemWrite_mem1, MemtoReg_mem1, RegWrite_mem1, Branch_mem1, JumpOnRegister_mem1;

	//sc_in  < bool > Branch_exe, Zero_exe;
	//sc_out < bool > Branch_mem, Zero_mem;

	sc_in  < sc_uint<32> > PC_exe;   // only for visualization purposes
	sc_out < sc_uint<32> > PC_mem1;   // only for visualization purposes
	sc_in  < bool > valid_exe;       // only for visualization purposes
	sc_out < bool > valid_mem1;       // only for visualization purposes

	sc_in < sc_uint<32> > MemOut, WriteVal;
	sc_out < sc_uint<32> > MemOut_fwd_exemem1, WriteVal_fwd_exemem1;
	// Modules

	regT < sc_uint<32> > *writeval, *memout;
	regT < sc_uint<32> > *aluOut, *regb, *BranchTarget;
	regT < sc_uint<5> >  *WriteReg, *rt;
	regT < bool > *MemRead, *MemWrite, *MemtoReg, *RegWrite, *Branch, *JumpOnRegister;

	regT < sc_uint<32> > *PC;        // only for visualization purposes
	regT < bool > *valid;            // only for visualization purposes

	SC_CTOR(reg_exe_mem1_t) {

		aluOut = new regT < sc_uint<32> > ("aluOut");
		aluOut->din(aluOut_exe);
		aluOut->dout(aluOut_mem1);
		aluOut->clk(clk);
		aluOut->enable(enable);
		aluOut->reset(reset);

		memout = new regT <sc_uint<32> > ("memout");
		memout->din(MemOut);
		memout->dout(MemOut_fwd_exemem1);
		memout->clk(clk);
		memout->enable(enable);
		memout->reset(reset);

		writeval = new regT <sc_uint<32> > ("writeval");
		writeval->din(WriteVal);
		writeval->dout(WriteVal_fwd_exemem1);
		writeval->clk(clk);
		writeval->enable(enable);
		writeval->reset(reset);

		regb = new regT < sc_uint<32> > ("regb");
		regb->din(regb_exe);
		regb->dout(regb_mem1);
		regb->clk(clk);
		regb->enable(enable);
		regb->reset(reset);

		WriteReg = new regT < sc_uint<5> > ("WriteReg");
		WriteReg->din(WriteReg_exe);
		WriteReg->dout(WriteReg_mem1);
		WriteReg->clk(clk);
		WriteReg->enable(enable);
		WriteReg->reset(reset);

		rt = new regT < sc_uint<5> > ("rt");
		rt->din(rt_exe);
		rt->dout(rt_mem1);
		rt->clk(clk);
		rt->enable(enable);
		rt->reset(reset);

		MemRead = new regT < bool >("MemRead");
		MemRead->din(MemRead_exe);
		MemRead->dout(MemRead_mem1);
		MemRead->clk(clk);
		MemRead->enable(enable);
		MemRead->reset(reset);

		MemWrite = new regT < bool >("MemWrite");
		MemWrite->din(MemWrite_exe);
		MemWrite->dout(MemWrite_mem1);
		MemWrite->clk(clk);
		MemWrite->enable(enable);
		MemWrite->reset(reset);

		MemtoReg = new regT < bool >("MemtoReg");
		MemtoReg->din(MemtoReg_exe);
		MemtoReg->dout(MemtoReg_mem1);
		MemtoReg->clk(clk);
		MemtoReg->enable(enable);
		MemtoReg->reset(reset);

		Branch = new regT < bool >("Branch");
		Branch->din(Branch_exe);
		Branch->dout(Branch_mem1);
		Branch->clk(clk);
		Branch->enable(enable);
		Branch->reset(reset);

		JumpOnRegister = new regT < bool >("JumpOnRegister");
		JumpOnRegister->din(JumpOnRegister_exe);
		JumpOnRegister->dout(JumpOnRegister_mem1);
		JumpOnRegister->clk(clk);
		JumpOnRegister->enable(enable);
		JumpOnRegister->reset(reset);

		RegWrite = new regT < bool >("RegWrite");
		RegWrite->din(RegWrite_exe);
		RegWrite->dout(RegWrite_mem1);
		RegWrite->clk(clk);
		RegWrite->enable(enable);
		RegWrite->reset(reset);

		// Visualization only
		PC = new regT < sc_uint<32> > ("PC");;
		PC->din(PC_exe);
		PC->dout(PC_mem1);
		PC->clk(clk);
		PC->enable(enable);
		PC->reset(reset);

		valid = new regT < bool > ("valid");;
		valid->din(valid_exe);
		valid->dout(valid_mem1);
		valid->clk(clk);
		valid->enable(enable);
		valid->reset(reset);

	}
};

#endif
