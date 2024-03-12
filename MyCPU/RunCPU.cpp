#include "myCPU.h"
#include <chrono>
#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#pragma warning(disable : 4996)

byte* rom = 0;
byte* ram = 0;
int musicData = 0;

byte ioMode = 0;
byte ioIndex = 0;

bool key_was_pressed(void)
{
	return (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0);
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD getxy()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

COORD _BACK{};
void pushxy(int x, int y) {
	_BACK = getxy();
	gotoxy(x, y);
}
void popxy() {
	gotoxy(_BACK.X, _BACK.Y);
}

void Display(int addr, int v) {
	addr %= 32;
	pushxy(addr % 16, addr / 16);
	char arr[2];
	arr[0] = v;
	arr[1] = 0;
	printf(arr);
	popxy();
}

char keyDown;
void memoryManager(byte& bus, int addr, bool rw) {
	if (addr < 0x7FFF) {
		if (rw);
		else {
			bus = rom[addr];
		}
	}
	else if (addr >= 0xFEE0 && addr < 0xFF00) {
		if (rw) {
			Display(addr - 0xFEE0, bus);
		}
	}
	else if(addr < 0x10000) {
		if (rw) {
			ram[addr - 0x8000] = bus;
		}
		else {
			bus = ram[addr - 0x8000];
		}
	}
}

template <
	class result_t = std::chrono::milliseconds,
	class clock_t = std::chrono::steady_clock,
	class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

const char* OPCODE_NAME[] = {
	"NOP",
	"JMP",
	"BZS",//e
	"BZC",//ne
	"BCS",//l
	"BCC",//ge
	"BG",
	"BLE",
	"LDA",
	"LDX",
	"LDY",
	"STA",
	"STX",
	"STY",
	"INC",
	"DEC",
	"ADD",
	"SUB",
	"ADDC",
	"SUBC",
	"AND",
	"OR",
	"XOR",
	"SHL",
	"SHR",
	"CMP",
	"PSH",
	"POP",
	"CAL",
	"RET",
	"SF",
	"CF",
};
const char* OPCODE_MODE[] = {
	"c",
	"a",
	"x",
	"yx"
};
void getOpName(byte code, char* b) {
	sprintf(b, "%s%s", OPCODE_NAME[code&0x1F], OPCODE_MODE[code >> 6]);
}

int frame = 0;
void printCPU(CPU& prog, bool rw) {
	frame++;
	char b[16];
	getOpName(prog.code, b);
	printf("[%i]%.4x : %.2x %c\n", frame, (int)prog.addr, (int)prog.io, rw ? 'W' : 'R');
	printf(" - code : %.2x - %s, IP: %.4x, A: %.2x, X: %.2x\n", (int)prog.code, b, (int)prog.IP, (int)prog.A, (int)prog.X);
}

double timerTickLength = 0;
long long timerStart = 0;
HWND windowHandle;

void InitTimer() {
	LARGE_INTEGER largeInt{};
	QueryPerformanceFrequency(&largeInt);
	timerTickLength = 1.0 / largeInt.QuadPart;
	QueryPerformanceCounter(&largeInt);
	timerStart = largeInt.QuadPart;
}

double GetTime() {
	LARGE_INTEGER largeInt{};
	QueryPerformanceCounter(&largeInt);
	return timerTickLength * (largeInt.QuadPart - timerStart);
}

void MusicControlLoop() {
	while (true) {
		int note = musicData & 0x1F;
		int length = musicData >> 5;
		int frequency = (int)(440 * powf(2, (note-12) / 12.0));
		if(!Beep(frequency, length * 125)) 
			throw 1;
	}
}

void RunCPU(byte* _rom, byte* _ram) {
	InitTimer();

	rom = _rom;
	ram = _ram;
	bool rw = false;
	CPU prog = {};
	int i = 0;

	std::thread music{ MusicControlLoop };
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int prev = since(start).count();
	double prevTime = 0;
	while (true) {
		memoryManager((uint8_t&)prog.io, prog.addr, rw);
		//printCPU(prog, rw);
		prog.interp(rw, GetKeyState(VK_SPACE) & 0x8000);

		double wait;
		do {
			double time = GetTime();
			wait = 1.0 / 1024 - (time - prevTime);
		} while (wait > 0);
		prevTime = GetTime();
	}
}