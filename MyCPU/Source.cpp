#include "myCPU.h"
#include <Windows.h>

//math - alu
//jumping - jump branch
//functions - call ret enter leave
//data manip - mov reg to addr, mov addr to reg

byte rom[2048] = {};
byte ram[2048] = {};
int main(char* argv[], size_t argc) {
	if(CompileToRom(
#include "code.txt"
,rom,2048)) return 1;
	RunCPU(rom, ram);
}