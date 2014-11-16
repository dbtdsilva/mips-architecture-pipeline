
#include "forward.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void forward::detect_forward()
{
    /*
      > FORWARD_ID_EXE RS/RT          > FORWARD_EXE_MEM1 RDD        > FORWARD_IF_ID RS/RT
        0 - AluOut                      0 - Normal                      0 - AluOut
        1 - AluOut_mem1                 1 - MemOut                      1 - AluOut_mem1
        2 - AluOut_mem1                 2 - WriteVal                    2 - AluOut_mem2
        3 - MemOut                      3 - WriteVal                    3 - MemOut
        Flag(ACTIVE/NOT)                                                Flag (ACTIVE/NOT)
    */
    /* FORWARD PARA O RS DO REGISTO ID_EXE */
    printf("----\n");

    forward_ifid_rs0.write(0);
    forward_ifid_rs1.write(0);
    forward_ifid_rsActive.write(0);
    if (Branch.read() || JumpOnRegister.read()) {

        forward_ifid_rsActive.write(1);
        if (rs.read() != 0 && rs.read() == WriteReg_mem1.read() && !MemRead_mem1.read() && RegWrite_mem1.read()) {
            printf("EXE -> IF/ID (RS) (AluOut)\n");
            forward_ifid_rs0.write(0);
            forward_ifid_rs1.write(0);
        } else if (rs.read() != 0 && rs.read() == WriteReg_mem2.read() && !MemRead_mem2.read() && RegWrite_mem2.read()) {
            printf("MEM1 -> IF/ID (RS) (AluOut)\n");
            forward_ifid_rs0.write(1);
            forward_ifid_rs1.write(0);
        } else {
            printf("NO FORWARD TO IF/ID RS\n");
            forward_ifid_rsActive.write(0);
        }
    }

    forward_ifid_rt0.write(0);
    forward_ifid_rt1.write(0);
    forward_ifid_rtActive.write(0);
    if (Branch.read()) {
        forward_ifid_rtActive.write(1);
        if (rt.read() != 0 && rt.read() == WriteReg_mem1.read() && !MemRead_mem1.read()
                                                        && !MemRead.read() && RegWrite_mem1.read()) {
            printf("EXE -> IF/ID (RT) (AluOut)\n");
            forward_ifid_rt0.write(0);
            forward_ifid_rt1.write(0);
        } else if (rt.read() != 0 && rt.read() == WriteReg_mem2.read() && !MemRead_mem2.read()
                                                        && !MemRead.read() && RegWrite_mem2.read()) {
            printf("MEM1 -> IF/ID (RT) (AluOut)\n");
            forward_ifid_rt0.write(1);
            forward_ifid_rt1.write(0);
        } else {
            printf("NO FORWARD TO IF/ID RT\n");
            forward_ifid_rtActive.write(0);
        }
    }

    forward_idexe_rs0.write(0);
    forward_idexe_rs1.write(0);
    forward_idexe_rsActive.write(0);

    forward_idexe_rt0.write(0);
    forward_idexe_rt1.write(0);
    forward_idexe_rtActive.write(0);

    if (!Branch_exe.read() && !JumpOnRegister_exe.read()) {
        forward_idexe_rsActive.write(1);
        if (rs_exe.read() != 0 && rs_exe.read() == WriteReg_mem1.read() && !MemRead_mem1.read() && RegWrite_mem1.read()) {
            printf("EXE -> ID/EXE (RS) (AluOut)\n");
            forward_idexe_rs0.write(0);
            forward_idexe_rs1.write(0);
        } else if (rs_exe.read() != 0 && rs_exe.read() == WriteReg_mem2.read() && !MemRead_mem2.read() && RegWrite_mem2.read()) {
            printf("MEM1 -> ID/EXE (RS) (AluOut)\n");
            forward_idexe_rs0.write(1);
            forward_idexe_rs1.write(0);
        } else if (rs_exe.read() != 0 && rs_exe.read() == WriteReg_wb.read() && !MemRead_wb.read() && RegWrite_wb.read()) {
            printf("MEM2 -> ID/EXE (RS) (AluOut)\n");
            forward_idexe_rs0.write(0);
            forward_idexe_rs1.write(1);
        } else if (rs_exe.read() != 0 && rs_exe.read() == WriteReg_wb.read() && MemRead_wb.read() && RegWrite_wb.read()) {
            printf("MEM2 -> ID/EXE (RS) (MemOut)\n");
            forward_idexe_rs0.write(1);
            forward_idexe_rs1.write(1);
        } else {
            printf("NO FORWARD TO ID/EXE RS\n");
            forward_idexe_rsActive.write(0);
        }

        /* FORWARD PARA O RT DO REGISTO ID_EXE */
        forward_idexe_rtActive.write(1);
        if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_mem1.read() && !MemRead_mem1.read()
                                                            && !MemRead_exe.read() && RegWrite_mem1.read()) {
            printf("EXE -> ID/EXE (RT) (AluOut)\n");
            forward_idexe_rt0.write(0);
            forward_idexe_rt1.write(0);
        } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_mem2.read() && !MemRead_mem2.read()
                                                            && !MemRead_exe.read() && RegWrite_mem2.read()) {
            printf("MEM1 -> ID/EXE (RT) (AluOut)\n");
            forward_idexe_rt0.write(1);
            forward_idexe_rt1.write(0);
        } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_wb.read() && !MemRead_wb.read()
                                                            && !MemRead_exe.read() && RegWrite_wb.read()) {
            printf("MEM2 -> ID/EXE (RT) (AluOut)\n");
            forward_idexe_rt0.write(0);
            forward_idexe_rt1.write(1);
        } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_wb.read() && MemRead_wb.read()
                                                            && !MemRead_exe.read() && RegWrite_wb.read()) {
            printf("MEM2 -> ID/EXE (RT) (MemOut)\n");
            forward_idexe_rt0.write(1);
            forward_idexe_rt1.write(1);
        } else {
            printf("NO FORWARD TO ID/EXE RT\n");
            forward_idexe_rtActive.write(0);
        }
    }



    /* FORWARD PARA O RDD DO REGISTO EXE_MEM1 */
    if(!Branch_mem1.read() && !JumpOnRegister_mem1.read()) {
        if (rt_mem1.read() != 0 && rt_mem1.read() == WriteReg_wb.read() && MemWrite_mem1.read()
                                                && RegWrite_wb.read() && MemRead_wb.read()) {
            printf("MEM2 -> EXE/MEM1 (RDD) (MemOut)\n");
            forward_exemem1_regb0.write(0);
            forward_exemem1_regb1.write(1);
        } else {
            printf("NO FORWARD TO EXE/MEM1 RDD\n");
            forward_exemem1_regb0.write(0);
            forward_exemem1_regb1.write(0);
        }
    }
}

