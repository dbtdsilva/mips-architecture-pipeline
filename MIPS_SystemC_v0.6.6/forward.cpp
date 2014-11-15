
#include "forward.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void forward::detect_forward()
{
    /*
      > FORWARD_ID_EXE RS/RT          > FORWARD_EXE_MEM1 RDD        > FORWARD_IF_ID RS/RT
        0 - AluOut                      0 - Normal                      0 - AluOut
        1 - AluOut_mem1                 1 - AluOut_mem1 ???             1 - AluOut_mem1
        2 - AluOut_mem1                 2 - MemOut_mem2                 2 - AluOut_mem2
        3 - MemOut_mem2                 3 - WriteVal                    3 - MemOut_mem2
        Flag(ACTIVE/NOT)                                                Flag (ACTIVE/NOT)
    */
    /* FORWARD PARA O RS DO REGISTO ID_EXE */
    printf("----\n");

    forward_idexe_rsActive.write(1);
    forward_ifid_rs0.write(0);
    forward_ifid_rs1.write(0);
    forward_ifid_rt0.write(0);
    forward_ifid_rt1.write(0);
    forward_ifid_rtActive.write(0);
    forward_ifid_rsActive.write(0);

    if (rs.read() != 0 && rs.read() == WriteReg_exe.read()) {
        printf("EXE -> ID/EXE (RS) (AluOut)\n");
        forward_idexe_rs0.write(0);
        forward_idexe_rs1.write(0);
    } else if (rs.read() != 0 && rs.read() == WriteReg_mem1.read()) {
        printf("MEM1 -> ID/EXE (RS) (AluOut)\n");
        forward_idexe_rs0.write(1);
        forward_idexe_rs1.write(0);
    } else if (rs.read() != 0 && rs.read() == WriteReg_mem2.read()) {
        printf("MEM2 -> ID/EXE (RS) (AluOut)\n");
        forward_idexe_rs0.write(0);
        forward_idexe_rs1.write(1);
    } else if (rs.read() != 0 && rs.read() == WriteReg_mem2.read()) {
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
    if (rt.read() != 0 && rt.read() == WriteReg_exe.read()) {
        //printf("EXE -> ID/EXE (RT) (AluOut)\n");
        forward_idexe_rt0.write(0);
        forward_idexe_rt1.write(0);
    } else if (rt.read() != 0 && rt.read() == WriteReg_mem1.read()) {
        //printf("MEM1 -> ID/EXE (RT) (AluOut)\n");
        forward_idexe_rt0.write(1);
        forward_idexe_rt1.write(0);
    } else if (rt.read() != 0 && rt.read() == WriteReg_mem2.read()) {
        //printf("MEM2 -> ID/EXE (RT) (AluOut)\n");
        forward_idexe_rt0.write(0);
        forward_idexe_rt1.write(1);
    } else if (rt.read() != 0 && rt.read() == WriteReg_mem2.read()) {
        //printf("MEM2 -> ID/EXE (RT) (MemOut)\n");
        forward_idexe_rt0.write(1);
        forward_idexe_rt1.write(1);
    } else {
        forward_idexe_rt0.write(0);
        forward_idexe_rt1.write(0);
        forward_idexe_rtActive.write(0);
    }

    /* FORWARD PARA O RDD DO REGISTO EXE_MEM1 */
    if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_mem1.read()) {
        //printf("MEM1 -> EXE/MEM1 (RDD) (AluOut) ????? \n");
        forward_exemem1_regb0.write(1);
        forward_exemem1_regb1.write(0);
    } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_mem2.read()) {
        //printf("MEM2 -> EXE/MEM1 (RDD) (MemOut)\n");
        forward_exemem1_regb0.write(0);
        forward_exemem1_regb1.write(1);
    } else if (rt_exe.read() != 0 && rt_exe.read() == WriteReg_wb.read()) {
        //printf("WB -> EXE/MEM1 (RDD)  (WriteVal)\n");
        forward_exemem1_regb0.write(1);
        forward_exemem1_regb1.write(1);
    } else {
        forward_exemem1_regb0.write(0);
        forward_exemem1_regb1.write(0);
    }
}
