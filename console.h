#ifndef __CONSOLE__
	#define __CONSOLE__
	#include "resource.h"
	#include <windows.h>
	#include <conio.h>
	namespace labyrinth {
		typedef SHORT coord;
		class console {
			public:
				console(const console&) = delete;
				void operator=(const console&) = delete;
				console& operator<<(symbols::color __right);
				console& operator<<(string __right);
				console& operator<<(char __right);
				console& operator<<(int __right);
				console& operator>>(string& __right);
				void repeat(string __arg, const size t);
				void repeat(char __arg, const size t);
				void repeat_vertical(const coord x, const coord y, string __arg, const size t);
				void repeat_vertical(const coord x, const coord y, char __arg, const size t);
				void repeat_vertical(const coord x, string __arg, const size t);
				void repeat_vertical(const coord x, char __arg, const size t);
				void repeat_vertical(string __arg, const size t);
				void repeat_vertical(char __arg, const size t);
				void repeat_vertical(int, const size) = delete;
				void scpos(const coord x, const coord y);
				void svcpos();
				void rcpos();
				void gcpos(coord* x, coord* y);
				void mvup();
				void mvright();
				void mvdown();
				void mvleft();
				void mvx0();
				void mvy0();
				void mvxy0();
				void at(const coord x, const coord y, string __arg);
				static console& get();
				static void get_resolution(size* width, size* height);
				string read(size read_length = string::npos);
				key readk();
				void readk(key k);
				key readk(bool(*comparator)(key k));
				void erase(coord x, coord y);
				void erase(coord x, coord y, size count);
				void gbnd(coord* x, coord* y);
			private:
				console();
				HANDLE stdhandle;
				HWND window;
				coord sx;
				coord sy;
				int readch();
		};
		extern console& io;
	}
#endif
