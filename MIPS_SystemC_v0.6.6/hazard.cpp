
#include "hazard.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void hazard::detect_hazard()
{
	//data hazards
	if( rs.read() !=0 && rs.read() == WriteReg_exe.read() && RegWrite_exe.read()==true
	    || rs.read() !=0 && rs.read() == WriteReg_mem1.read() && RegWrite_mem1.read()==true
	    || rt.read() !=0 && rt.read() == WriteReg_exe.read() && RegWrite_exe.read()==true
	    || rt.read() !=0 && rt.read() == WriteReg_mem1.read() && RegWrite_mem1.read()==true ) {

		enable_pc.write(false);
		enable_ifid.write(false);
		reset_idexe.write(true);
		reset_ifid.write(false);
	} else if (BranchTaken.read() || Jump.read()) {
		enable_pc.write(true);
		enable_ifid.write(true);
		reset_idexe.write(false);
		reset_ifid.write(true);
	}
	else {
	    enable_pc.write(true);
		enable_ifid.write(true);
		reset_idexe.write(false);
		reset_ifid.write(false);
	}
}

