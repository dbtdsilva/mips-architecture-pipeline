#ifndef REG_MEM1_MEM2_H
#define REG_MEM1_MEM2_H

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

SC_MODULE(reg_mem1_mem2_t) {

	// Ports
	sc_in  < bool > clk;
	sc_in  < bool > reset;
	sc_in  < bool > enable;

	sc_in  < sc_uint<32> > aluOut_mem1;
	sc_out < sc_uint<32> > aluOut_mem2;

	sc_in  < sc_uint<5> > WriteReg_mem1;
	sc_out < sc_uint<5> > WriteReg_mem2;

	sc_in  < bool > MemtoReg_mem1, RegWrite_mem1, MemRead_mem1;
	sc_out < bool > MemtoReg_mem2, RegWrite_mem2, MemRead_mem2; 

	sc_in  < sc_uint<32> > PC_mem1;   // only for visualization purposes
	sc_out < sc_uint<32> > PC_mem2;   // only for visualization purposes
	sc_in  < bool > valid_mem1;       // only for visualization purposes
	sc_out < bool > valid_mem2;       // only for visualization purposes

	// Modules
	regT < sc_uint<5> >  *WriteReg;
	regT < bool > *MemtoReg, *RegWrite, *MemRead;

	regT < sc_uint<32> > *aluOut, *PC;        // only for visualization purposes
	regT < bool > *valid;            // only for visualization purposes

	SC_CTOR(reg_mem1_mem2_t) {


		aluOut = new regT < sc_uint<32> > ("aluOut");
		aluOut->din(aluOut_mem1);
		aluOut->dout(aluOut_mem2);
		aluOut->clk(clk);
		aluOut->enable(enable);
		aluOut->reset(reset);

		MemRead = new regT < bool > ("MemRead");
		MemRead->din(MemRead_mem1);
		MemRead->dout(MemRead_mem2);
		MemRead->clk(clk);
		MemRead->enable(enable);
		MemRead->reset(reset);

		WriteReg = new regT < sc_uint<5> > ("WriteReg");
		WriteReg->din(WriteReg_mem1);
		WriteReg->dout(WriteReg_mem2);
		WriteReg->clk(clk);
		WriteReg->enable(enable);
		WriteReg->reset(reset);

		MemtoReg = new regT < bool >("MemtoReg");
		MemtoReg->din(MemtoReg_mem1);
		MemtoReg->dout(MemtoReg_mem2);
		MemtoReg->clk(clk);
		MemtoReg->enable(enable);
		MemtoReg->reset(reset);

		RegWrite = new regT < bool >("RegWrite");
		RegWrite->din(RegWrite_mem1);
		RegWrite->dout(RegWrite_mem2);
		RegWrite->clk(clk);
		RegWrite->enable(enable);
		RegWrite->reset(reset);

		// Visualization only
		PC = new regT < sc_uint<32> > ("PC");
		PC->din(PC_mem1);
		PC->dout(PC_mem2);
		PC->clk(clk);
		PC->enable(enable);
		PC->reset(reset);

		valid = new regT < bool > ("valid");
		valid->din(valid_mem1);
		valid->dout(valid_mem2);
		valid->clk(clk);
		valid->enable(enable);
		valid->reset(reset);
	}
};

#endif
