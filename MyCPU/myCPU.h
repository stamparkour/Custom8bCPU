#pragma once

typedef unsigned char byte;
typedef unsigned short word;

#define OP_RET OPCODE::OP_POP | 0xC0
#define OP_IRET OPCODE::OP_PUSH | 0xC0
enum OPCODE {
	OP_NOP,
	OP_JMP,
	OP_BZS,//e
	OP_BZC,//ne
	OP_BCS,//l
	OP_BCC,//ge
	OP_BG,
	OP_BLE,
	OP_LDA,
	OP_LDX,
	OP_LDY,
	OP_STA,
	OP_STX,
	OP_STY,
	OP_INC,
	OP_DEC,
	OP_ADD,
	OP_SUB,
	OP_ADDC,
	OP_SUBC,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_SHL,
	OP_SHR,
	OP_CMP,
	OP_PSH,
	OP_POP,
	OP_CAL,
	OP_INT,
	OP_SF,
	OP_CF,
};

struct CPU {

	byte index;
	byte code = OP_JMP | 0x80;
	byte buffer;
	byte A;
	byte X;
	byte Y;
	word IP = 0x7FFE;
	word addr = 0x7FFE;
	byte io;
	byte SP = 255;
	union {
		byte flags;
		struct {
			byte carry : 1;
			byte zero : 1;
			byte bcd : 1;
			byte interruptEnable : 1;
		};
	};
	void interp(bool& RW, bool interrupt);
};

void RunCPU(byte* _rom, byte* _ram);
void InitTimer();
double GetTime();