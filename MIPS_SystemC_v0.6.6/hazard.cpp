
#include "hazard.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void hazard::detect_hazard()
{
	/* 			NORMAL 		BUBBLE 		STALL
		ENABLE 	  1			   1		  0
		RESET     0            1          0       */


    // STALL MEM1 / BUBBLE ON MEM2
    if (true) {
	enable_pc.write(false);
        enable_ifid.write(false);
        reset_ifid.write(false);
        reset_idexe.write(false);
        enable_idexe.write(false);
        reset_exemem1.write(false);
        enable_exemem1.write(false);
        reset_mem1mem2.write(true);
    }
    // STALL EXE / BUBBLE ON MEM1
    else if (true) {
	enable_pc.write(false);
        enable_ifid.write(false);
        reset_ifid.write(false);
        reset_idexe.write(false);
        enable_idexe.write(false);
        reset_exemem1.write(true);
        enable_exemem1.write(true);
        reset_mem1mem2.write(false);
    }
    // STALL ID / BUBBLE ON EXE
    else if (  (rs.read() != 0 && rs.read() == WriteReg_exe.read() && Branch.read() && RegWrite_exe.read())
            || (rt.read() != 0 && rt.read() == WriteReg_exe.read() && Branch.read() && RegWrite_exe.read())
            || (rs.read() != 0 && rs.read() == WriteReg_mem1.read() && MemRead_mem1.read() && RegWrite_mem1.read() && Branch.read())
            || (rs.read() != 0 && rs.read() == WriteReg_mem2.read() && MemRead_mem2.read() && RegWrite_mem2.read() && Branch.read()))
    {
        enable_pc.write(false);
        enable_ifid.write(false);
        reset_ifid.write(false);
        reset_idexe.write(true);
        enable_idexe.write(true);
        reset_exemem1.write(false);
        enable_exemem1.write(true);
        reset_mem1mem2.write(false);
    }
    else if (BranchTaken.read() || Jump.read())
    {	// Discards instruction going to IF/ID
        enable_pc.write(true);
        enable_ifid.write(true);
        reset_idexe.write(false);
        reset_ifid.write(true);
        enable_idexe.write(true);
        reset_exemem1.write(false);
        enable_exemem1.write(true);
        reset_mem1mem2.write(false);
    }
    else
    {
        enable_pc.write(true);
        enable_ifid.write(true);
        reset_idexe.write(false);
        reset_ifid.write(false);
        enable_idexe.write(true);
        reset_exemem1.write(false);
        enable_exemem1.write(true);
        reset_mem1mem2.write(false);
    }
}

