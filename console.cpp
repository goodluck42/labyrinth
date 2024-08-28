#include "console.h"

namespace labyrinth {
	console& console::operator<<(symbols::color __right) {
		SetConsoleTextAttribute(this->stdhandle, static_cast<WORD>(__right));
		return *this;
	}
	console& console::operator<<(string __right) {
		WriteConsoleA(this->stdhandle, __right.c_str(), __right.size(), NULL, NULL);
		return *this;
	}
	console& console::operator<<(char __right) {
		WriteConsoleA(this->stdhandle, &__right, 1, NULL, NULL);
		return *this;
	}
	console& console::operator<<(int __right) {
		return *this << std::to_string(__right);
	}
	console& console::operator>>(string& __right) {
		__right.assign(this->read());
		return *this;
	}
	void console::repeat(string __arg, const size t) {
		for (size i = 0; i < t; i++) {
			*this << __arg;
		}
	}
	void console::repeat(char __arg, const size t) {
		for (size i = 0; i < t; i++) {
			*this << __arg;
		}
	}
	void console::repeat_vertical(const coord x, const coord y, string __arg, const size t) {
		coord _y = y;
		for (size i = 0; i < t; i++) {
			this->scpos(x, _y);
			*this << __arg;
			_y++;
		}
	}
	void console::repeat_vertical(const coord x, const coord y, char __arg, const size t) {
		coord _y = y;
		for (size i = 0; i < t; i++) {
			this->scpos(x, _y);
			*this << __arg;
			_y++;
		}
	}
	void console::repeat_vertical(const coord x, string __arg, const size t) {
		coord y;
		this->gcpos(nullptr, &y);
		this->repeat_vertical(x, y, __arg, t);
	}
	void console::repeat_vertical(const coord x, char __arg, const size t) {
		coord y;
		this->gcpos(nullptr, &y);
		this->repeat_vertical(x, y, __arg, t);
	}
	void console::repeat_vertical(string __arg, const size t) {
		this->repeat_vertical(0, __arg, t);
	}
	void console::repeat_vertical(char __arg, const size t) {
		this->repeat_vertical(0, __arg, t);
	}
	void console::scpos(const coord x, const coord y) {
		SetConsoleCursorPosition(this->stdhandle, {x, y});
	}
	void console::svcpos() {
		this->gcpos(&this->sx, &this->sy);
	}
	void console::rcpos() {
		this->scpos(this->sx, this->sy);
	}
	void console::gcpos(coord* x, coord* y) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(this->stdhandle, &info);
		if (x != nullptr)  {
			*x = info.dwCursorPosition.X;
		}
		if (y != nullptr)  {
			*y = info.dwCursorPosition.Y;
		}
	}
	void console::mvup() {
		coord x;
		coord y;
		this->gcpos(&x, &y);
		if (y != 0) {
			this->scpos(x, y - 1);
		}
	}
	void console::mvright() {
		coord x;
		coord y;
		coord xbnd;
		this->gbnd(&xbnd, nullptr);
		this->gcpos(&x, &y);
		if (x != xbnd) {
			this->scpos(x + 1, y);
		}
	}
	void console::mvdown() {
		coord x;
		coord y;
		this->gcpos(&x, &y);
		this->scpos(x, y + 1);
	}
	void console::mvleft() {
		coord x;
		coord y;
		this->gcpos(&x, &y);
		if (x != 0) {
			this->scpos(x - 1, y);
		}
	}
	void console::mvx0() {
		coord y;
		this->gcpos(nullptr, &y);
		this->scpos(0, y);
	}
	void console::mvy0() {
		coord x;
		this->gcpos(&x, nullptr);
		this->scpos(x, 0);
	}
	void console::mvxy0() {
		this->scpos(0, 0);
	}
	void console::at(const coord x, const coord y, string __arg) {
		this->svcpos();
		this->scpos(x, y);
		*this << __arg;
		this->rcpos();
	}
	console& console::get() {
		static console instance;
		return instance;
	}
	void console::get_resolution(size* width, size* height) {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		if (width != nullptr) {
			*width = desktop.right;
		}
		if (height != nullptr) {
			*height = desktop.bottom;
		}
	}
	string console::read(size read_length) {
		string _out;
		auto flag = true;
		size lim = 0;
		do {
			auto rawk = this->readch();
			auto k = static_cast<key>(rawk);
			auto symbol = static_cast<char>(rawk);
			switch (k) {
				case key::ESCAPE: {
					return string();
				}
				case key::ENTER: {
					if (_out.size() != 0) {
						flag = false;
					}
					break;
				}
				case key::TAB: {
					if (lim + 4 <= read_length) {
						for (size i = 0; i < 4; i++) {
							_out.push_back(' ');
							*this << ' ';
							lim++;
						}
						break;
					}
				}
				case key::BACKSPACE: {
					if (_out.size() == 0) {
						break;
					}
					coord _x;
					coord _y;
					coord _bx;
					this->gbnd(&_bx, nullptr);
					this->gcpos(&_x, &_y);
					static auto endtrigger = true;
					//Пофикси BACKSPACE 
					if (_x == 0) {
						this->scpos(_bx - 1, _y - 1);
						this->erase(_bx - 1, _y - 1);
					} else {
						if (_x == _bx - 1 && endtrigger) {
							this->scpos(_bx - 1, _y);
							this->erase(_bx - 1, _y);
							endtrigger = false;
						} else {
							this->scpos(_x - 1, _y);
							this->erase(_x - 1, _y);
						}
						lim--;
					}
					_out.pop_back();
					break;
				}
				default: {
					if (lim + 1 <= read_length) {
						coord _x;
						coord _y;
						this->gcpos(&_x, &_y);
						_out.push_back(symbol);
						*this << symbol;
						lim++;
					}
				}
			}
		} while (flag);
		return _out;
	}
	key console::readk() {
		return static_cast<key>(this->readch());
	}
	void console::readk(key k) {
		do {
			if (this->readk() == k) {
				break;
			}
		} while (true);
	}
	key console::readk(bool(*comparator)(key k)) {
		do {
			auto k = this->readk();
			if (comparator == nullptr) {
				return k;
			}
			if (comparator(k)) {
				return k;
			}
		} while (true);
	}
	void console::erase(coord x, coord y) {
		this->erase(x, y, 1);
	}
	void console::erase(coord x, coord y, size count) {
		COORD coord = {x, y};
		DWORD written;
		FillConsoleOutputCharacter(this->stdhandle, ' ', count, coord, &written);
		FillConsoleOutputAttribute(this->stdhandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, count, coord, &written);
	}
	void console::gbnd(coord* x, coord* y) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(this->stdhandle, &info);
		if (x != nullptr) {
			*x = info.dwSize.X;
		}
		if (y != nullptr) {
			*y = info.dwSize.Y;
		}
	}
	//private
	console::console(): stdhandle(GetStdHandle(STD_OUTPUT_HANDLE)), window(GetConsoleWindow()), sx(0), sy(0) {
		SetWindowLong(this->window, GWL_STYLE, GetWindowLong(this->window, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
		CONSOLE_CURSOR_INFO info;
		RECT rect;
		GetWindowRect(this->window, &rect); 
		MoveWindow(this->window, rect.left, rect.top, 900, 600, TRUE);
		GetConsoleCursorInfo(this->stdhandle, &info);
		info.bVisible = FALSE;
		SetConsoleCursorInfo(this->stdhandle, &info);
	}
	int console::readch() {
		return ::_getch();
	}
	console& io = console::get();
}