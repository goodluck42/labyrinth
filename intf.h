#ifndef __INTF__
	#define __INTF__
	#include "resource.h"
	#include "console.h"
	#include "menu.h"
	#include "level.h"
	namespace labyrinth {
		class field_rect;
		class intf {
			public:
				void render_border(const coord stx, const coord sty, const size width, const size height);
				void render_menu(const coord stx, const coord sty, menu& mmenu);
				void render_level(const coord stx, const coord sty, level& lv, vector<int*>* known_mines = nullptr);
				void render_ginfo(const coord stx, const coord sty, const field_rect& ginfo);
				void render_loseinfo(const coord stx, const coord sty, const size width, const size height);
				void render_wininfo(const coord stx, const coord sty, const size width, const size height);
				void render_gameoverinfo(const coord stx, const coord sty, const size width, const size height, bool win);
				void clear(const coord stx, const coord sty, const size width, const size height);
				static intf& get();
				intf(const intf&) = delete;
				void operator=(const intf&) = delete;
			private:
				intf();
		};
	}
#endif