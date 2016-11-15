
#include "jaddrdecode.h"

/**
 * \c shifl2 module callback function.
 */
void jaddrdecode::entry()
{
	if (JumpOnRegister.read()) {
		out.write(data1.read());
	} else {
		out.write(PC_id.read().range(31, 28) | (inst.read().range(25, 0) << 2));
	}
}
