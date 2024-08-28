#include "enemy.h"
#include "player.h"

namespace labyrinth {
	enemy::enemy(player* _player,const size shx, const size shy, const size gx, const size gy, vector<vector<int>>& grid): _player(_player), shx(shx), shy(shy), alive(false), grid(grid), x(0), y(0), gx(gx), gy(gy), pdir(enemy::direction::none), player_dir(direction::none), px(x), py(y), trigger_kill(false), player_hit(0) { }
	void enemy::automove() {
		if (!this->is_alive()) {
			return;
		}
		static auto get_available_direction = [](size gx, size gy, vector<vector<int>>& grid, direction* player_dir) -> vector<direction> {
			vector<direction> dirs;
			if (gx != 0) {
				auto& v = grid[gy][gx - 1];
				if (v != _BLOCK && v != _EXIT && v != _ENEMY && v != _BOMB) {
					if (v == _PLAYER && player_dir != nullptr) {
						*player_dir = direction::left;
					}
					dirs.push_back(direction::left);
				}
			}
			if (gy != 0) {
				auto& v = grid[gy - 1][gx];
				if (v != _BLOCK && v != _EXIT && v != _ENEMY && v != _BOMB) {
					if (v == _PLAYER && player_dir != nullptr) {
						*player_dir = direction::top;
					}
					dirs.push_back(direction::top);
				}
			}
			if (gy != grid.size() - 1) {
				auto& v = grid[gy + 1][gx];
				if (v != _BLOCK && v != _EXIT && v != _ENEMY && v != _BOMB) {
					if (v == _PLAYER && player_dir != nullptr) {
						*player_dir = direction::bottom;
					}
					dirs.push_back(direction::bottom);
				}
			}
			if (gx != grid[gy].size() - 1) {
				auto& v = grid[gy][gx + 1];
				if (v != _BLOCK && v != _EXIT && v != _ENEMY && v != _BOMB) {
					if (v == _PLAYER && player_dir != nullptr) {
						*player_dir = direction::right;
					}
					dirs.push_back(direction::right);
				}
			}
			return dirs;
		};
		static auto has_direction = [](vector<direction>& dirs, direction dir) -> bool {
			for (auto& v : dirs) {
				if (v == dir) {
					return true;
				}
			}
			return false;
		};
		static auto get_next_pos = [](vector<vector<int>>& grid, size& gx, size& gy, direction dir) -> int& {
			switch (dir) {
				case labyrinth::enemy::direction::top: {
					return grid[gy - 1][gx];
				}
				case labyrinth::enemy::direction::right: {
					return grid[gy][gx + 1];
				}
				case labyrinth::enemy::direction::bottom: {
					return grid[gy + 1][gx];
				}
				case labyrinth::enemy::direction::left: {
					return grid[gy][gx - 1];
				}
			}
			return grid[gy][gx];
		};
		this->px = this->x;
		this->py = this->y;
		this->player_dir = direction::none;
		this->updatepos();
		if (this->x != this->px || this->y != this->py) {
			io.erase(this->px, this->py);
		}
		auto& position = this->grid[this->gy][this->gx];
		auto available_dirs = get_available_direction(this->gx, this->gy, this->grid, &this->player_dir);
		if (available_dirs.size() == 0) {
			return;
		}
		auto dir = this->player_dir != direction::none ? this->player_dir : has_direction(available_dirs, this->pdir) ? this->pdir : random_of<direction>(available_dirs);
		auto& next_pos = get_next_pos(this->grid, this->gx, this->gy, dir);
		this->pdir = dir;
		switch (next_pos) {
			case _PLAYER: {
				if (this->_player != nullptr) {
					this->_player->hit(random<size>(5, 10));
					this->player_hit++;
					if (this->player_hit == 5) {
						this->kill();
						this->_player->enemies_killed++;
					}
					this->_player->update_pinfo();
				}
				break;
			}
			case _MINE: {
				next_pos = _FSPACE;
				switch (dir) {
					case labyrinth::enemy::direction::top: {
						io.erase(this->x, this->y - 1);
						break;
					}
					case labyrinth::enemy::direction::right: {
						io.erase(this->x + 1, this->y);
						break;
					}
					case labyrinth::enemy::direction::bottom: {
						io.erase(this->x, this->y + 1);
						break;
					}
					case labyrinth::enemy::direction::left: {
						io.erase(this->x - 1, this->y);
						break;
					}
				}
				this->kill();
				this->_player->enemies_killed++;
				this->_player->update_pinfo();
				break;
			}
			case _COIN: {
				this->_player->_level.coins_generated--;
				this->_player->update_pinfo();
			}
			case _HEALTH:
			case _FSPACE: {
				position = _FSPACE;
				next_pos = _ENEMY;
				switch (dir) {
					case labyrinth::enemy::direction::top: {
						this->gy--;
						break;
					}
					case labyrinth::enemy::direction::right: {
						this->gx++;
						break;
					}
					case labyrinth::enemy::direction::bottom: {
						this->gy++;
						break;
					}
					case labyrinth::enemy::direction::left: {
						this->gx--;
						break;
					}
				}
			}
		}
	}
	void enemy::updatepos() {
		this->x = this->shx + this->gx;
		this->y = this->shy + this->gy;
		io << symbols::get_color_for(_ENEMY);
		io.at(this->x, this->y, symbols::parse(_ENEMY));
		io << symbols::color::white;
	}
	void enemy::kill() {
		this->alive = false;
		io.erase(this->x, this->y);
		this->grid[this->gy][this->gx] = _FSPACE;
	}
	void enemy::spawn() {
		this->x = this->shx + this->gx;
		this->y = this->shy + this->gy;
		io.at(this->x, this->y, symbols::parse(_ENEMY));
		this->alive = true;
	}
	bool enemy::is_alive() {
		return this->alive;
	}
}