#include "myCPU.h"
#include <Windows.h>
#include <fstream>
#include <xaudio2.h>
#include <math.h>
#include <iostream>

//math - alu
//jumping - jump branch
//functions - call ret enter leave
//data manip - mov reg to addr, mov addr to reg


byte rom[0x8000] = {};
byte ram[0x8000] = {};
int main(size_t argc, char* argv[]) {
	const char* c = argv[1];
	if (!c) c = "customasm_win64\\sp801\\basic.bin";
	std::fstream s{c, std::ios::in | std::ios::binary};
	s.read((char*)rom, 0x8000);
	RunCPU(rom, ram);
}