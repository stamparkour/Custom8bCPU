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

void memoryManager(byte& bus, int addr, bool rw) {
	if (addr < 2048) {
		if (rw);
		else {
			bus = rom[addr];
		}
	}
	else if(addr < 4000) {
		if (rw) {
			ram[addr - 2048] = bus;
		}
		else {
			bus = ram[addr - 2048];
		}
	}
	else if (addr == 4095) {
		if (rw) {
			char arr[2];
			arr[0] = bus;
			arr[1] = 0;
			printf(arr);
		}
	}
	else if (addr == 4094) {
		if (rw) {
			int v = bus;
			printf("%d\n", v);
		}
	}
	else if (addr == 4093) {
		if (rw) {
			musicData = bus;
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

int frame = 0;
void printCPU(CPU& prog, Reg12& addr, byte bus, bool rw) {
	frame++;
	printf("[%i]%.4x : %.2x %c\n", frame, (int)addr, (int)bus, rw ? 'W' : 'R');
	printf("[%i]code: %.2x, IP: %.4x, A: %.2x, X: %.2x\n", frame, (int)prog.code, (int)prog.IP, (int)prog.A, (int)prog.X);
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
	byte bus = 0;
	Reg12 addr = 0;
	bool rw = false;
	CPU prog = {};
	int i = 0;

	std::thread music{ MusicControlLoop };
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int prev = since(start).count();
	double prevTime = 0;
	while (true) {
		memoryManager(bus, addr, rw);
		rw = prog.interp(addr, bus);
		//printCPU(prog, addr, bus, rw);

		double wait;
		do {
			double time = GetTime();
			wait = 1.0 / 1000 - (time - prevTime);
		} while (wait > 0);
		prevTime = GetTime();
	}
}