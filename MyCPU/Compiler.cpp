#include <stdio.h>
#include <cstring>
#include <utility>
#include "myCPU.h"
#pragma warning(disable : 4996)

struct Label {
	const char* text;
	byte* ptr;

	Label& operator =(Label&& v) noexcept {
		using std::swap;
		swap(text,v.text);
		swap(ptr, v.ptr);
		return *this;
	}
};

struct Opcode {
	const char* opcodeExp = 0;
	int parameters = 0;
	void (*compileAction)(char** para);

	bool compile(const char* txt) {
		if (opcodeExp == 0) return false;
		char arr[4][16];
		char* ptr[4]{ arr[0], arr[1], arr[2], arr[3], };
		if (parameters != sscanf(txt, opcodeExp, arr[0], arr[1], arr[2], arr[3])) return false;
		
		compileAction( ptr);
		return true;
	}
};

static byte rom[2048] = {};
int labelIndex = 3;
Label labels[1024] = {
	{"cout", 4095 + rom},
	{"nout", 4094 + rom},
	{"mout", 4093 + rom},
};
int labelRefIndex = 0;
Label labelRefs[1024] = {};
int romIndex = 0;
int ramIndex = 2048;

#define OPCODE_LENGTH 256
Opcode opcodes[OPCODE_LENGTH]{
	{" %[#]",1,[](char** para) {}},
	{" :%s",1,[](char** para) {
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labels[labelIndex++] = { ptr, rom + romIndex };
	}},
	{" vb:%s",1,[](char** para) {
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labels[labelIndex++] = { ptr, rom + ramIndex };
		ramIndex += 1;
	}},
	{" no%[p]",1,[](char** para) {
		rom[romIndex++] = OP_NOP;
	}},
	{" db %xh",1,[](char** para) {
		rom[romIndex++] = para[0][0];
	}},
	{" db %i",1,[](char** para) {
		rom[romIndex++] = para[0][0];
	}},
	{" jmp :%s",1,[](char** para) {
		rom[romIndex++] = OP_JMP;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" bzs :%s",1,[](char** para) {
		rom[romIndex++] = OP_BZS;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" be :%s",1,[](char** para) {
		rom[romIndex++] = OP_BZS;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" br%[k]",1,[](char** para) {
		rom[romIndex++] = OP_BRK;
	}},
	{" jmp %xh",1,[](char** para) {
		rom[romIndex++] = OP_JMP;
		rom[romIndex++] = para[0][0];
		rom[romIndex++] = para[0][1];
	}},
	{" stx :%s",1,[](char** para) {
		rom[romIndex++] = OP_STX_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" ldx :%s",1,[](char** para) {
		rom[romIndex++] = OP_LDX_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" sta :%s",1,[](char** para) {
		rom[romIndex++] = OP_STA_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" lda :%s",1,[](char** para) {
		rom[romIndex++] = OP_LDA_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" add :%s",1,[](char** para) {
		rom[romIndex++] = OP_ADD_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" sub :%s",1,[](char** para) {
		rom[romIndex++] = OP_SUB_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" and :%s",1,[](char** para) {
		rom[romIndex++] = OP_AND_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" or :%s",1,[](char** para) {
		rom[romIndex++] = OP_OR_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{" addc :%s",1,[](char** para) {
		rom[romIndex++] = OP_ADDc_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	}},
	{ " subc :%s",1,[](char** para) {
		rom[romIndex++] = OP_SUBc_c;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	} },
	{ " call :%s",1,[](char** para) {
		rom[romIndex++] = OP_CAL;
		char* ptr = new char[16];
		strcpy(ptr, para[0]);
		labelRefs[labelRefIndex++] = { ptr, rom + romIndex };
		romIndex += 2;
	} },
	{ " re%[t]",1,[](char** para) {
		rom[romIndex++] = OP_RET;
	} },
};

const char* NextLine(const char* t) {
	int i = 0;
	for (; *t != '\n'; t++) 
		if (*t == '\0') return 0;
	if (*(t+1) == '\0') return 0;
	return t+1;
}
int CompilePhase(const char* txt) {
	while (txt) {
		if (*txt == '\n') goto break1;
		for (int i = 0; i < OPCODE_LENGTH; i++) {
			if (opcodes[i].compile(txt)) goto break1;
		}
		return 1;
		break1:
		txt = NextLine(txt);
	}
	return 0;
}
int LabelPhase() {
	for (int i = 0; i < labelRefIndex; i++) {
		for (int j = 0; j < labelIndex; j++) {
			if (!strcmp(labelRefs[i].text, labels[j].text)) {
				labelRefs[i].ptr[0] = labels[j].ptr - rom;
				labelRefs[i].ptr[1] = (labels[j].ptr - rom) >> 8;
				goto continue1;
			}
		}
		return 2;
	continue1:;
	}
	return 0;
}

int CompileToRom(const char* txt, byte* _rom, int romLength) {
	int v;
	if(v = CompilePhase(txt)) 
		return v;
	if(v = LabelPhase()) 
		return v;

	for (int i = 0; i < romLength; i++) {
		_rom[i] = rom[i];
	}

	return 0;
}