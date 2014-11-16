//mips.cpp

/**
 *
 * MIPS module implementation.
 */

#include "mips.h"

/**
 * buils IF stage components
 */
void mips::buildIF(void)
{
	// Program Counter
	PCreg = new registo("PCregister");
	PCreg->din(NPC);
	PCreg->dout(PC);
	PCreg->clk(clk);
	PCreg->reset(reset);
	PCreg->enable(enable_pc);
	// Instruction Memory
	instmem = new imem("imem");
	instmem->addr(PC);
	instmem->inst(inst);
	// Adds 4 to Program Counter
	add4 = new add("add4");
	add4->op1(PC);
	add4->op2(const4);
	add4->res(PC4);
	// Selects Next Program Counter Value
	mPC = new mux4< sc_uint<32> >("mPC");
	mPC->sel0(BranchTaken);
	mPC->sel1(Jump);
	mPC->din0(PC4);
	mPC->din1(BranchTarget);
	mPC->din2(JumpAddr);
	mPC->din3(JumpAddr);
	mPC->dout(NPC);
}

/**
 * buils ID stage components
 */
void mips::buildID(void)
{
	// Decodes Instruction
	dec1 = new decode("decode");
	dec1->inst(inst_id);
	dec1->rs(rs);
	dec1->rt(rt);
	dec1->rd(rd);
	dec1->imm(imm);
	dec1->opcode(opcode);
	dec1->shamt(shamt);
	dec1->funct(funct);
	// Register File
	rfile = new regfile("regfile");
	rfile->reg1(rs);
	rfile->reg2(rt);
	rfile->regwrite(WriteReg_wb);
	rfile->data1(regdata1_mux);
	rfile->data2(regdata2_mux);
	rfile->wr(RegWrite_wb);
	rfile->datawr(WriteVal);
	rfile->clk(clk);
	rfile->reset(reset);

	rsFwd_id = new mux4< sc_uint<32> >("rsFwd_id");
	rsFwd_id->sel0(forward_id_rs0);
	rsFwd_id->sel1(forward_id_rs1);
	rsFwd_id->din0(AluOut_fwd_ifid);
	rsFwd_id->din1(AluOut_mem1_fwd_ifid);
	rsFwd_id->din2(AluOut_mem2_fwd_ifid);
	rsFwd_id->din3(MemOut_fwd_ifid);
	rsFwd_id->dout(ValRS_fwd_ifid);

	rsActive_id = new mux< sc_uint<32> >("rsActive_id");
	rsActive_id->sel(forward_id_rsActive);
	rsActive_id->din0(regdata1_mux);
	rsActive_id->din1(ValRS_fwd_ifid);
	rsActive_id->dout(ResultRS_fwd_id);

	rtFwd_id = new mux4< sc_uint<32> >("rtFwd_id");
	rtFwd_id->sel0(forward_id_rt0);
	rtFwd_id->sel1(forward_id_rt1);
	rtFwd_id->din0(AluOut_fwd_ifid);
	rtFwd_id->din1(AluOut_mem1_fwd_ifid);
	rtFwd_id->din2(AluOut_mem2_fwd_ifid);
	rtFwd_id->din3(MemOut_fwd_ifid);
	rtFwd_id->dout(ValRT_fwd_ifid);

	rtActive_id = new mux< sc_uint<32> >("rtActive_id");
	rtActive_id->sel(forward_id_rtActive);
	rtActive_id->din0(regdata2_mux);
	rtActive_id->din1(ValRT_fwd_ifid);
	rtActive_id->dout(ResultRT_fwd_id);
	// 16 to 32 bit signed Immediate extension
	e1 = new ext("ext");
	e1->din(imm);
	e1->dout(imm_ext);
	// shift left 2 imm_ext
	sl2 = new shiftl2("sl2");
	sl2->din(imm_ext);
	sl2->dout(addr_ext);
	// Adds Branch Immediate to Program Counter + 4
	addbr = new add("addbr");
	addbr->op1(PC4_id);
	addbr->op2(addr_ext);
	addbr->res(BranchTarget);
	// Control
	ctrl = new control("control");
	ctrl->opcode(opcode);
	ctrl->funct(funct);
	ctrl->RegDst(RegDst);
	ctrl->Branch(Branch);
	ctrl->MemRead(MemRead);
	ctrl->MemWrite(MemWrite);
	ctrl->MemtoReg(MemtoReg);
	ctrl->ALUOp(ALUOp);
	ctrl->ALUSrc(ALUSrc);
	ctrl->RegWrite(RegWrite);
	ctrl->BranchNotEqual(BranchNotEqual);
	ctrl->Jump(Jump);
	ctrl->JumpOnRegister(JumpOnRegister);
	mrs = new mux< sc_uint<32> >("muxRs");
	mrs->sel(Jump);
	mrs->din0(ResultRS_fwd_id);
	mrs->din1(PC4_id);
	mrs->dout(regdata1);
	mrt = new mux< sc_uint<32> >("muxRt");
	mrt->sel(Jump);
	mrt->din0(ResultRT_fwd_id);
	mrt->din1(const0);
	mrt->dout(regdata2);
	// Selects Register to Write
	mr = new mux4< sc_uint<5> >("muxRDst");
	// Jump | RegDst
	mr->sel1(Jump);
	mr->sel0(RegDst);
	mr->din0(rt);
	mr->din1(rd);
	mr->din2(const31);
	mr->din3(const31);
	mr->dout(WriteReg);
	jAddrDecode = new jaddrdecode("jAddrDecode");
	jAddrDecode->inst(inst_id);
	jAddrDecode->PC_id(PC_id);
	jAddrDecode->JumpOnRegister(JumpOnRegister);
	jAddrDecode->data1(ResultRS_fwd_id);
	jAddrDecode->out(JumpAddr);
	comp = new comparator("comp");
	comp->din1(regdata1);
	comp->din2(regdata2);
	comp->equal(equal);
	xor1 = new xorgate("xor1");
	xor1->din1(equal);
	xor1->din2(BranchNotEqual);
	xor1->dout(BranchResult);
	a1 = new andgate("a1");
	a1->din1(Branch);
	a1->din2(BranchResult);
	a1->dout(BranchTaken);
}

/**
 * buils EXE stage components
 */
void mips::buildEXE(void)
{
	rsFwd_exe = new mux4< sc_uint<32> >("rsFwd_exe");
	rsFwd_exe->sel0(forward_exe_rs0);
	rsFwd_exe->sel1(forward_exe_rs1);
	rsFwd_exe->din0(AluOut_fwd_idexe);
	rsFwd_exe->din1(AluOut_mem1_fwd_idexe);
	rsFwd_exe->din2(AluOut_mem2_fwd_idexe);
	rsFwd_exe->din3(MemOut_fwd_idexe);
	rsFwd_exe->dout(ValRS_fwd_idexe);

	rsActive_exe = new mux< sc_uint<32> >("rsActive_exe");
	rsActive_exe->sel(forward_exe_rsActive);
	rsActive_exe->din0(rega_exe);
	rsActive_exe->din1(ValRS_fwd_idexe);
	rsActive_exe->dout(ResultRS_fwd_exe);

	rtFwd_exe = new mux4< sc_uint<32> >("rtFwd_exe");
	rtFwd_exe->sel0(forward_exe_rt0);
	rtFwd_exe->sel1(forward_exe_rt1);
	rtFwd_exe->din0(AluOut_fwd_idexe);
	rtFwd_exe->din1(AluOut_mem1_fwd_idexe);
	rtFwd_exe->din2(AluOut_mem2_fwd_idexe);
	rtFwd_exe->din3(MemOut_fwd_idexe);
	rtFwd_exe->dout(ValRT_fwd_idexe);

	rtActive_exe = new mux< sc_uint<32> >("rtActive_exe");
	rtActive_exe->sel(forward_exe_rtActive);
	rtActive_exe->din0(regb_exe);
	rtActive_exe->din1(ValRT_fwd_idexe);
	rtActive_exe->dout(ResultRT_fwd_exe);

	// Selects second operand of ALU
	m1 = new mux< sc_uint<32> >("muxOp");
	m1->sel(ALUSrc_exe);
	m1->din0(ResultRT_fwd_exe);
	m1->din1(imm_exe);
	m1->dout(ALUIn2);
	// ALU
	alu1 = new alu("alu");
	alu1->din1(ResultRS_fwd_exe);
	alu1->din2(ALUIn2);
	alu1->op(ALUOp_exe);
	alu1->dout(ALUOut);
	alu1->zero(Zero);
}

/**
 * buils MEM stage components
 */
void mips::buildMEM1(void)
{
	rddFwd_mem1 = new mux4<sc_uint<32> >("rddFwd_mem1");
	rddFwd_mem1->sel0(forward_mem1_regb0);
	rddFwd_mem1->sel1(forward_mem1_regb1);
	rddFwd_mem1->din0(regb_mem1);
	rddFwd_mem1->din1(MemOut);
	rddFwd_mem1->din2(WriteVal);
	rddFwd_mem1->din3(WriteVal);
	rddFwd_mem1->dout(ResultRDD_fwd_mem1);

	// Data Memory
	datamem = new dmem("datamem");
	datamem->addr(ALUOut_mem1);
	datamem->din(ResultRDD_fwd_mem1);
	datamem->wr(MemWrite_mem1);
	datamem->rd(MemRead_mem1);
	datamem->clk(clk);
}

void mips::buildMEM2(void)
{
	datamem->dout(MemOut);
}
/**
 * buils WB stage components
 */
void mips::buildWB(void)
{
	// Selects Result
	m2 = new mux< sc_uint<32> >("muxRes");
	m2->sel(MemtoReg_wb);
	m2->din0(ALUOut_wb);
	m2->din1(MemOut_wb);
	m2->dout(WriteVal);
}

/**
 * Instantiates the pipeline registers and calls other functions to
 * buils stage specific components
 */
void mips::buildArchitecture(void)
{
	const4 = 4;
	const31 = 31;
	const0 = 0;
	const1 = true;

	buildIF();

	//reg_if_id
	reg_if_id = new reg_if_id_t("reg_if_id");
	reg_if_id->inst_if(inst);
	reg_if_id->inst_id(inst_id);

	reg_if_id->AluOut(ALUOut);
	reg_if_id->AluOut_fwd_ifid(AluOut_fwd_ifid);
	reg_if_id->AluOut_mem1(ALUOut_mem1);
	reg_if_id->AluOut_mem1_fwd_ifid(AluOut_mem1_fwd_ifid);
	reg_if_id->AluOut_mem2(ALUOut_mem2);
	reg_if_id->AluOut_mem2_fwd_ifid(AluOut_mem2_fwd_ifid);
	reg_if_id->MemOut(MemOut);
	reg_if_id->MemOut_fwd_ifid(MemOut_fwd_ifid);
	reg_if_id->forward_ifid_rs0(forward_ifid_rs0);
	reg_if_id->forward_id_rs0(forward_id_rs0);
	reg_if_id->forward_ifid_rs1(forward_ifid_rs1);
	reg_if_id->forward_id_rs1(forward_id_rs1);
	reg_if_id->forward_ifid_rt0(forward_ifid_rt0);
	reg_if_id->forward_id_rt0(forward_id_rt0);
	reg_if_id->forward_ifid_rt1(forward_ifid_rt1);
	reg_if_id->forward_id_rt1(forward_id_rt1);
	reg_if_id->forward_ifid_rsActive(forward_ifid_rsActive);
	reg_if_id->forward_id_rsActive(forward_id_rsActive);
	reg_if_id->forward_ifid_rtActive(forward_ifid_rtActive);
	reg_if_id->forward_id_rtActive(forward_id_rtActive);

	reg_if_id->PC4_if(PC4);
	reg_if_id->PC4_id(PC4_id);
	reg_if_id->PC_if(PC);
	reg_if_id->PC_id(PC_id);
	reg_if_id->valid_if(const1);
	reg_if_id->valid_id(valid_id);
	reg_if_id->clk(clk);
	reg_if_id->reset(reset_ifid);
	reg_if_id->enable(enable_ifid);
	or_reset_ifid = new orgate("or_reset_ifid");
	or_reset_ifid->din1(reset);
	or_reset_ifid->din2(reset_haz_ifid);
	or_reset_ifid->dout(reset_ifid);

	buildID();

	//reg_id_exe
	reg_id_exe = new reg_id_exe_t("reg_id_exe");
	reg_id_exe->rega_id(regdata1);
	reg_id_exe->rega_exe(rega_exe);
	reg_id_exe->regb_id(regdata2);
	reg_id_exe->regb_exe(regb_exe);
	reg_id_exe->rs(rs);
	reg_id_exe->rs_exe(rs_exe);
	reg_id_exe->rt(rt);
	reg_id_exe->rt_exe(rt_exe);

	reg_id_exe->AluOut(ALUOut);
	reg_id_exe->AluOut_fwd_idexe(AluOut_fwd_idexe);
	reg_id_exe->AluOut_mem1(ALUOut_mem1);
	reg_id_exe->AluOut_mem1_fwd_idexe(AluOut_mem1_fwd_idexe);
	reg_id_exe->AluOut_mem2(ALUOut_mem2);
	reg_id_exe->AluOut_mem2_fwd_idexe(AluOut_mem2_fwd_idexe);
	reg_id_exe->MemOut(MemOut);
	reg_id_exe->MemOut_fwd_idexe(MemOut_fwd_idexe);
	reg_id_exe->forward_idexe_rs0(forward_idexe_rs0);
	reg_id_exe->forward_exe_rs0(forward_exe_rs0);
	reg_id_exe->forward_idexe_rs1(forward_idexe_rs1);
	reg_id_exe->forward_exe_rs1(forward_exe_rs1);
	reg_id_exe->forward_idexe_rt0(forward_idexe_rt0);
	reg_id_exe->forward_exe_rt0(forward_exe_rt0);
	reg_id_exe->forward_idexe_rt1(forward_idexe_rt1);
	reg_id_exe->forward_exe_rt1(forward_exe_rt1);
	reg_id_exe->forward_idexe_rsActive(forward_idexe_rsActive);
	reg_id_exe->forward_exe_rsActive(forward_exe_rsActive);
	reg_id_exe->forward_idexe_rtActive(forward_idexe_rtActive);
	reg_id_exe->forward_exe_rtActive(forward_exe_rtActive);

	reg_id_exe->imm_id(imm_ext);
	reg_id_exe->imm_exe(imm_exe);
	reg_id_exe->WriteReg_id(WriteReg);
	reg_id_exe->WriteReg_exe(WriteReg_exe);
	reg_id_exe->MemRead_id(MemRead);
	reg_id_exe->MemRead_exe(MemRead_exe);
	reg_id_exe->MemWrite_id(MemWrite);
	reg_id_exe->MemWrite_exe(MemWrite_exe);
	reg_id_exe->MemtoReg_id(MemtoReg);
	reg_id_exe->MemtoReg_exe(MemtoReg_exe);
	reg_id_exe->RegWrite_id(RegWrite);
	reg_id_exe->RegWrite_exe(RegWrite_exe);
	reg_id_exe->ALUSrc_id(ALUSrc);
	reg_id_exe->ALUSrc_exe(ALUSrc_exe);
	reg_id_exe->ALUOp_id(ALUOp);
	reg_id_exe->ALUOp_exe(ALUOp_exe);
	reg_id_exe->PC_id(PC_id);
	reg_id_exe->PC_exe(PC_exe);
	reg_id_exe->valid_id(valid_id);
	reg_id_exe->valid_exe(valid_exe);
	reg_id_exe->clk(clk);
	reg_id_exe->reset(reset_idexe);
	reg_id_exe->enable(const1);
	or_reset_idexe = new orgate("or_reset_idexe");
	or_reset_idexe->din1(reset);
	or_reset_idexe->din2(reset_haz_idexe);
	or_reset_idexe->dout(reset_idexe);

	buildEXE();

	//reg_exe_mem
	reg_exe_mem1 = new reg_exe_mem1_t("reg_exe_mem1");

	reg_exe_mem1->WriteVal(WriteVal);
	reg_exe_mem1->WriteVal_fwd_exemem1(WriteVal_fwd_exemem1);
	reg_exe_mem1->MemOut(MemOut);
	reg_exe_mem1->MemOut_fwd_exemem1(MemOut_fwd_exemem1);
	reg_exe_mem1->forward_exemem1_sel0(forward_exemem1_regb0);
	reg_exe_mem1->forward_exemem1_sel1(forward_exemem1_regb1);
	reg_exe_mem1->forward_mem1_sel0(forward_mem1_regb0);
	reg_exe_mem1->forward_mem1_sel1(forward_mem1_regb1);

	reg_exe_mem1->aluOut_exe(ALUOut);
	reg_exe_mem1->aluOut_mem1(ALUOut_mem1);
	reg_exe_mem1->MemRead_exe(MemRead_exe);
	reg_exe_mem1->MemRead_mem1(MemRead_mem1);
	reg_exe_mem1->MemWrite_exe(MemWrite_exe);
	reg_exe_mem1->MemWrite_mem1(MemWrite_mem1);
	reg_exe_mem1->MemtoReg_exe(MemtoReg_exe);
	reg_exe_mem1->MemtoReg_mem1(MemtoReg_mem1);
	reg_exe_mem1->RegWrite_exe(RegWrite_exe);
	reg_exe_mem1->RegWrite_mem1(RegWrite_mem1);
	reg_exe_mem1->regb_exe(ResultRT_fwd_exe);
	reg_exe_mem1->regb_mem1(regb_mem1);
	reg_exe_mem1->WriteReg_exe(WriteReg_exe);
	reg_exe_mem1->WriteReg_mem1(WriteReg_mem1);
	reg_exe_mem1->PC_exe(PC_exe);
	reg_exe_mem1->PC_mem1(PC_mem1);
	reg_exe_mem1->valid_exe(valid_exe);
	reg_exe_mem1->valid_mem1(valid_mem1);
	reg_exe_mem1->clk(clk);
	reg_exe_mem1->reset(reset);
	reg_exe_mem1->enable(const1);

	buildMEM1();

	reg_mem1_mem2 = new reg_mem1_mem2_t("reg_mem1_mem2");
	reg_mem1_mem2->aluOut_mem1(ALUOut_mem1);
	reg_mem1_mem2->aluOut_mem2(ALUOut_mem2);
	reg_mem1_mem2->MemRead_mem1(MemRead_mem1);
	reg_mem1_mem2->MemRead_mem2(MemRead_mem2);
	reg_mem1_mem2->WriteReg_mem1(WriteReg_mem1);
	reg_mem1_mem2->WriteReg_mem2(WriteReg_mem2);
	reg_mem1_mem2->MemtoReg_mem1(MemtoReg_mem1);
	reg_mem1_mem2->MemtoReg_mem2(MemtoReg_mem2);
	reg_mem1_mem2->RegWrite_mem1(RegWrite_mem1);
	reg_mem1_mem2->RegWrite_mem2(RegWrite_mem2);
	reg_mem1_mem2->PC_mem1(PC_mem1);
	reg_mem1_mem2->PC_mem2(PC_mem2);
	reg_mem1_mem2->valid_mem1(valid_mem1);
	reg_mem1_mem2->valid_mem2(valid_mem2);
	reg_mem1_mem2->clk(clk);
	reg_mem1_mem2->reset(reset);
	reg_mem1_mem2->enable(const1);

	buildMEM2();

	//reg_mem_wb
	reg_mem2_wb = new reg_mem2_wb_t("reg_mem2_wb");
	reg_mem2_wb->aluOut_mem2(ALUOut_mem2);
	reg_mem2_wb->aluOut_wb(ALUOut_wb);
	reg_mem2_wb->memOut_mem2(MemOut);
	reg_mem2_wb->memOut_wb(MemOut_wb);
	reg_mem2_wb->MemtoReg_mem2(MemtoReg_mem2);
	reg_mem2_wb->MemtoReg_wb(MemtoReg_wb);
	reg_mem2_wb->RegWrite_mem2(RegWrite_mem2);
	reg_mem2_wb->RegWrite_wb(RegWrite_wb);
	reg_mem2_wb->WriteReg_mem2(WriteReg_mem2);
	reg_mem2_wb->WriteReg_wb(WriteReg_wb);
	reg_mem2_wb->PC_mem2(PC_mem2);
	reg_mem2_wb->PC_wb(PC_wb);
	reg_mem2_wb->valid_mem2(valid_mem2);
	reg_mem2_wb->valid_wb(valid_wb);
	reg_mem2_wb->clk(clk);
	reg_mem2_wb->reset(reset);
	reg_mem2_wb->enable(const1);

	buildWB();

	forward_unit = new forward("forward_unit");
	forward_unit->WriteReg_exe(WriteReg_exe);
	forward_unit->WriteReg_mem1(WriteReg_mem1);
	forward_unit->WriteReg_mem2(WriteReg_mem2);
	forward_unit->WriteReg_wb(WriteReg_wb);
    forward_unit->rs(rs);
    forward_unit->rt(rt);
    forward_unit->rs_exe(rs_exe);
    forward_unit->rt_exe(rt_exe);

    forward_unit->Branch(Branch);
    forward_unit->RegWrite_exe(RegWrite_exe);
    forward_unit->RegWrite_mem1(RegWrite_mem1);
    forward_unit->RegWrite_mem2(RegWrite_mem2);
    forward_unit->RegWrite_wb(RegWrite_wb);
    forward_unit->MemRead_exe(MemRead_exe);
    forward_unit->MemRead_mem1(MemRead_mem1);
    forward_unit->MemRead_mem2(MemRead_mem2);
    forward_unit->MemWrite_exe(MemWrite_exe);
    forward_unit->MemWrite(MemWrite);

    forward_unit->forward_idexe_rs0(forward_idexe_rs0);
    forward_unit->forward_idexe_rs1(forward_idexe_rs1);
    forward_unit->forward_idexe_rt0(forward_idexe_rt0);
    forward_unit->forward_idexe_rt1(forward_idexe_rt1);
    forward_unit->forward_idexe_rsActive(forward_idexe_rsActive);
    forward_unit->forward_idexe_rtActive(forward_idexe_rtActive);
    forward_unit->forward_ifid_rs0(forward_ifid_rs0);
    forward_unit->forward_ifid_rs1(forward_ifid_rs1);
    forward_unit->forward_ifid_rt0(forward_ifid_rt0);
    forward_unit->forward_ifid_rt1(forward_ifid_rt1);
    forward_unit->forward_ifid_rsActive(forward_ifid_rsActive);
    forward_unit->forward_ifid_rtActive(forward_ifid_rtActive);
    forward_unit->forward_exemem1_regb0(forward_exemem1_regb0);
    forward_unit->forward_exemem1_regb1(forward_exemem1_regb1);

	hazard_unit = new hazard("hazard_unit");
	hazard_unit->rs(rs);
	hazard_unit->rt(rt);
	hazard_unit->BranchTaken(BranchTaken);
	hazard_unit->Jump(Jump);

	hazard_unit->WriteReg_exe(WriteReg_exe);
	hazard_unit->RegWrite_exe(RegWrite_exe);
	hazard_unit->WriteReg_mem1(WriteReg_mem1);
	hazard_unit->RegWrite_mem1(RegWrite_mem1);
	hazard_unit->WriteReg_mem2(WriteReg_mem2);
	hazard_unit->RegWrite_mem2(RegWrite_mem2);

	hazard_unit->enable_pc(enable_pc);
	hazard_unit->enable_ifid(enable_ifid);
	hazard_unit->reset_ifid(reset_haz_ifid);
	hazard_unit->reset_idexe(reset_haz_idexe);
}

mips::~mips(void)
{
	delete PCreg;
	delete instmem;
	delete add4;
	delete addbr;
	delete a1;
	delete mPC;
	delete xor1;
	delete dec1;
	delete mr;
	delete rfile;
	delete e1;
	delete sl2;
	delete mrt;
	delete mrs;
	delete comp;
	delete m1;
	delete alu1;
	delete datamem;
	delete forward_unit;
	delete m2;
	delete ctrl;
	delete hazard_unit;
	delete or_reset_idexe;
	delete or_reset_ifid;
	delete reg_if_id;
	delete reg_id_exe;
	delete reg_exe_mem1;
	delete reg_mem1_mem2;
	delete reg_mem2_wb;
}
