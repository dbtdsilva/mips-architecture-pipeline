#ifndef REG_ID_EXE_H
#define REG_ID_EXE_H

/**
 *
 * reg_id_exe_t module interface.
 */

#include <systemc.h>

#include "regT.h"

/**
 * reg_id_exe_t module.
 * reg_id_exe_t module is the ID/EXE pipeline register. 
 */

SC_MODULE(reg_id_exe_t) {

	// Ports
	
	sc_in  < bool > clk;
	sc_in  < bool > reset;
	sc_in  < bool > enable;

	sc_in  < sc_uint<32> > rega_id, regb_id, imm_id;// PC4_id;
	sc_out < sc_uint<32> > rega_exe, regb_exe, imm_exe;// PC4_exe;

	sc_in  < sc_uint<5> > WriteReg_id, rs, rt;
	sc_out < sc_uint<5> > WriteReg_exe, rs_exe, rt_exe;

	sc_in  < bool > MemRead_id, MemWrite_id, MemtoReg_id; 
	sc_out < bool > MemRead_exe, MemWrite_exe, MemtoReg_exe; 

	sc_in  < bool > ALUSrc_id, RegWrite_id, Branch, JumpOnRegister;
	sc_out < bool > ALUSrc_exe, RegWrite_exe, Branch_exe, JumpOnRegister_exe;

	sc_in  < sc_uint<3> > ALUOp_id;
	sc_out < sc_uint<3> > ALUOp_exe;

	sc_in  < sc_uint<32> > PC_id;   // only for visualization purposes
	sc_out < sc_uint<32> > PC_exe;  // only for visualization purposes
	sc_in  < bool > valid_id;       // only for visualization purposes
	sc_out < bool > valid_exe;      // only for visualization purposes

	sc_in < sc_uint<32> > AluOut, AluOut_mem1, AluOut_mem2, MemOut;
	sc_out < sc_uint<32> > AluOut_fwd_idexe, AluOut_mem1_fwd_idexe, AluOut_mem2_fwd_idexe, MemOut_fwd_idexe;
	// Modules
	
	regT < sc_uint<32> > *rega,*regb,*imm;//*PC4;

	regT < sc_uint<32> > *alu_mem1, *alu_mem2, *alu, *memout;
	
	regT < bool > *MemRead, *MemWrite, *MemtoReg, *RegWrite, *ALUSrc, *Branch_rgt, *Jump;
	regT < sc_uint<5> > *WriteReg, *rsReg, *rtReg;
	regT < sc_uint<3> > *ALUOp;

	regT < sc_uint<32> > *PC;      // only for visualization purposes
	regT < bool > *valid;          // only for visualization purposes

	sc_signal < bool > flag;
	SC_CTOR(reg_id_exe_t) {
		flag = true;

		rega = new regT < sc_uint<32> > ("rega");
		rega->din(rega_id);
		rega->dout(rega_exe);
		rega->clk(clk);
		rega->enable(enable);
		rega->reset(reset);

		rsReg = new regT < sc_uint<5> > ("rsReg");
		rsReg->din(rs);
		rsReg->dout(rs_exe);
		rsReg->clk(clk);
		rsReg->enable(enable);
		rsReg->reset(reset);

		Branch_rgt = new regT < bool > ("Branch_rgt");
		Branch_rgt->din(Branch);
		Branch_rgt->dout(Branch_exe);
		Branch_rgt->clk(clk);
		Branch_rgt->enable(enable);
		Branch_rgt->reset(reset);

		Jump = new regT < bool > ("Jump");
		Jump->din(JumpOnRegister);
		Jump->dout(JumpOnRegister_exe);
		Jump->clk(clk);
		Jump->enable(enable);
		Jump->reset(reset);

		rtReg = new regT < sc_uint<5> > ("rtReg");
		rtReg->din(rt);
		rtReg->dout(rt_exe);
		rtReg->clk(clk);
		rtReg->enable(enable);
		rtReg->reset(reset);

		alu = new regT <sc_uint<32> > ("alu");
		alu->din(AluOut);
		alu->dout(AluOut_fwd_idexe);
		alu->clk(clk);
		alu->enable(flag);
		alu->reset(reset);

		alu_mem1 = new regT <sc_uint<32> > ("alu_mem1");
		alu_mem1->din(AluOut_mem1);
		alu_mem1->dout(AluOut_mem1_fwd_idexe);
		alu_mem1->clk(clk);
		alu_mem1->enable(flag);
		alu_mem1->reset(reset);

		alu_mem2 = new regT <sc_uint<32> > ("alu_mem2");
		alu_mem2->din(AluOut_mem2);
		alu_mem2->dout(AluOut_mem2_fwd_idexe);
		alu_mem2->clk(clk);
		alu_mem2->enable(flag);
		alu_mem2->reset(reset);

		memout = new regT <sc_uint<32> > ("memout");
		memout->din(MemOut);
		memout->dout(MemOut_fwd_idexe);
		memout->clk(clk);
		memout->enable(flag);
		memout->reset(reset);

		regb = new regT < sc_uint<32> >("regb");
		regb->din(regb_id);
		regb->dout(regb_exe);
		regb->clk(clk);
		regb->enable(enable);
		regb->reset(reset);

		WriteReg = new regT < sc_uint<5> >("WriteReg");
		WriteReg->din(WriteReg_id);
		WriteReg->dout(WriteReg_exe);
		WriteReg->clk(clk);
		WriteReg->enable(enable);
		WriteReg->reset(reset);

		/*PC4 = new regT < sc_uint<32> >("PC4");
		PC4->din(PC4_id);
		PC4->dout(PC4_exe);
		PC4->clk(clk);
		PC4->enable(enable);
		PC4->reset(reset);
*/
		imm = new regT < sc_uint<32> >("imm");
		imm->din(imm_id);
		imm->dout(imm_exe);
		imm->clk(clk);
		imm->enable(enable);
		imm->reset(reset);

		MemRead = new regT < bool >("MemRead");
		MemRead->din(MemRead_id);
		MemRead->dout(MemRead_exe);
		MemRead->clk(clk);
		MemRead->enable(enable);
		MemRead->reset(reset);

		MemWrite = new regT < bool >("MemWrite");
		MemWrite->din(MemWrite_id);
		MemWrite->dout(MemWrite_exe);
		MemWrite->clk(clk);
		MemWrite->enable(enable);
		MemWrite->reset(reset);

		MemtoReg = new regT < bool >("MemtoReg");
		MemtoReg->din(MemtoReg_id);
		MemtoReg->dout(MemtoReg_exe);
		MemtoReg->clk(clk);
		MemtoReg->enable(enable);
		MemtoReg->reset(reset);
/*
		Branch = new regT < bool >("Branch");
		Branch->din(Branch_id);
		Branch->dout(Branch_exe);
		Branch->clk(clk);
		Branch->enable(enable);
		Branch->reset(reset);
*/
		RegWrite = new regT < bool >("RegWrite");
		RegWrite->din(RegWrite_id);
		RegWrite->dout(RegWrite_exe);
		RegWrite->clk(clk);
		RegWrite->enable(enable);
		RegWrite->reset(reset);

		ALUSrc = new regT < bool >("ALUSrc");
		ALUSrc->din(ALUSrc_id);
		ALUSrc->dout(ALUSrc_exe);
		ALUSrc->clk(clk);
		ALUSrc->enable(enable);
		ALUSrc->reset(reset);

		ALUOp = new regT < sc_uint<3> >("ALUOp");
		ALUOp->din(ALUOp_id);
		ALUOp->dout(ALUOp_exe);
		ALUOp->clk(clk);
		ALUOp->enable(enable);
		ALUOp->reset(reset);

		PC = new regT < sc_uint<32> >("PC");
		PC->din(PC_id);
		PC->dout(PC_exe);
		PC->clk(clk);
		PC->enable(enable);
		PC->reset(reset);

		valid = new regT < bool >("valid");
		valid->din(valid_id);
		valid->dout(valid_exe);
		valid->clk(clk);
		valid->enable(enable);
		valid->reset(reset);

	}
};

#endif
