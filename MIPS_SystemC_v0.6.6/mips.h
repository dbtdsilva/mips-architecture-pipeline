//mips.h

#ifndef MIPSMOD_H
#define MIPSMOD_H

/**
 *
 * MIPS module interface.
 */

#include <systemc.h>

#include "imem.h"
#include "decode.h"
#include "regfile.h"
#include "alu.h"
#include "dmem.h"
#include "control.h"
#include "hazard.h"
#include "forward.h"
#include "comparator.h"

#include "mux.h"
#include "mux4.h"
#include "reg.h"
#include "ext.h"
#include "shiftl2.h"
#include "jaddrdecode.h"
#include "add.h"
#include "gates.h"

#include "regT.h"
#include "reg_if_id.h"
#include "reg_id_exe.h"
#include "reg_exe_mem1.h"
#include "reg_mem1_mem2.h"
#include "reg_mem2_wb.h"

/**
 * MIPS module.
 * MIPS module is the main module of the MIPS simulator.
 * Instruction memory, register file, ALU, data memory, etc are instatiated
 * %and interconnected inside this module.
 *
 *   - input ports
 *   	- \c bool \c reset	- reset
 *   	- \c bool \c clk	- clock
 *
 * \image latex MIPS_datapathPipe3.eps "architecture of mips" width=15cm
 */

SC_MODULE(mips) {

   // Ports
   sc_in < bool > clk;
   sc_in < bool > reset;

   // Modules
   // IF
   registo           *PCreg;     // PC register
   imem              *instmem;   // instruction memory
   add *add4;                    // adds 4 to PC
   mux4< sc_uint<32> > *mPC;      // selects Next PC from PCbrach and PC + 4

   //ID
   decode            *dec1;      // decodes instruction
   regfile           *rfile;     // register file
   control           *ctrl;      // control
   comparator        *comp;
   mux4< sc_uint<5> >  *mr;       // selects destination register
   mux< sc_uint<32> > *mrs, *mrt;
   ext *e1;                      // sign extends imm to 32 bits
   xorgate *xor1;
   orgate *or_reset_idexe;
   orgate *or_reset_ifid;
   hazard *hazard_unit;
   jaddrdecode *jAddrDecode;
   //EXE
   alu               *alu1;      // ALU
   mux< sc_uint<32> > *m1;       // selects 2nd ALU operand
   shiftl2 *sl2;                 // shift left 2 imm_ext
   add *addbr;                   // adds imm to PC + 4

   //MEM
   dmem              *datamem;   // data memory
   andgate *a1;                  // beq instruction and equal values

   //WB
   mux< sc_uint<32> > *m2;       // selects value to write in register (ALUout or MemOut)

   //pipeline registers
   reg_if_id_t       *reg_if_id;
   reg_id_exe_t      *reg_id_exe;
   reg_exe_mem1_t     *reg_exe_mem1;
   reg_mem1_mem2_t    *reg_mem1_mem2;
   reg_mem2_wb_t      *reg_mem2_wb;

   // Signals
   /* FORWARD RELATED */
   forward *forward_unit;
   sc_signal <sc_uint<5> > rs_exe, rt_exe, rt_mem1;
   sc_signal <sc_uint<32> > AluOut_fwd_ifid, AluOut_mem1_fwd_ifid;
   sc_signal <sc_uint<32> > AluOut_fwd_idexe, AluOut_mem1_fwd_idexe, AluOut_mem2_fwd_idexe, MemOut_fwd_idexe;
   sc_signal <sc_uint<32> > MemOut_fwd_exemem1, WriteVal_fwd_exemem1;

   sc_signal <bool> forward_idexe_rs0, forward_idexe_rs1, forward_idexe_rt0, forward_idexe_rt1, forward_idexe_rtActive, forward_idexe_rsActive,
                              forward_exemem1_regb0, forward_exemem1_regb1;

   //sc_signal <bool> forward_exe_rs0, forward_exe_rs1, forward_exe_rt0, forward_exe_rt1, forward_exe_rtActive, forward_exe_rsActive,
   //                           forward_mem1_regb0, forward_mem1_regb1;
   sc_signal <bool> forward_ifid_rs0, forward_ifid_rs1, forward_ifid_rt0, forward_ifid_rt1;
   //sc_signal <bool> forward_id_rs0, forward_id_rs1, forward_id_rt0, forward_id_rt1, forward_id_rtActive, forward_id_rsActive;

   sc_signal <sc_uint<32> > ValRS_fwd_idexe, ValRT_fwd_idexe, ResultRS_fwd_exe, ResultRT_fwd_exe;
   sc_signal <sc_uint<32> > ResultRDD_fwd_mem1, ValRT_fwd_ifid, ValRS_fwd_ifid, ResultRT_fwd_id, ResultRS_fwd_id;
   mux<sc_uint<32> > *rsActive_exe, *rtActive_exe;
   mux4<sc_uint<32> > *rtFwd_exe, *rsFwd_exe, *rsFwd_id, *rtFwd_id, *rddFwd_mem1;

   /* End */

   // IF
   sc_signal < sc_uint<32> > PC,       // Program Counter
                             NPC,      // Next Program Counter
			     PC4;      // PC + 4
   sc_signal < sc_uint<32> > inst;     // current instruction
   sc_signal <bool> enable_pc;

   sc_signal <bool> enable_ifid;

   //ID
   sc_signal < sc_uint<32> > inst_id,  // current instruction ID phase
                             PC4_id;
   sc_signal < sc_uint<32> > const0;
   sc_signal < sc_uint<5> > const31;
   // instruction fields
   sc_signal <bool> equal, BranchResult, BranchNotEqual;
   sc_signal <bool> Jump, JumpOnRegister, JumpOnRegister_exe, JumpOnRegister_mem1;
   sc_signal < sc_uint<32> > addr_ext; // imm_ext shift left 2
   sc_signal < sc_uint<5> > rs, rt, rd;
   sc_signal < sc_uint<16> > imm;
   sc_signal < sc_uint<6> > opcode;
   sc_signal < sc_uint<5> > shamt;
   sc_signal < sc_uint<6> > funct;
   sc_signal < sc_uint<32> > JumpAddr;

   // register file signals
   sc_signal < sc_uint<5> > WriteReg;  // register to write

   sc_signal < sc_uint<32> > regdata1, regdata1_mux, // value of register rs
                             regdata2, regdata2_mux, // value of regiter rt
			     WriteVal; // value to write in register WriteReg

   sc_signal < sc_uint<32> > imm_ext;  // imm sign extended

   sc_signal < sc_uint<32> > rega_exe, // value of register rs EXE phase
                             regb_exe, // value of regiter rt EXE phase
                             regb_mem1; // value of regiter rt MEM phase

   sc_signal <bool> reset_haz_idexe, reset_idexe, reset_haz_ifid, reset_ifid;
   // control signals
   sc_signal <bool> MemRead, MemWrite, MemtoReg;
   sc_signal <bool> RegWrite, RegDst;
   sc_signal <bool> ALUSrc;
   sc_signal < sc_uint<3> > ALUOp;
   sc_signal <bool> Branch, Branch_exe;

   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_id;      // PC of instruction in ID
   sc_signal < bool >        valid_id;   // true if there is an instruction in ID

   //EXE
   sc_signal < bool > Zero;            // ALU output is zero
   sc_signal < sc_uint<32> > imm_exe; //PC4_exe;
   sc_signal < sc_uint<5> > WriteReg_exe;
   // ALU signals
   sc_signal < sc_uint<32> > ALUIn2,   // ALU second operand
                             ALUOut;   // ALU Output
   sc_signal <bool> MemRead_exe, MemWrite_exe, MemtoReg_exe;
   sc_signal <bool> RegWrite_exe;
   sc_signal <bool> ALUSrc_exe;
   sc_signal < sc_uint<3> > ALUOp_exe;
   //sc_signal <bool> Branch_exe;

   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_exe;     // PC of instruction in ID
   sc_signal < bool > valid_exe;         // true if there is an instruction in ID

   //MEM1
   sc_signal < sc_uint<32> > ALUOut_mem1;// BranchTarget_mem;
   sc_signal < sc_uint<5> > WriteReg_mem1;
   sc_signal <bool> MemRead_mem1, MemWrite_mem1, MemtoReg_mem1;
   sc_signal <bool> RegWrite_mem1;
   sc_signal <bool> Branch_mem1;

   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_mem1;
   sc_signal < bool > valid_mem1;

   //MEM2
   sc_signal < sc_uint<32> > ALUOut_mem2;
   sc_signal < sc_uint<32> > MemOut;   // data memory output
   sc_signal < sc_uint<5> > WriteReg_mem2;
   sc_signal <bool> RegWrite_mem2, MemtoReg_mem2, MemRead_mem2, MemRead_wb;
   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_mem2;
   sc_signal < bool > valid_mem2; //

   //WB
   sc_signal < sc_uint<32> > MemOut_wb, ALUOut_wb;
   sc_signal < sc_uint<5> > WriteReg_wb;
   sc_signal <bool> MemtoReg_wb;
   sc_signal <bool> RegWrite_wb;

   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_wb;
   sc_signal < bool > valid_wb;

   //nonpipelined signals
   sc_signal < sc_uint<32> > BranchTarget; // PC if branch
   sc_signal < bool > BranchTaken;       // execute branch
   sc_signal < sc_uint<32> > const4;   // contant 4
   sc_signal < bool > const1;          // contant 4

   SC_CTOR(mips) {
       buildArchitecture();
   }

   ~mips(void);

   void buildArchitecture();

   void buildIF();
   void buildID();
   void buildEXE();
   void buildMEM1();
   void buildMEM2();
   void buildWB();
};

#endif
