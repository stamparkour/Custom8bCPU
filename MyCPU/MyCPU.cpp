#include "myCPU.h"
#include <cstdint>
#include <iostream>

int BCDfix(uint8_t v) {
	int a = v & 0xF;
	int b = v >> 4;
	int c = 0;
	if (a > 9) {
		a -= 10;
		b++;
	}
	if (b > 9) {
		b -= 10;
		c++;
	}
	return a | (b << 4) | (c << 8);
}

int fromBCD(int v) {
	int a = v & 0xF;
	int b = (v >> 4) & 0xF;
	return a + b * 10;
}
int toBCD(int v) {
	int a = v % 10;
	int b = (v / 10) % 10;
	int c = (v / 100) % 10;
	return a | (b<<4) | (c<<8);
}

bool AddrMode(CPU& prog) {
	switch (prog.code >> 6) {
	case 0: {
		prog.IP = prog.IP + 1;
		return true;
	} case 1: switch (prog.index & 0x3) {
		case 0: {
			prog.buffer = prog.io;
			prog.addr = prog.IP = prog.IP + 1;
		} return false;
		case 1: {
			prog.addr = prog.buffer | (prog.io << 8);
			prog.IP = prog.IP + 1;
		} return true;
	} case 2: switch (prog.index & 0x3) {
		case 0: {
			prog.buffer = prog.io;
			prog.addr = prog.IP = prog.IP + 1;
		} return false;
		case 1: {
			prog.addr = (prog.buffer | (prog.io << 8)) + prog.X;
			prog.IP = prog.IP + 1;
		} return true;
	} case 3: switch (prog.index & 0x3) {
		case 0: {
			prog.addr = prog.X | (prog.Y << 8);
		} return true;
	}
	}
}

void JMP(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			prog.IP = prog.addr;
			prog.code = OP_NOP;
		}
	}
}
void BZS(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			if (prog.zero) {
				prog.IP = prog.addr;
			}
			else {
				prog.addr = prog.IP;
			}
			prog.code = OP_NOP;
		}
	}
}
void BZC(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			if (!prog.zero) {
				prog.IP = prog.addr;
			}
			else {
				prog.addr = prog.IP;
			}
			prog.code = OP_NOP;
		}
	}
}
void BCS(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			if (prog.carry) {
				prog.IP = prog.addr;
			}
			else {
				prog.addr = prog.IP;
			}
			prog.code = OP_NOP;
		}
	}
}
void BCC(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			if (!prog.carry) {
				prog.IP = prog.addr;
			}
			else {
				prog.addr = prog.IP;
			}
			prog.code = OP_NOP;
		}
	}
}
void BG(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			if (!prog.carry && !prog.zero) {
				prog.IP = prog.addr;
			}
			else {
				prog.addr = prog.IP;
			}
			prog.code = OP_NOP;
		}
	}
}
void BLE(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			if (prog.carry || prog.zero) {
				prog.IP = prog.addr;
			}
			else {
				prog.addr = prog.IP;
			}
			prog.code = OP_NOP;
		}
	}
}

void LDA(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		prog.A = prog.io;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void LDX(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		prog.X = prog.io;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void LDY(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		prog.Y = prog.io;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}

void STA(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			RW = true;
			prog.io = prog.A;
		}
	}
	else if (prog.index == 0x4) {
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void STX(CPU& prog, bool& RW) {
	if (prog.code >> 6 == 0) {
		prog.X = prog.Y;
		prog.code = OP_NOP;
	}
	else {
		if ((prog.index & 0xC) == 0) {
			if (AddrMode(prog)) {
				prog.index = 0x3;
				RW = true;
				prog.io = prog.X;
			}
		}
		else if (prog.index == 0x4) {
			prog.addr = prog.IP;
			prog.code = OP_NOP;
		}
	}
}
void STY(CPU& prog, bool& RW) {
	if (prog.code >> 6 == 0) {
		prog.Y = prog.X;
		prog.code = OP_NOP;
	}
	else {
		if ((prog.index & 0xC) == 0) {
			if (AddrMode(prog)) {
				prog.index = 0x3;
				RW = true;
				prog.io = prog.Y;
			}
		}
		else if (prog.index == 0x4) {
			prog.addr = prog.IP;
			prog.code = OP_NOP;
		}
	}
}

void INC(CPU& prog, bool& RW)  {
	if (prog.code >> 6 == 3) {
		prog.A = prog.Y;
	}
	else {
		int v = 0;
		switch (prog.code >> 6) {
		case 0:
			v = prog.A + 1;
			break;
		case 1:
			v = prog.X + 1;
			break;
		case 2:
			v = prog.Y + 1;
			break;
		}
		if (prog.bcd) {
			v = BCDfix(v);
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		switch (prog.code >> 6) {
		case 0:
 			prog.A = v;
			break;
		case 1:
			prog.X = v;
			break;
		case 2:
			prog.Y = v;
			break;
		}

	}
	prog.code = OP_NOP;
	
}

void DEC(CPU& prog, bool& RW) {
	if (prog.code >> 6 == 3) {
		prog.A = prog.X;
	}
	else {
		int v = 0;
		switch (prog.code >> 6) {
		case 0:
			v = prog.A - 1;
			break;
		case 1:
			v = prog.X - 1;
			break;
		case 2:
			v = prog.Y - 1;
			break;
		}
		if (prog.bcd) {
			v = BCDfix(v);
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		switch (prog.code >> 6) {
		case 0:
			prog.A = v;
			break;
		case 1:
			prog.X = v;
			break;
		case 2:
			prog.Y = v;
			break;
		}
	}

	prog.code = OP_NOP;
}
void ADD(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v;
		if (prog.bcd) {
			v = toBCD(fromBCD(prog.A) + fromBCD(prog.io));
		}
		else {
			v = prog.A + prog.io;
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void SUB(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v;
		if (prog.bcd) {
			v = toBCD(fromBCD(prog.A) + fromBCD(prog.io));
		}
		else {
			v = prog.A + prog.io;
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void AND(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v = prog.A & prog.io;
		prog.carry = false;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void OR(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v = prog.A | prog.io;
		prog.carry = false;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void XOR(CPU& prog, bool& RW) {
	if ((prog.index ^ 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v = prog.A & prog.io;
		prog.carry = false;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void SHL(CPU& prog, bool& RW) {
	if (prog.code >> 6 == 3) {
		prog.Y = prog.A;
	}
	else {
		int v = 0;
		switch (prog.code >> 6) {
		case 0:
			v = prog.A << 1;
			break;
		case 1:
			v = prog.X << 1;
			break;
		case 2:
			v = prog.Y << 1;
			break;
		}
		v |= prog.carry & prog.bcd;
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		switch (prog.code >> 6) {
		case 0:
			prog.A = v;
			break;
		case 1:
			prog.X = v;
			break;
		case 2:
			prog.Y = v;
			break;
		}
	}
	prog.code = OP_NOP;
}
void SHR(CPU& prog, bool& RW) {
	if (prog.code >> 6 == 3) {
		prog.X = prog.A;
	}
	else {
		int v = 0;
		switch (prog.code >> 6) {
		case 0:
			v = prog.A << 7;
			break;
		case 1:
			v = prog.X << 7;
			break;
		case 2:
			v = prog.Y << 7;
			break;
		}
		v |= (prog.carry & prog.bcd) ? 0x8000 : 0;
		prog.carry = (v & 0x80) != 0;
		v >>= 8;
		prog.zero = !v;
		switch (prog.code >> 6) {
		case 0:
			prog.A = v;
			break;
		case 1:
			prog.X = v;
			break;
		case 2:
			prog.Y = v;
			break;
		}
	}
	prog.code = OP_NOP;
	
}
void CMP(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v;
		if (prog.bcd) {
			v = toBCD(fromBCD(prog.A) + fromBCD(prog.io));
		}
		else {
			v = prog.A - prog.io;
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void ADDC(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v;
		if (prog.bcd) {
			v = toBCD(fromBCD(prog.A) + fromBCD(prog.io) + prog.carry);
		}
		else {
			v = prog.A + prog.io + prog.carry;
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void SUBC(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		int v;
		if (prog.bcd) {
			v = toBCD(fromBCD(prog.A) + fromBCD(prog.io) - prog.carry);
		}
		else {
			v = prog.A + prog.io - prog.carry;
		}
		prog.carry = (v & 0x100) != 0;
		prog.zero = !v;
		prog.A = v;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}

void PUSH(CPU& prog, bool& RW) {
	if (prog.code >> 6 == 3) {//int
		if (prog.index == 0) {
			prog.addr = prog.SP | 0xFF00;
			prog.SP = prog.SP - 1;
			RW = true;
			prog.io = (prog.IP) >> 8;
		}
		else if (prog.index == 1) {
			prog.addr = prog.SP | 0xFF00;
			prog.SP = prog.SP - 1;
			RW = true;
			prog.io = (prog.IP) & 0xFF;
		}
		else if (prog.index == 2) {
			prog.addr = prog.SP | 0xFF00;
			prog.SP = prog.SP - 1;
			RW = true;
			prog.io = prog.flags;
		}
		else if (prog.index == 3) {
			prog.addr = 0x7FFC;
		}
		else if (prog.index == 4) {
			prog.addr++;
			prog.buffer = prog.io;
		}
		else if (prog.index == 5) {
			prog.addr = prog.IP = prog.buffer | (prog.io << 8);
			prog.interruptEnable = 0;
			prog.code = OP_NOP;
		}
	}
	else {
		if (prog.index == 0) {
			int v = 0;

			switch (prog.code >> 6) {
			case(0): v = prog.A; break;
			case(1): v = prog.X; break;
			case(2): v = prog.Y; break;
			}
			prog.addr = prog.SP | 0xFF00;
			prog.SP = prog.SP - 1;
			RW = true;
			prog.io = v;
		}
		else if (prog.index == 1) {
			prog.addr = prog.IP;
			prog.code = OP_NOP;
		}
	}
}
void POP(CPU& prog, bool& RW) {
	int v = 0;

	if ((prog.code >> 6) == 3) {//iret
		if (prog.index == 0) {
			prog.SP = prog.SP + 1;
			prog.addr = prog.SP | 0xFF00;
		}
		else if (prog.index == 1) {
			prog.SP = prog.SP + 1;
			prog.addr = prog.SP | 0xFF00;
			prog.flags = prog.io;
		}
		else if (prog.index == 2) {
			prog.SP = prog.SP + 1;
			prog.addr = prog.SP | 0xFF00;
			prog.buffer = prog.io;
		}
		else if (prog.index == 3) {
			prog.addr = prog.IP = prog.buffer | (prog.io << 8);
			prog.code = OP_NOP;
		}
	}
	else {
		if (prog.index == 0) {
			prog.SP = prog.SP + 1;
			prog.addr = prog.SP | 0xFF00;
		}
		else if (prog.index == 1) {
			v = prog.io;
			prog.addr = prog.IP;
			switch (prog.code >> 6) {
			case(0): prog.A = v; break;
			case(1): prog.X = v; break;
			case(2): prog.Y = v; break;
			}
			prog.code = OP_NOP;
		}
	}
}

void CALL(CPU& prog, bool& RW) {
	int v = 0;

	if ((prog.code >> 6) == 0) {//ret
		if (prog.index == 0) {
			prog.SP = prog.SP + 1;
			prog.addr = prog.SP | 0xFF00;
		}
		else if (prog.index == 1) {
			prog.SP = prog.SP + 1;
			prog.addr = prog.SP | 0xFF00;
			prog.buffer = prog.io;
		}
		else if (prog.index == 2) {
			prog.addr = prog.IP = prog.buffer | (prog.io << 8);
			prog.code = OP_NOP;
		}
	}
	else {
		if (prog.index == 0) {
			prog.addr = prog.SP | 0xFF00;
			prog.SP = prog.SP - 1;
			RW = true;
			prog.io = (prog.IP + 2) >> 8;
		}
		else if (prog.index == 1) {
			prog.addr = prog.SP | 0xFF00;
			prog.SP = prog.SP - 1;
			RW = true;
			prog.io = (prog.IP + 2) & 0xFF;
		}
		else if (prog.index == 2) {
			prog.addr = prog.IP;
			prog.index = 3;
		}
		else if ((prog.index & 0xC) == 4) {
			if (AddrMode(prog)) {
				prog.IP = prog.addr;
				prog.code = OP_NOP;
			}
		}
	}
}
void SET(CPU& prog, bool& RW) {
	switch (prog.code >> 6) {
	case 0:
		prog.carry = 1;
		break;
	case 1:
		prog.zero = 1;
		break;
	case 2:
		prog.bcd = 1;
		break;
	case 3:
		prog.interruptEnable = 1;
		break;
	}
	prog.code = OP_NOP;
}
void CLR(CPU& prog, bool& RW) {
	switch (prog.code >> 6) {
	case 0:
		prog.carry = 0;
		break;
	case 1:
		prog.zero = 0;
		break;
	case 2:
		prog.bcd = 0;
		break;
	case 3:
		prog.interruptEnable = 0;
		break;
	}
	prog.code = OP_NOP;
}

void LDYX(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
		}
	}
	else if (prog.index == 0x4) {
		prog.X = prog.io;
		prog.addr++;
		if ((prog.code >> 6) == 0) prog.IP++;
	}
	else if (prog.index == 0x5) {
		prog.Y = prog.io;
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}
void STYX(CPU& prog, bool& RW) {
	if ((prog.index & 0xC) == 0) {
		if (AddrMode(prog)) {
			prog.index = 0x3;
			RW = true;
			prog.io = prog.X;
		}
	}
	else if (prog.index == 0x4) {
		prog.io = prog.Y;
		RW = true;
		prog.addr++;
	}
	else if (prog.index == 0x5) {
		prog.addr = prog.IP;
		prog.code = OP_NOP;
	}
}


void CPU::interp(bool& RW, bool interrupt) {
	RW = false;
	if (code == OP_NOP) {
		index = 255;
		if (interrupt && interruptEnable) {
			code = OP_INT;
		}
		else {
			addr = IP = IP + 1;
			code = io;
		}
	}
	else if (code == OP_BRK) {
		char t;
		//std::cin >> t;
		index = 255;
		if (interrupt && interruptEnable) {
			code = OP_INT;
		}
		else {
			addr = IP = IP + 1;
			code = io;
		}
	}

	switch (code & 0x3F) {
	case OP_JMP:
		JMP(*this, RW); break;
	case OP_BZS:
		BZS(*this, RW); break;
	case OP_BZC:
		BZC(*this, RW); break;
	case OP_BCS:
		BCS(*this, RW); break;
	case OP_BCC:
		BCC(*this, RW); break;
	case OP_BG:
		BG(*this, RW); break;
	case OP_BLE:
		BLE(*this, RW); break;
	case OP_LDA:
		LDA(*this, RW); break;
	case OP_LDX:
		LDX(*this, RW); break;
	case OP_LDY:
		LDY(*this, RW); break;
	case OP_STA:
		STA(*this, RW); break;
	case OP_STX:
		STX(*this, RW); break;
	case OP_STY:
		STY(*this, RW); break;
	case OP_INC:
		INC(*this, RW); break;
	case OP_DEC:
		DEC(*this, RW); break;
	case OP_ADD:
		ADD(*this, RW); break;
	case OP_SUB:
		SUB(*this, RW); break;
	case OP_ADDC:
		ADDC(*this, RW); break;
	case OP_SUBC:
		SUBC(*this, RW); break;
	case OP_AND:
		AND(*this, RW); break;
	case OP_OR:
		OR(*this, RW); break;
	case OP_XOR:
		XOR(*this, RW); break;
	case OP_SHL:
		SHL(*this, RW); break;
	case OP_SHR:
		SHR(*this, RW); break;
	case OP_CMP:
		CMP(*this, RW); break;
	case OP_PSH:
		PUSH(*this, RW); break;
	case OP_POP:
		POP(*this, RW); break;
	case OP_CAL:
		CALL(*this, RW); break;
	case OP_SF:
		SET(*this, RW); break;
	case OP_CF:
		CLR(*this, RW); break;
	case OP_STYX:
		STYX(*this, RW); break;
	case OP_LDYX:
		LDYX(*this, RW); break;
	}

	index = index + 1;
}