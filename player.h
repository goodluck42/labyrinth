#ifndef __PLAYER__
	#define __PLAYER__
	#include "resource.h"
	#include "console.h"
	#include "level.h"
	#include "field_rect.h"
	#include "intf.h"
	namespace labyrinth {
		class player {
			friend class level;
			friend void exit_game();
			public:
				player() = delete;
				player(const size shx, const size shy, const coord stx_pinfo, const coord sty_pinfo, level& _level, const size health, const size bombs, const size mines);
				size health;
				coord x;
				coord y;
				bool is_alive();
				void hit();
				void hit(const size dmg);
				void heal(const size v);
				void listen();
				void free(void(*callback)(intf&, game&, player&) = nullptr, bool erase_p = false, bool erase_d = false);
				void spawn();
				void update_pinfo();
				void freeze();
				size enemies_killed;
				level& _level;
				vector<int*> known_mines;
			private:
				const size shx;
				const size shy;
				const coord stx_pinfo;
				const coord sty_pinfo;
				const field_rect pinfo_coins;
				const field_rect pinfo_health;
				const field_rect pinfo_bombs;
				const field_rect pinfo_mines;
				const field_rect pinfo_enemies;
				size gx;
				size gy;
				size px;
				size py;
				void updatepos();
				bool listening;
				size coins;
				size bombs;
				size mines;
				bool contains_mine(int* mine);
				bool mine_pending;
				bool mine_placed;
				size mine_gx;
				size mine_gy;
		};
	}
#endif
