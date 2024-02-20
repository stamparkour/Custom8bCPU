#pragma once

typedef unsigned char byte;
typedef unsigned short word;


class Reg8 {
	unsigned char d;
	void set(int v) {
		d = v;
	}
public:
	Reg8() {
		d = 0;
	}
	Reg8(int v) {
		set(v);
	}
	inline Reg8& operator =(int v) noexcept {
		set(v);
		return *this;
	}
	inline operator int() const noexcept {
		return d;
	}
};

class Reg4 {
	unsigned char d;
	void set(int v) {
		d = v & 0xF;
	}
public:
	Reg4() {
		d = 0;
	}
	Reg4(int v) {
		set(v);
	}
	inline Reg4& operator =(int v) noexcept {
		set(v);
		return *this;
	}
	inline operator int() const noexcept {
		return d;
	}
};

class Reg12 {
	unsigned short d;
	void set(int v) {
		d = v & 0xFFF;
	}
public:
	Reg12() {
		d = 0;
	}
	Reg12(int v) {
		set(v);
	}
	inline Reg12& operator =(int v) noexcept {
		set(v);
		return *this;
	}
	inline operator int() const noexcept {
		return d;
	}
};

class Reg16 {
	unsigned short d;
	void set(int v) {
		d = v;
	}
public:
	Reg16() {
		d = 0;
	}
	Reg16(int v) {
		set(v);
	}
	inline Reg16& operator =(int v) noexcept {
		set(v);
	}
	inline operator int() const noexcept {
		return d;
	}
};

enum OPCODE {
	OP_NOP,
	OP_JMP,
	OP_BCS,//l
	OP_BZS,//e
	OP_BCC,//ge
	OP_BZC,//ne
	//OP_BL,
	//OP_BGE,
	OP_BG,
	OP_BLE,
	OP_STX_c,
	OP_STA_c,
	OP_LDX_c,
	OP_LDA_c,
	OP_STX_x,
	OP_STA_x,
	OP_LDX_x,
	OP_LDA_x,
	OP_INCX,
	OP_INCA,
	OP_DECX,
	OP_DECA,
	OP_ADD_c,
	OP_SUB_c,
	OP_ADDc_c,
	OP_SUBc_c,
	OP_XOR_c,
	OP_AND_c,
	OP_OR_c,
	OP_SHL_c,
	OP_SHR_c,
	OP_CMP_c,
	OP_ADD_x,
	OP_SUB_x,
	OP_ADDc_x,
	OP_SUBc_x,
	OP_XOR_x,
	OP_AND_x,
	OP_OR_x,
	OP_SHL_x,
	OP_SHR_x,
	OP_CMP_x,
	OP_PSHA,
	OP_POPA,
	OP_PSHX,
	OP_POPX,
	OP_CAL,
	OP_RET,
	OP_BRK,
	OP_STSP,
};

struct CPU {

	Reg4 index;
	Reg8 code;
	Reg8 buffer;
	Reg8 A;
	Reg8 X;
	Reg12 IP;
	Reg8 SP = 255;
	bool negative;
	bool carry;
	bool zero;
	bool interp(Reg12& addr, byte& bus);
};

int CompileToRom(const char* txt, byte* _rom, int romLength);
void RunCPU(byte* _rom, byte* _ram);