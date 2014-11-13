
#include "forward.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void forward::detect_forward()
{
    /*
      > FORWARD_ID_EXE RS/RT          > FORWARD_EXE_MEM1 RDD
        0 - AluOut                      0 - Normal
        1 - AluOut_mem1                 1 - AluOut_mem1
        2 - AluOut_mem1                 2 - MemOut_mem2
        3 - MemOut_mem2                 3 - WriteVal
        Flag - Active or not
    */
    /* FORWARD PARA O RS DO REGISTO ID_EXE */
    printf("----\n");

    forward_idexe_rsActive.write(1);
    if (rs.read() != 0 && rs.read() == WriteReg_exe.read() && RegWrite_exe.read() == 1 && MemRead_exe.read() == 0) {
        printf("EXE -> ID/EXE (RS) (AluOut)\n");
        forward_idexe_rs0.write(0);
        forward_idexe_rs1.write(0);
    } else if (rs.read() != 0 && rs.read() == WriteReg_mem1.read() && RegWrite_mem1.read() == 1 && MemRead_mem1.read() == 0) {
        printf("MEM1 -> ID/EXE (RS) (AluOut)\n");
        forward_idexe_rs0.write(1);
        forward_idexe_rs1.write(0);
    } else if (rs.read() != 0 && rs.read() == WriteReg_mem2.read() && RegWrite_mem2.read() == 1 && MemRead_mem2.read() == 0) {
        printf("MEM2 -> ID/EXE (RS) (AluOut)\n");
        forward_idexe_rs0.write(0);
        forward_idexe_rs1.write(1);
    } else if (rs.read() != 0 && rs.read() == WriteReg_mem2.read() && RegWrite_mem2.read() == 1 && MemRead_mem2.read() == 1) {
        printf("MEM2 -> ID/EXE (RS) (MemOut)\n");
        forward_idexe_rs0.write(1);
        forward_idexe_rs1.write(1);
    } else {
        forward_idexe_rs0.write(0);
        forward_idexe_rs1.write(0);
        forward_idexe_rsActive.write(0);
    }

    /* FORWARD PARA O RT DO REGISTO ID_EXE */
    forward_idexe_rtActive.write(1);
    if (rt.read() != 0 && rt.read() == WriteReg_exe.read() &&
                    MemWrite.read() == false && RegWrite_exe.read() == 1 && MemRead_exe.read() == 0) {
        printf("EXE -> ID/EXE (RT) (AluOut)\n");
        forward_idexe_rt0.write(0);
        forward_idexe_rt1.write(0);
    } else if (rt.read() != 0 && rt.read() == WriteReg_mem1.read() &&
                    MemWrite.read() == 0 && RegWrite_mem1.read() == 1 && MemRead_mem1.read() == 0) {
        printf("MEM1 -> ID/EXE (RT) (AluOut)\n");
        forward_idexe_rt0.write(1);
        forward_idexe_rt1.write(0);
    } else if (rt.read() != 0 && rt.read() == WriteReg_mem2.read() &&
                    MemWrite.read() == 0 && RegWrite_mem2.read() == 1 && MemRead_mem2.read() == 0) {
        printf("MEM2 -> ID/EXE (RT) (AluOut)\n");
        forward_idexe_rt0.write(0);
        forward_idexe_rt1.write(1);
    } else if (rt.read() != 0 && rt.read() == WriteReg_mem2.read() &&
                    MemWrite.read() == 0 && RegWrite_mem2.read() == 1 && MemRead_mem2.read() == 1) {
        printf("MEM2 -> ID/EXE (RT) (MemOut)\n");
        forward_idexe_rt0.write(1);
        forward_idexe_rt1.write(1);
    } else {
        forward_idexe_rt0.write(0);
        forward_idexe_rt1.write(0);
        forward_idexe_rtActive.write(0);
    }

    /* FORWARD PARA O RDD DO REGISTO EXE_MEM1 */
    if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_mem1.read() && MemWrite_exe.read() == 1 && RegWrite_mem1.read()) {
        printf("MEM1 -> EXE/MEM1 (RDD) (AluOut)\n");
        forward_exemem1_regb0.write(1);
        forward_exemem1_regb1.write(0);
    } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_mem2.read() && MemWrite_exe.read() == 1 && RegWrite_mem2.read()) {
        printf("MEM2 -> EXE/MEM1 (RDD) (MemOut)\n");
        forward_exemem1_regb0.write(0);
        forward_exemem1_regb1.write(1);
    } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_wb.read()  && MemWrite_exe.read() == 1 && RegWrite_wb.read()) {
        printf("WB -> EXE/MEM1 (RDD)  (WriteVal)\n");
        forward_exemem1_regb0.write(1);
        forward_exemem1_regb1.write(1);
    } else {
        forward_exemem1_regb0.write(0);
        forward_exemem1_regb1.write(0);
    }
}
