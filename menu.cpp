#include "menu.h"
#include "intf.h"
namespace labyrinth {
	menu::row::row(const string text, const string rgrapple, const string lgrapple, cb callback): data(nullptr), text(text), rgrapple(rgrapple), lgrapple(lgrapple), callback(callback), xb(0), xe(0), y(0), lgrapple_x(0), rgrapple_x(0), callback2(nullptr) {}
	menu::row::row(mapdata* data, const string rgrapple, const string lgrapple, cb2 callback): data(data), text(data->name), rgrapple(rgrapple), lgrapple(lgrapple), callback(nullptr), xb(0), xe(0), y(0), lgrapple_x(0), rgrapple_x(0), callback2(callback) {}
	menu::menu(const size width, const size height, vector<row>& rows, alignment align): width(width), height(height), rows(rows), index(0), mindex(rows.size()), listening(false), align(align) {}
	void menu::listen(bool handle_esc) {
		static auto erase_grapples = [](const menu::row& row) -> void {
			io.erase(row.lgrapple_x, row.y, row.lgrapple.size());
			io.erase(row.rgrapple_x, row.y, row.rgrapple.size());
		};
		static auto show_grapples = [](const menu::row& row) -> void {
			io.scpos(row.lgrapple_x, row.y);
			io << row.lgrapple;
			io.scpos(row.rgrapple_x, row.y);
			io << row.rgrapple;;
		};
		size index = 0;
		this->listening = true;
		do {
			auto& this_row = this->rows[index];
			show_grapples(this_row);
			key k = io.readk([](key pressed_key) -> bool {
				return pressed_key == key::W || pressed_key == key::w || pressed_key == key::S || pressed_key == key::s || pressed_key == key::ENTER || pressed_key == key::ESCAPE;
			});
			switch (k) {
				case key::ESCAPE: {
					if (handle_esc) {
						this->free();
					}
					break;
				}
				case key::W:
				case key::w: {
					if (index != 0) {
						erase_grapples(this_row);
						index--;
					}
					break;
				}
				case key::S:
				case key::s: {
					if (index != this->rows.size() - 1) {
						erase_grapples(this_row);
						index++;
					}
					break;
				}
				case key::ENTER: {				
					if (this_row.callback != nullptr) {
						this_row.callback();
					} else if (this_row.callback2 != nullptr) {
						this_row.callback2(this_row.data);
					}
					this->free();
					break;
				}
			}
		} while (this->listening);
	}
	void menu::free() {
		this->listening = false;
	}
}