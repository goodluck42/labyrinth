#ifndef __GAME__
	#define __GAME__
	#include "resource.h"
	#include "field.h"
	#include "field_rect.h"
	#include "menu.h"
	#include "intf.h"
	#include "level.h"
	#include "player.h"
	#include "random.h"
	namespace labyrinth {
		void exit_game();
		void make_snapshot();
		void show_maps(menu& this_m);
		vector<menu::row> get_loadmap_rows();
		vector<menu::row> get_deletemap_rows();
		extern menu mmenu;
		extern menu pmenu;
		extern vector<menu::row> mmenu_content;
		extern vector<menu::row> pmenu_content;
		class game final {
			public:
				game(game const&) = delete;
				void operator=(game const&) = delete;
				void init();
				void start(const size enemies, const size aidkits, const size bombs, const size mines, vector<vector<int>>* grid = nullptr);
				static game& get();
				size fwidth;
				size fheight;
				void free(bool start_new = true);
				size level_bombs;
				size level_aidkits;
				size level_enemies;
				size level_mines;
				size player_bombs;
				size player_mines;
				size player_health;
				const field_rect ginfo;
				void use_menu(menu& tmenu, bool listen_esc = false);
				player* _player;
				const size x_shift;
				const size y_shift;
				level* mlevel;
			private:
				intf* ui;
				bool is_game_started;
				game();
		};
	}
#endif
