#pragma once

typedef unsigned char byte;
typedef unsigned short word;

#define OP_NOP OPCODE::OP_JMP
#define OP_BRK OPCODE::OP_BZS
#define OP_RET OPCODE::OP_CAL
#define OP_INT OPCODE::OP_PSH | 0xC0
#define OP_IRET OPCODE::OP_POP | 0xC0
#define OP_XTA OPCODE::OP_INC | 0xC0
#define OP_YTA OPCODE::OP_DEC | 0xC0
#define OP_ATX OPCODE::OP_SHL | 0xC0
#define OP_ATY OPCODE::OP_SHR | 0xC0
#define OP_XTY OPCODE::OP_STX
#define OP_YTX OPCODE::OP_STY
enum OPCODE {
	OP_JMP,
	OP_BZS,//e
	OP_BZC,//ne
	OP_BCS,//l
	OP_BCC,//ge
	OP_BG,
	OP_BLE,
	OP_CAL,
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
	OP_CMP,
	OP_SHL,
	OP_SHR,
	OP_PSH,
	OP_POP,
	OP_SF,
	OP_CF,
	OP_LDYX,
	OP_STYX,
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

void RunCPU(byte* _rom, byte* _ram, const char* flags);
void InitTimer();
double GetTime();