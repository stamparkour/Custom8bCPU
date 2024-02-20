#include "myCPU.h"

bool CPU::interp(Reg12& addr, byte& bus) {
	bool rw = false;
	if (code == OP_NOP) {
		addr = IP = IP + 1;
		code = bus;
		index = 255;
	}

	switch (code << 4 | index) {
	case((OP_JMP << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_JMP << 4) | 1): {
		addr = IP = (bus << 8) | buffer;
		code = OP_NOP;
	} break;
	case((OP_BCS << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_BCS << 4) | 1): {
		if (carry) addr = IP = (bus << 8) | buffer;
		else addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_BZS << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_BZS << 4) | 1): {
		if (zero) addr = IP = (bus << 8) | buffer;
		else addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_BCC << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_BCC << 4) | 1): {
		if (!carry) addr = IP = (bus << 8) | buffer;
		else addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_BZC << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_BZC << 4) | 1): {
		if (!zero) addr = IP = (bus << 8) | buffer;
		else addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_BG << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_BG << 4) | 1): {
		if (!zero && !carry) addr = IP = (bus << 8) | buffer;
		else addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_BLE << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_BLE << 4) | 1): {
		if (zero || carry) addr = IP = (bus << 8) | buffer;
		else addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_STX_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_STX_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_STX_c << 4) | 2): {
		X = bus;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_STA_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_STA_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_STA_c << 4) | 2): {
		A = bus;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_LDX_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_LDX_c << 4) | 1): {
		addr = (bus << 8) | buffer;
		rw = true;
		bus = X;
	} break;
	case((OP_LDX_c << 4) | 2): {
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_LDA_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_LDA_c << 4) | 1): {
		addr = (bus << 8) | buffer;
		rw = true;
		bus = A;
	} break;
	case((OP_LDA_c << 4) | 2): {
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_STX_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_STX_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_STX_x << 4) | 2): {
		X = bus;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_STA_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_STA_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_STA_x << 4) | 2): {
		A = bus;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_LDX_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_LDX_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
		rw = true;
		bus = X;
	} break;
	case((OP_LDX_x << 4) | 2): {
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_LDA_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_LDA_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
		rw = true;
		bus = A;
	} break;
	case((OP_LDA_x << 4) | 2): {
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_INCX << 4) | 2): {
		X = X + 1;
		addr = IP = IP + 1;
		code = bus;
		index = 255;
	} break;
	case((OP_INCA << 4) | 2): {
		A = A + 1;
		addr = IP = IP + 1;
		code = bus;
		index = 255;
	} break;
	case((OP_DECX << 4) | 2): {
		X = X - 1;
		addr = IP = IP + 1;
		code = bus;
		index = 255;
	} break;
	case((OP_DECA << 4) | 2): {
		A = A - 1;
		addr = IP = IP + 1;
		code = bus;
		index = 255;
	} break;
	case((OP_ADD_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_ADD_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_ADD_c << 4) | 2): {
		int v = A + bus;
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SUB_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SUB_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_SUB_c << 4) | 2): {
		int v = A - bus;
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_ADDc_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_ADDc_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_ADDc_c << 4) | 2): {
		int v = A + bus + (carry ? 1 : 0);
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SUBc_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SUBc_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_SUBc_c << 4) | 2): {
		int v = A - bus - (carry ? 1 : 0);
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_XOR_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_XOR_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_XOR_c << 4) | 2): {
		int v = A ^ bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_AND_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_AND_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_AND_c << 4) | 2): {
		int v = A & bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_OR_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_OR_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_OR_c << 4) | 2): {
		int v = A | bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SHL_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SHL_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_SHL_c << 4) | 2): {
		int v = A << bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SHR_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SHR_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_SHR_c << 4) | 2): {
		int v = A >> bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_CMP_c << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_CMP_c << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_CMP_c << 4) | 2): {
		int v = A - bus;
		carry = v & 0x100;
		zero = !v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;

	case((OP_ADD_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_ADD_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_ADD_x << 4) | 2): {
		int v = A + bus;
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SUB_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SUB_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_SUB_x << 4) | 2): {
		int v = A - bus;
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_ADDc_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_ADDc_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_ADDc_x << 4) | 2): {
		int v = A + bus + (carry ? 1 : 0);
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SUBc_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SUBc_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_SUBc_x << 4) | 2): {
		int v = A - bus - (carry ? 1 : 0);
		carry = v & 0x100;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_XOR_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_XOR_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_XOR_x << 4) | 2): {
		int v = A ^ bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_AND_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_AND_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_AND_x << 4) | 2): {
		int v = A & bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_OR_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_OR_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_OR_x << 4) | 2): {
		int v = A | bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SHL_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SHL_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_SHL_x << 4) | 2): {
		int v = A << bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_SHR_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_SHR_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_SHR_x << 4) | 2): {
		int v = A >> bus;
		carry = false;
		zero = !v;
		A = v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_CMP_x << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_CMP_x << 4) | 1): {
		addr = ((bus << 8) | buffer) + X;
	} break;
	case((OP_CMP_x << 4) | 2): {
		int v = A - bus;
		carry = v & 0x100;
		zero = !v;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	case((OP_PSHA << 4) | 0): {
		addr = SP | 0xFF00;
		SP = SP - 1;
		bus = A;
		rw = true;
	} break;
	case((OP_PSHA << 4) | 1): {
		addr = IP;
		code = OP_NOP;
	} break;
	case((OP_PSHX << 4) | 0): {
		addr = SP | 0xFF00;
		SP = SP - 1;
		bus = X;
		rw = true;
	} break;
	case((OP_PSHX << 4) | 1): {
		addr = IP;
		code = OP_NOP;
	} break;
	case((OP_POPA << 4) | 0): {
		SP = SP + 1;
		addr = SP | 0xFF00;
	} break;
	case((OP_POPA << 4) | 1): {
		A = bus;
		addr = IP;
		code = OP_NOP;
	} break;
	case((OP_POPX << 4) | 0): {
		SP = SP + 1;
		addr = SP | 0xFF00;
	} break;
	case((OP_POPX << 4) | 1): {
		X = bus;
		addr = IP;
		code = OP_NOP;
	} break;
	case((OP_CAL << 4) | 0): {
		addr = IP = IP + 1;
		buffer = bus;
		rw = true;
	} break;
	case((OP_CAL << 4) | 1): {
		IP = (bus << 8) | buffer;
		addr = SP | 0xFF00;
		SP = SP - 1;
		bus = IP >> 8;
		rw = true;
	} break;
	case((OP_CAL << 4) | 2): {
		addr = SP | 0xFF00;
		SP = SP - 1;
		bus = IP;
		rw = true;
	} break;
	case((OP_CAL << 4) | 3): {
		addr = IP;
		code = OP_NOP;
	} break;
	case((OP_RET << 4) | 0): {
		SP = SP + 1;
		addr = SP | 0xFF00;
	} break;
	case((OP_RET << 4) | 1): {
		SP = SP + 1;
		addr = SP | 0xFF00;
		buffer = bus;
	} break;
	case((OP_RET << 4) | 2): {
		addr = IP = (bus << 8) | buffer;
		code = OP_NOP;
	} break;
	case((OP_STSP << 4) | 0): {
		buffer = bus;
		addr = IP = IP + 1;
	} break;
	case((OP_STSP << 4) | 1): {
		addr = (bus << 8) | buffer;
	} break;
	case((OP_STSP << 4) | 2): {
		SP = bus;
		addr = IP = IP + 1;
		code = OP_NOP;
	} break;
	}

	index = index + 1;
	return rw;
}