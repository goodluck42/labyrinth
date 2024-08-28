#include "intf.h"
#include "field_rect.h"

namespace labyrinth {
	using namespace symbols;
	intf::intf() {};
	intf& intf::get() {
		static intf instance;
		return instance;
	}
	void intf::render_border(const coord stx, const coord sty, const size width, const size height) {
		io.scpos(stx, sty);
		io.repeat(parse(block::bold), width);
		io.repeat_vertical(stx, sty + 1, parse(block::left), height - 2);
		io.repeat_vertical(static_cast<coord>(stx + width - 1), sty + 1, parse(block::right), height - 2);
		io.scpos(stx, sty + height - 1);
		io.repeat(parse(block::bold), width);
	}
	void intf::render_menu(const coord stx, const coord sty, menu& mmenu) {
		static auto center = [](const menu::row& row, const size& width, const size& y, menu::alignment align, const coord stx) -> void {
			switch (align) {
				case menu::alignment::center: {
					io.scpos((width / 2) - (row.text.size() / 2), y);
					break;
				}
				case menu::alignment::left: {
					io.scpos(stx, y);
					break;
				}
			}
			io << row.text;
		};
		static auto wdata = [](menu::row& row, const size& width, const coord& y, menu::alignment align, const coord stx) -> void {
			const auto tlength = row.text.size();
			switch (align) {
				case menu::alignment::center: {
					row.xb = (width / 2) - (tlength / 2);
					row.xe = row.xb + tlength;
					break;
				}
				case menu::alignment::left: {
					row.xb = stx;
					row.xe = stx + tlength;
					break;
				}
			}
			row.lgrapple_x = row.xb - row.lgrapple.size() - 1;
			row.rgrapple_x = row.xe + 1;
			row.y = y;
		};
		io.scpos(stx, sty);
		size y = (mmenu.height / 2) - (mmenu.rows.size() + 1);
		for (size i = 0; i < mmenu.rows.size(); i++) {
			center(mmenu.rows[i], mmenu.width, y, mmenu.align, stx);
			wdata(mmenu.rows[i], mmenu.width, y, mmenu.align, stx);
			y += 2;
		}
	}
	void intf::render_level(const coord stx, const coord sty, level& lv, vector<int*>* known_mines) {
		auto contains = [known_mines](int* v) -> bool {
			for (auto& s : *known_mines) {
				if (s == v) {
					return true;
				}
			}
			return false;
		};
		io.scpos(stx, sty);
		for (size i = 0; i < lv.h; i++) {
			for (size j = 0; j < lv.w; j++) {
				auto& v = lv(i, j);
				io << (known_mines != nullptr && contains(&v) ? symbols::color::red : symbols::get_color_for(v)) << parse(lv(i, j));
			}
			io.scpos(stx, sty + i + 1);
		}
		io << symbols::color::white;
	}
	void intf::clear(const coord stx, const coord sty, const size width, const size height) {
		for (size i = sty; i < height; i++) {
			io.erase(stx, i, width);
		}
	}
	void intf::render_ginfo(const coord stx, const coord sty, const field_rect& ginfo) {
		using namespace symbols;
		size y_shift = sty;
		io.scpos(stx, y_shift++);
		io << get_color_for(_PLAYER) << parse(_PLAYER) << " - player";
		io.scpos(stx, y_shift++);
		io << get_color_for(_EXIT) << parse(_EXIT) << " - exit";
		io.scpos(stx, y_shift++);
		io << get_color_for(_COIN) << parse(_COIN) << " - coin (friendly)";
		io.scpos(stx, y_shift++);
		io << get_color_for(_MINE) << parse(_MINE) << " - mine (friendly)";
		io.scpos(stx, y_shift++);
		io << symbols::color::red << parse(_MINE) << " - mine";
		io.scpos(stx, y_shift++);
		io << get_color_for(_ENEMY) << parse(_ENEMY) << " - enemy";
		io.scpos(stx, y_shift++);
		io << get_color_for(_BOMB) << parse(_BOMB) << " - bomb (friendly)";
		io.scpos(stx, y_shift++);
		io << get_color_for(_HEALTH) << parse(_HEALTH) << " - medkit";
		io << color::white;
		ginfo.update(stx, sty + 10, "W - move forward");
		ginfo.update(stx, sty + 11, "A - move left");
		ginfo.update(stx, sty + 12, "S - move backward");
		ginfo.update(stx, sty + 13, "D - move right");
		ginfo.update(stx, sty + 14, "Space - bomb trigger");
		ginfo.update(stx, sty + 15, "X - place a mine");
	}
	void intf::render_loseinfo(const coord stx, const coord sty, const size width, const size height) {
		const string text = "You lose. Failures happen :<";
		io.scpos(stx + width / 2 - (text.size() / 2), sty + height / 2);
		io << color::red << text << color::white;
	}
	void intf::render_wininfo(const coord stx, const coord sty, const size width, const size height) {
		const string text = "You win!";
		io.scpos(stx + width / 2 - (text.size() / 2), sty + height / 2);
		io << color::violetV2 << text << color::white;
	}
	void intf::render_gameoverinfo(const coord stx, const coord sty, const size width, const size height, bool win) {
		const string text = win ? "You win!" : "You lose. Failures happen :<";
		io.scpos(stx + width / 2 - (text.size() / 2), sty + height / 2);
		io << (win ? color::violetV2 : color::red) << text << color::white;
	}
}