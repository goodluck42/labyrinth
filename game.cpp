#include "game.h"
#define _RGRAPPLE "<="
#define _LGRAPPLE "=>"
#include "snapshot.h"
namespace labyrinth {
	static const size field_width = 75;
	static const size field_height = 35;
	void exit_game() {
		auto& _i = intf::get();
		auto& _g = game::get();
		_i.clear(0, 0, _g.fwidth, _g.fheight);
		if (_g._player != nullptr) {
			_g._player->pinfo_bombs.erase();
			_g._player->pinfo_coins.erase();
			_g._player->pinfo_enemies.erase();
			_g._player->pinfo_health.erase();
			_g._player->pinfo_mines.erase();
		}
		_g.ginfo.erase();
		io.scpos(0, 0);
		_g.free(false);
		std::exit(0);
	}
	void make_snapshot() {
		auto& _i = intf::get();
		auto& _g = game::get();
		const size textpos_x = _g.fwidth * 0.50 - 8;
		const size textpos_y = _g.fheight * 0.4;
		static const field fld(textpos_x, textpos_y, textpos_x + 32, textpos_y);
		_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
		io.scpos(textpos_x, textpos_y);
		string new_mapname;
		if (maps.size() >= 12) {
			io << "> Limit of maps reached";
			std::this_thread::sleep_for(std::chrono::milliseconds(1200));
			fld.erase();
			_g.use_menu(pmenu);
			return;
		}
		do {
			io << "Enter map name > ";
			new_mapname = io.read(16);
			if (new_mapname == "") {
				_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
				_g.use_menu(pmenu);
				return;
			}
			if (!is_map_exists(new_mapname)) {
				snapshot(_g.mlevel->raw_grid, new_mapname);
				_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
				_g.use_menu(pmenu);
				return;
			} else {
				fld.erase();
				fld.update("> Map already exists");
				std::this_thread::sleep_for(std::chrono::milliseconds(1200));
				fld.erase();
				io.scpos(textpos_x, textpos_y);
			}
		} while (true);
	}
	void show_maps(menu& this_m) {
		auto& _i = intf::get();
		auto& _g = game::get();
		const size textpos_x = 2;
		const size textpos_y = 1;
		_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
		io.scpos(textpos_x, textpos_y);
		_g.use_menu(this_m, true);
		_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
		_g.use_menu(mmenu);
	}
	vector<menu::row> get_loadmap_rows() {
		vector<menu::row> rows;
		if (maps.size() == 0) {
			mapdata _nulldata;
			_nulldata.name = "> There are no saved maps";
			rows.push_back(menu::row(&_nulldata, " ", " ", nullptr));
		} else {
			for (auto& md : maps) {
				menu::row nrow(&md, " ", ">", [](mapdata* md) -> void {
					auto& g = game::get();
					g.start(g.level_enemies, g.level_aidkits, g.level_bombs, g.level_mines, &md->grid);
				});
				rows.push_back(nrow);
			}
		}
		return rows;
	}
	vector<menu::row> get_deletemap_rows() {
		vector<menu::row> rows;
		if (maps.size() == 0) {
			mapdata _nulldata;
			_nulldata.name = "> There are no maps to delete";
			rows.push_back(menu::row(&_nulldata, " ", " ", nullptr));
		} else {
			for (auto& md : maps) {
				menu::row nrow(&md, " ", ">", [](mapdata* md) -> void {
					auto& g = game::get();
					removemap(md->name);
					savedata();
				});
				rows.push_back(nrow);
			}
		}
		return rows;
	}
	vector<menu::row> mmenu_content{menu::row("START GAME", _RGRAPPLE, _LGRAPPLE, []() -> void {
		auto& _i = intf::get();
		auto& _g = game::get();
		_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
		_g.start(_g.level_enemies, _g.level_aidkits, _g.level_bombs, _g.level_mines);
	}), menu::row("LOAD MAP",  _RGRAPPLE, _LGRAPPLE, []() -> void {
		auto mrows = get_loadmap_rows();
		auto& _g = game::get();
		menu this_m(_g.fwidth, _g.fheight, mrows);
		show_maps(this_m);
	}), menu::row("DELETE MAP",  _RGRAPPLE, _LGRAPPLE, []() -> void {
		auto mrows = get_deletemap_rows();
		auto& _g = game::get();
		menu this_m(_g.fwidth, _g.fheight, mrows);
		show_maps(this_m);
	}), menu::row("EXIT GAME", "!", "!", exit_game)};
	vector<menu::row> pmenu_content{menu::row("CONTINUE", _RGRAPPLE, _LGRAPPLE, []() -> void {
		auto& _g = game::get();
		auto& _i = intf::get();
		_i.clear(_g.x_shift, _g.y_shift, _g.fwidth - 2, _g.fheight - 1);
		_i.render_level(_g.x_shift, _g.y_shift, *_g.mlevel, &_g._player->known_mines);
		_g._player->listen();
	}), menu::row("SPANSHOT MAP", _RGRAPPLE, _LGRAPPLE, make_snapshot), menu::row("MAIN MENU", _RGRAPPLE, _LGRAPPLE, []() -> void {
		auto& _g = game::get();
		auto& _i = intf::get();
		_g._player->free(nullptr, true, true);
		_g.use_menu(mmenu);
	}), menu::row("EXIT", "!", "!", exit_game)};
	menu mmenu(field_width, field_height, mmenu_content);
	menu pmenu(field_width, field_height, pmenu_content);
	game::game(): fwidth(field_width), fheight(field_height), ui(nullptr), mlevel(nullptr), _player(nullptr), is_game_started(false), level_bombs(3), level_aidkits(3), level_enemies(12), level_mines(9), player_bombs(1), player_mines(3), player_health(120), ginfo(field_width + 2, 6, 24, 18), x_shift(1), y_shift(1) { }
	game& game::get() {
		static game instance;
		return instance;
	}
	void game::init() {
		fetch_data();
		this->ui = &intf::get();
		this->ui->render_border(0, 0, this->fwidth, this->fheight);
		this->ui->render_menu(0, 0, mmenu);
		mmenu.listen();
	}
	void game::start(const size enemies, const size aidkits, const size bombs, const size mines, vector<vector<int>>* grid) {
		if (this->is_game_started) {
			return;
		}
		this->is_game_started = true;
		this->mlevel = grid == nullptr ? new level(this->fwidth - 2, this->fheight - 2) : new level(this->fwidth - 2, this->fheight - 2, *grid);
		this->_player = new player(this->x_shift, this->y_shift, this->fwidth + 2, 0, *this->mlevel, this->player_health, this->player_bombs, this->player_mines);
		this->mlevel->generate();
		this->mlevel->generate_coins();
		this->mlevel->generate_enemies(this->x_shift, this->y_shift, 12, *this->_player);
		this->mlevel->generate_aidkits(aidkits);
		this->mlevel->generate_bombs(bombs);
		this->mlevel->spawn_exit();
		this->mlevel->spawn_enemies();
		this->mlevel->generate_mines(mines);
		this->ui->render_level(this->x_shift, this->y_shift, *this->mlevel);
		this->ui->render_ginfo(this->fwidth + 2, 6, this->ginfo);
		this->_player->spawn();
		this->_player->update_pinfo();
		this->_player->listen();
	}
	void game::free(bool start_new) {
		this->is_game_started = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		if (this->mlevel != nullptr) {
			delete this->mlevel;
			this->mlevel = nullptr;
		}
		if (this->_player != nullptr) {
			delete this->_player;
			this->_player = nullptr;
		}
		if (start_new) {
			this->ui->render_menu(0, 0, mmenu);
			mmenu.listen();
		}
	}
	void game::use_menu(menu& tmenu, bool listen_esc) {
		this->ui->render_menu(0, 0, tmenu);
		tmenu.listen(listen_esc);
	}
}