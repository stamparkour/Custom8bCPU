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


byte rom[0x7FFF] = {};
byte ram[0x7FFF] = {};
int main(size_t argc, char* argv[]) {
	std::fstream s{argv[1], std::ios::in | std::ios::binary};
	s.read((char*)rom, 0x7FFF);
	RunCPU(rom, ram);
}