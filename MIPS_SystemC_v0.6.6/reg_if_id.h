#ifndef REG_IF_ID_H
#define REG_IF_ID_H

/**
 *
 * reg_if_id_t module interface.
 */

#include <systemc.h>

#include "regT.h"

/**
 * reg_if_id_t module.
 * reg_if_id_t module is the IF/ID pipeline register.
 */

SC_MODULE(reg_if_id_t) {

	// Ports

	sc_in  < bool > clk;
	sc_in  < bool > reset;
	sc_in  < bool > enable;

	sc_in  < sc_uint<32> > inst_if, PC4_if;
	sc_out < sc_uint<32> > inst_id, PC4_id;

	sc_in  < sc_uint<32> > PC_if;     // only for visualization purposes
	sc_in  < bool >        valid_if;  // only for visualization purposes
	sc_out < sc_uint<32> > PC_id;     // only for visualization purposes
	sc_out < bool >        valid_id;  // only for visualization purposes
	sc_in < sc_uint<32> > AluOut, AluOut_mem1, AluOut_mem2, MemOut;
	sc_out < sc_uint<32> > AluOut_fwd_ifid, AluOut_mem1_fwd_ifid, AluOut_mem2_fwd_ifid, MemOut_fwd_ifid;

	sc_in <bool> forward_ifid_rs0, forward_ifid_rs1, forward_ifid_rt0, forward_ifid_rt1, forward_ifid_rtActive, forward_ifid_rsActive;
	sc_out <bool> forward_id_rs0, forward_id_rs1, forward_id_rt0, forward_id_rt1, forward_id_rtActive, forward_id_rsActive;
	// Modules

	regT < sc_uint<32> > *inst, *PC4;

	regT < bool > *forward_rs0, *forward_rs1, *forward_rt0, *forward_rt1, *forward_rsActive, *forward_rtActive;
	regT < sc_uint<32> > *alu_mem1, *alu_mem2, *alu, *memout;
	regT < sc_uint<32> > *PC;        // only for visualization purposes
	regT < bool > *valid;            // only for visualization purposes

	SC_CTOR(reg_if_id_t) {

		inst = new regT < sc_uint<32> > ("inst");
		inst->din(inst_if);
		inst->dout(inst_id);
		inst->clk(clk);
		inst->enable(enable);
		inst->reset(reset);

		forward_rs0 = new regT < bool >("forward_rs0");
		forward_rs0->din(forward_ifid_rs0);
		forward_rs0->dout(forward_id_rs0);
		forward_rs0->clk(clk);
		forward_rs0->enable(enable);
		forward_rs0->reset(reset);

		forward_rs1 = new regT < bool >("forward_rs1");
		forward_rs1->din(forward_ifid_rs1);
		forward_rs1->dout(forward_id_rs1);
		forward_rs1->clk(clk);
		forward_rs1->enable(enable);
		forward_rs1->reset(reset);

		forward_rt0 = new regT < bool >("forward_rt0");
		forward_rt0->din(forward_ifid_rt0);
		forward_rt0->dout(forward_id_rt0);
		forward_rt0->clk(clk);
		forward_rt0->enable(enable);
		forward_rt0->reset(reset);

		forward_rt1 = new regT < bool >("forward_rt1");
		forward_rt1->din(forward_ifid_rt1);
		forward_rt1->dout(forward_id_rt1);
		forward_rt1->clk(clk);
		forward_rt1->enable(enable);
		forward_rt1->reset(reset);

		forward_rsActive = new regT < bool >("forward_rsActive");
		forward_rsActive->din(forward_ifid_rsActive);
		forward_rsActive->dout(forward_id_rsActive);
		forward_rsActive->clk(clk);
		forward_rsActive->enable(enable);
		forward_rsActive->reset(reset);

		forward_rtActive = new regT < bool >("forward_rtActive");
		forward_rtActive->din(forward_ifid_rtActive);
		forward_rtActive->dout(forward_id_rtActive);
		forward_rtActive->clk(clk);
		forward_rtActive->enable(enable);
		forward_rtActive->reset(reset);

		alu = new regT <sc_uint<32> > ("alu");
		alu->din(AluOut);
		alu->dout(AluOut_fwd_ifid);
		alu->clk(clk);
		alu->enable(enable);
		alu->reset(reset);

		alu_mem1 = new regT <sc_uint<32> > ("alu_mem1");
		alu_mem1->din(AluOut_mem1);
		alu_mem1->dout(AluOut_mem1_fwd_ifid);
		alu_mem1->clk(clk);
		alu_mem1->enable(enable);
		alu_mem1->reset(reset);

		alu_mem2 = new regT <sc_uint<32> > ("alu_mem2");
		alu_mem2->din(AluOut_mem2);
		alu_mem2->dout(AluOut_mem2_fwd_ifid);
		alu_mem2->clk(clk);
		alu_mem2->enable(enable);
		alu_mem2->reset(reset);

		memout = new regT <sc_uint<32> > ("memout");
		memout->din(MemOut);
		memout->dout(MemOut_fwd_ifid);
		memout->clk(clk);
		memout->enable(enable);
		memout->reset(reset);

		PC4 = new regT < sc_uint<32> > ("PC4");
		PC4->din(PC4_if);
		PC4->dout(PC4_id);
		PC4->clk(clk);
		PC4->enable(enable);
		PC4->reset(reset);

		PC = new regT < sc_uint<32> > ("PC");
		PC->din(PC_if);
		PC->dout(PC_id);
		PC->clk(clk);
		PC->enable(enable);
		PC->reset(reset);

		valid = new regT < bool > ("valid");
		valid->din(valid_if);
		valid->dout(valid_id);
		valid->clk(clk);
		valid->enable(enable);
		valid->reset(reset);

	}
};

#endif
