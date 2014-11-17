//MIPSarch.cpp
//
// Widget to display MIPS architecture
//

#include <qpixmap.h>
#include <qevent.h>
#include <qcursor.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <QMouseEvent>

#include "PortValItem.h"
#include "PortValRead.h"

#include "MIPSarch.h"
#include "../mips.h"
#include "../mipsaux.h"

/**
 * MIPSarchcanvas constructor.
 */

MIPSarchCanvas::MIPSarchCanvas(mips &m, QObject *parent) : Q3Canvas(parent), mips1(m)
{
    // set background to MipsArch.png
    QPixmap backFig("GUI/MIPS_datapathPipe4_memoryResized.png");
    resize(backFig.width(),backFig.height());
    setBackgroundPixmap(backFig);

    // create and position the items that display the port values
    PortValItem *portVal;
    Q3CanvasText *qText;

    // instruction labels in stages
    qText=new Q3CanvasText("nop",this);
    qText->setTextFlags(Qt::AlignLeft | Qt::AlignTop);
    qText->setVisible(true);
    qText->move(10,6);
    qText->setColor(QColor("red"));
    instStage.push_back(qText);
    pcStage.push_back(new PortValRead(mips1.PCreg->dout,"PC"));
    validpcStage.push_back(new PortValRead(mips1.reg_if_id->valid_if,"valid_PC"));

    qText=new Q3CanvasText("nop",this);
    qText->setTextFlags(Qt::AlignLeft | Qt::AlignTop);
    qText->setVisible(true);
    qText->move(310,6);
    qText->setColor(QColor("red"));
    instStage.push_back(qText);
    pcStage.push_back(new PortValRead(mips1.reg_if_id->PC_id,"PC_id"));
    validpcStage.push_back(new PortValRead(mips1.reg_if_id->valid_id,"valid_PC_id"));

    qText=new Q3CanvasText("nop",this);
    qText->setTextFlags(Qt::AlignLeft | Qt::AlignTop);
    qText->setVisible(true);
    qText->move(470,6);
    qText->setColor(QColor("red"));
    instStage.push_back(qText);
    pcStage.push_back(new PortValRead(mips1.reg_id_exe->PC_exe,"PC_exe"));
    validpcStage.push_back(new PortValRead(mips1.reg_id_exe->valid_exe,"valid_PC_exe"));

    qText=new Q3CanvasText("nop",this);
    qText->setTextFlags(Qt::AlignLeft | Qt::AlignTop);
    qText->setVisible(true);
    qText->move(580,6);
    qText->setColor(QColor("red"));
    instStage.push_back(qText);
    pcStage.push_back(new PortValRead(mips1.reg_exe_mem1->PC_mem1,"PC_mem1"));
    validpcStage.push_back(new PortValRead(mips1.reg_exe_mem1->valid_mem1,"valid_PC_mem1"));

    qText=new Q3CanvasText("nop",this);
    qText->setTextFlags(Qt::AlignLeft | Qt::AlignTop);
    qText->setVisible(true);
    qText->move(690,6);
    qText->setColor(QColor("red"));
    instStage.push_back(qText);
    pcStage.push_back(new PortValRead(mips1.reg_mem1_mem2->PC_mem2,"PC_mem2"));
    validpcStage.push_back(new PortValRead(mips1.reg_mem1_mem2->valid_mem2,"valid_PC_mem2"));

    qText=new Q3CanvasText("nop",this);
    qText->setTextFlags(Qt::AlignLeft | Qt::AlignTop);
    qText->setVisible(true);
    qText->move(775,6);
    qText->setColor(QColor("red"));
    instStage.push_back(qText);
    pcStage.push_back(new PortValRead(mips1.reg_mem2_wb->PC_wb,"PC_wb"));
    validpcStage.push_back(new PortValRead(mips1.reg_mem2_wb->valid_wb,"valid_PC_wb"));


    /* Debug */
    portVal=new PortValItem(this,mips1.forward_unit->forward_ifid_rs0, "rs0_id");
    portVal->move(10,16);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.forward_unit->forward_ifid_rs1, "rs1_id");
    portVal->move(10,26);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.forward_unit->forward_ifid_rt0, "rt0_id");
    portVal->move(10,46);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.forward_unit->forward_ifid_rt1, "rt1_id");
    portVal->move(10,56);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    // value of port signals
    // IF
    portVal=new PortValItem(this,mips1.instmem->addr, "PC");
    portVal->move(71,270);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.add4->res, "PC4");
    portVal->move(102,132);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.PCreg->din, "NPC");
    portVal->move(11,270);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.instmem->inst, "inst");
    portVal->move(143,287);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    //ID
    portVal=new PortValItem(this,mips1.rfile->data1, "RegA");
    portVal->move(399,37);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rfile->data2, "RegB");
    portVal->move(399,45);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.e1->dout, "imm_ext2");
    portVal->move(414,27);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_if_id->PC_id, "PC4_id");
    portVal->move(399,16);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);



    portVal=new PortValItem(this,mips1.a1->dout, "BranchTaken");
    portVal->move(258,18);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->Jump, "Jump");
    portVal->move(258,5);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.addbr->res, "BranchTarget");
    portVal->move(258,32);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.jAddrDecode->out, "JumpAddr");
    portVal->move(174,59);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->RegWrite, "RegWrite");
    portVal->move(352,93);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->MemtoReg, "MemtoReg");
    portVal->move(352,105);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->Branch, "Branch");
    portVal->move(348,76);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->MemRead, "MemRead");
    portVal->move(352,128);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->MemWrite, "MemWrite");
    portVal->move(352,139);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->ALUSrc, "ALUSrc");
    portVal->move(352,150);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->ALUOp, "ALUOp");
    portVal->move(352,160);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->RegDst, "RegDst");
    portVal->move(352,172);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->Jump, "Jump");
    portVal->move(406,229);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->BranchNotEqual, "BranchNotEqual");
    portVal->move(316,81);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_if_id->inst_id, "inst_id");
    portVal->move(201,287);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.ctrl->JumpOnRegister, "JumpOnRegister");
    portVal->move(268,105);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.dec1->opcode, "opcode");
    portVal->move(268,137);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.dec1->funct, "funct");
    portVal->move(268,170);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);


    portVal=new PortValItem(this,mips1.dec1->rd, "rd");
    portVal->move(350,361);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.dec1->imm, "imm");
    portVal->move(272,402);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rfile->reg1, "r1");
    portVal->move(312,252);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rfile->reg2, "r2");
    portVal->move(312,269);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rsFwd_id->dout, "r1_out");
    portVal->move(372,250);
    portVal->setColor(QColor("red"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rtFwd_id->dout, "r2_out");
    portVal->move(372,292);
    portVal->setColor(QColor("red"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.e1->dout, "imm_ext");
    portVal->move(376,403);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.mr->dout, "muxRDst");
    portVal->move(397,369);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);


    // Hazard unit
    portVal=new PortValItem(this,mips1.hazard_unit->enable_pc, "enable_pc");
    portVal->move(58,470);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->reset_idexe, "reset_haz_idexe");
    portVal->move(412,472);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->enable_ifid, "enable_ifid");
    portVal->move(207,429);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->reset_ifid, "reset_haz_ifid");
    portVal->move(160,427);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->rs, "rs");
    portVal->move(280,453);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->rt, "rt");
    portVal->move(280,462);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->BranchTaken, "BranchTaken");
    portVal->move(330,470);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->Jump, "Jump");
    portVal->move(297,479);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->WriteReg_exe, "WriteReg_exe");
    portVal->move(330,503);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->RegWrite_exe, "RegWrite_exe");
    portVal->move(330,516);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->WriteReg_mem1, "WriteReg_mem1");
    portVal->move(330,530);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->RegWrite_mem1, "RegWrite_mem1");
    portVal->move(130,503);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->WriteReg_mem2, "WriteReg_mem2");
    portVal->move(130,516);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.hazard_unit->RegWrite_mem2, "RegWrite_mem2");
    portVal->move(130,530);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    //EXE
    portVal=new PortValItem(this,mips1.reg_id_exe->RegWrite_exe, "RegWrite_exe");
    portVal->move(456,93);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_id_exe->MemtoReg_exe, "MemtoReg_exe");
    portVal->move(456,105);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_id_exe->MemRead_exe, "MemRead_exe");
    portVal->move(456,128);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_id_exe->MemWrite_exe, "MemWrite_exe");
    portVal->move(456,139);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_id_exe->ALUSrc_exe, "ALUSrc_exe");
    portVal->move(456,150);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_id_exe->ALUOp_exe, "ALUOp_exe");
    portVal->move(456,160);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    //portVal=new PortValItem(this,mips1.reg_id_exe->rega_exe, "rega_exe");
    portVal=new PortValItem(this,mips1.rsActive_exe->dout, "rega_exe");
    portVal->move(439,250);
    portVal->setColor(QColor("red"));
    portValVec.push_back(portVal);

    //portVal=new PortValItem(this,mips1.reg_id_exe->regb_exe, "regb_exe");
    portVal=new PortValItem(this,mips1.rtActive_exe->dout, "regb_exe");
    portVal->move(439,292);
    portVal->setColor(QColor("red"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.alu1->din2, "alu_din2");
    portVal->move(497,311);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_id_exe->WriteReg_exe, "WriteReg_exe");
    portVal->move(467,369);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.alu1->dout, "alu_dout");
    portVal->move(557,270);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    //MEM1
    portVal=new PortValItem(this,mips1.reg_exe_mem1->RegWrite_mem1, "RegWrite_mem1");
    portVal->move(625,93);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_exe_mem1->MemtoReg_mem1, "MemtoReg_mem1");
    portVal->move(625,105);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_exe_mem1->MemRead_mem1, "MemRead_mem");
    portVal->move(625,128);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_exe_mem1->MemWrite_mem1, "MemWrite_mem");
    portVal->move(625,139);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_exe_mem1->aluOut_mem1, "aluOut_mem1");
    portVal->move(605,282);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rddFwd_mem1->dout, "regb_mem1");
    portVal->move(615,320);
    portVal->setColor(QColor("red"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_exe_mem1->WriteReg_mem1, "WriteReg_mem1");
    portVal->move(621,383);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    //MEM2
    portVal=new PortValItem(this,mips1.reg_mem1_mem2->RegWrite_mem2, "RegWrite_mem2");
    portVal->move(726,93);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem1_mem2->MemtoReg_mem2, "MemtoReg_mem2");
    portVal->move(726,105);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.datamem->dout, "dmem.dout");
    portVal->move(726,324);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem1_mem2->aluOut_mem2, "aluOut_mem2");
    portVal->move(726,351);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem1_mem2->WriteReg_mem2, "WriteReg_mem2");
    portVal->move(726,383);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    //WB

    portVal=new PortValItem(this,mips1.reg_mem2_wb->memOut_wb, "memOut_wb");
    portVal->move(776,324);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem2_wb->aluOut_wb, "aluOut_wb");
    portVal->move(776,351);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem2_wb->MemtoReg_wb, "MemtoReg_wb");
    portVal->move(831,310);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem2_wb->WriteReg_wb, "WriteReg_wb");
    portVal->move(562,431);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.reg_mem2_wb->RegWrite_wb, "RegWrite_wb");
    portVal->move(562,413);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);

    portVal=new PortValItem(this,mips1.rfile->datawr, "WriteVal");
    portVal->move(562,442);
    portVal->setColor(QColor("blue"));
    portValVec.push_back(portVal);



    updateArch();

    setDoubleBuffering(true);
}

/**
 * updates the values of all ports values.
 */
void MIPSarchCanvas::updateArch(void)
{
    unsigned int p,s,addr;
    char instText[200];
    for(p=0; p<portValVec.size(); p++) {
         portValVec[p]->updateVal();
    }
    for(s=0; s<instStage.size(); s++) {
	 if(validpcStage[s]->read() == 1) { // Stage PC is valid
	     if(pcStage[s]->read() < mips1.instmem->size()) { // StagePC is less than size of instmem
	        addr=mips1.instmem->at(pcStage[s]->read());
	        disassemble(addr,instText);
                instStage[s]->setText(instText);
	     }
	     else instStage[s]->setText("nop");
	 }
	 else instStage[s]->setText("bubble");
    }
    update();
}

MIPSarchCanvas::~MIPSarchCanvas()
{
    // no need to delete child widgets, Qt does it all for us
    for(unsigned int p=0; p < portValVec.size(); p++) {
       delete portValVec[p];
       portValVec[p]=0;
    }
}


/**
 * MIPSarch contructor.
 * Creates and sets the MIPSarchcanvas to be displayed, and some
 * GUI functionalities.
 */
MIPSarch::MIPSarch( mips &m, QWidget* parent,  const char* name, Qt::WFlags fl )
    : Q3CanvasView(0, parent, name, fl )
{
    archCanvas=new MIPSarchCanvas(m,this);
    setCanvas(archCanvas);

    setIcon(QPixmap("mips.xpm"));

    resize(QSize(855+5,545+5));
    setMaximumSize(QSize(855+5,545+5));

    cursor=new QCursor(Qt::PointingHandCursor);
    setCursor(*cursor);

    viewport()->setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

/*
 *  Destroys the object and frees any allocated resources
 */
MIPSarch::~MIPSarch()
{
    // no need to delete child widgets, Qt does it all for us
    delete cursor;
}

/**
 * updates the port values.
 */
void MIPSarch::updateArch(void)
{
     archCanvas->updateArch();
}

/**
 * emits signals when click is inside certain regions
 */
void MIPSarch::contentsMousePressEvent(QMouseEvent *e)
{
/*

    QPoint pos=e->pos();
    if(pos.x() > 80 && pos.x() < 112
       && pos.y()> 200 && pos.y() < 270)
       emit imemClicked();
    if(pos.x() > 320 && pos.x() < 370
       && pos.y()> 190 && pos.y() < 300)
       emit regfileClicked();
    if(pos.x() > 590 && pos.x() < 645
       && pos.y()> 220 && pos.y() < 280)
       emit dmemClicked();
*/
}

/**
 * changes cursor when mouse is over certain regions
 */

void MIPSarch::contentsMouseMoveEvent(QMouseEvent *e)
{
    QPoint pos=e->pos();

    //fprintf(stderr,"x=%4d y=%4d\n",pos.x(),pos.y());

    cursor->setShape(Qt::ArrowCursor);
    setCursor(*cursor);

}

/**
 * This window is not to be closed.
 */
void MIPSarch::closeEvent(QCloseEvent *e)
{
   e->ignore();
}
