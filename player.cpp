#include "player.h"
#include "game.h"
#define _COINS_TEXT "Coins: "
#define _HEALTH_TEXT "Health: "
#define _BOMBS_TEXT "Bombs: "
#define _MINES_TEXT "Mines: "
#define _ENEMIES_TEXT "Enemies killed: "
namespace labyrinth {
	player::player(const size shx, const size shy, const coord stx_pinfo, const coord sty_pinfo, level& _level, const size health, const size bombs, const size mines): shx(shx), shy(shy), health(health), x(0), y(0), gx(0), gy(0), _level(_level), coins(0), stx_pinfo(stx_pinfo), sty_pinfo(sty_pinfo), pinfo_coins(stx_pinfo, sty_pinfo, 16, 1), pinfo_health(stx_pinfo, sty_pinfo + 1, 16, 1), listening(false), pinfo_bombs(stx_pinfo, sty_pinfo + 2, 16, 1), bombs(bombs), mines(mines), pinfo_mines(stx_pinfo, sty_pinfo + 3, 16, 1), pinfo_enemies(stx_pinfo, sty_pinfo + 4, 24, 1), px(0), py(0), enemies_killed(0), mine_gx(0), mine_gy(0), mine_pending(false), mine_placed(false) {
		this->pinfo_coins.update(_COINS_TEXT + std::to_string(this->coins) + " / " + std::to_string(_level.coins_generated));
		this->pinfo_health.update(_HEALTH_TEXT + std::to_string(this->health));
		this->pinfo_bombs.update(_BOMBS_TEXT + std::to_string(this->bombs));
		this->pinfo_mines.update(_MINES_TEXT + std::to_string(this->mines));
		this->pinfo_enemies.update(_ENEMIES_TEXT + std::to_string(this->enemies_killed) + " / " + std::to_string(_level.enemies_generated));
	}
	void player::updatepos() {
		this->x = this->shx + this->gx;
		this->y = this->shy + this->gy;
		io << symbols::color::yellow;
		io.at(this->x, this->y, symbols::parse(_PLAYER));
		io << symbols::color::white;
	}
	bool player::is_alive() {
		return this->health != 0;
	}
	void player::hit() {
		if (this->is_alive()) {
			this->health--;
		}
	}
	void player::hit(const size dmg) {
		if (this->is_alive()) {
			if (dmg >= this->health) {
				this->health = 0;
			} else {
				this->health -= dmg;
			}
		}
	}
	void player::heal(const size v) {
		this->health += v;
	}
	void player::listen() {
		if (this->listening) {
			return;
		}
		const size mine_damage = 33;
		bool won = false;
		this->listening = true;
		while(this->listening && this->is_alive()) {
			this->px = this->x;
			this->py = this->y;
			this->updatepos();
			if (this->x != this->px || this->y != this->py && !this->mine_placed) {
				if (this->mine_pending) {
					io << symbols::color::red;
					io.at(this->px, this->py, symbols::parse(_MINE));
					io << symbols::color::white;
					this->_level.grid[this->mine_gy][this->mine_gx] = _MINE;
					this->mine_pending = false;
				} else {
					io.erase(this->px, this->py);
				}
			}
			auto& position = this->_level.grid[this->gy][this->gx];
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (_kbhit()) {
				switch (io.readk()) {
					case key::ESCAPE: {
						this->freeze();
						break;
					}
					case key::W:
					case key::w: {
						if (this->gy != 0) {
							auto& v = this->_level(this->gy - 1, this->gx);
							if (v == _FSPACE || v == _COIN || v == _HEALTH || v == _BOMB || v == _MINE) {
								if (v == _COIN) {
									this->coins++;
								}
								if (v == _HEALTH) {
									this->heal(random<size>(10, 25));
								}
								if (v == _BOMB) {
									this->bombs++;
								}
								if (v == _MINE) {
									if (this->contains_mine(&v)) {
										this->hit(mine_damage);
									} else {
										this->mines++;
									}
								}
								position = _FSPACE;
								v = _PLAYER;
								this->gy--;
							} else {
								if (v == _EXIT) {
									this->listening = false;
									won = true;
									break;
								}
								if (v == _ENEMY) {
									this->hit(random<size>(2, 10));
								}
							}
							this->update_pinfo();
						}
						break;
					}
					case key::S:
					case key::s: {
						if (this->gy != this->_level.h - 1) {
							auto& v = this->_level(this->gy + 1, this->gx);
							if (v == _FSPACE || v == _COIN || v == _HEALTH || v == _BOMB || v == _MINE) {
								if (v == _COIN) {
									this->coins++;
								}
								if (v == _HEALTH) {
									this->heal(random<size>(10, 25));
								}
								if (v == _BOMB) {
									this->bombs++;
								}
								if (v == _MINE) {
									if (this->contains_mine(&v)) {
										this->hit(mine_damage);
									} else {
										this->mines++;
									}
								}
								position = _FSPACE;
								v = _PLAYER;
								this->gy++;
							} else {
								if (v == _EXIT) {
									this->listening = false;
									won = true;
									break;
								}
								if (v == _ENEMY) {
									this->hit(random<size>(15, 25));
								}
							}
							this->update_pinfo();
						}
						break;
					}
					case key::A:
					case key::a :{
						if (this->gx != 0) {
							auto& v = this->_level(this->gy, this->gx - 1);
							if (v == _FSPACE || v == _COIN || v == _HEALTH || v == _BOMB || v == _MINE) {
								if (v == _COIN) {
									this->coins++;
								}
								if (v == _HEALTH) {
									this->heal(random<size>(10, 25));
								}
								if (v == _BOMB) {
									this->bombs++;
								}
								if (v == _MINE) {
									if (this->contains_mine(&v)) {
										this->hit(mine_damage);
									} else {
										this->mines++;
									}
								}
								position = _FSPACE;
								v = _PLAYER;
								this->gx--;
							} else {
								if (v == _EXIT) {
									this->listening = false;
									won = true;
									break;
								}
								if (v == _ENEMY) {
									this->hit(random<size>(15, 25));
								}
							}
							this->update_pinfo();
						}
						break;
					}
					case key::D:
					case key::d: {
						if (this->gx != this->_level.w - 1) {
							auto& v = this->_level(this->gy, this->gx + 1);
							if (v == _FSPACE || v == _COIN || v == _HEALTH || v == _BOMB || v == _MINE) {
								if (v == _COIN) {
									this->coins++;
								}
								if (v == _HEALTH) {
									this->heal(random<size>(10, 25));
								}
								if (v == _BOMB) {
									this->bombs++;
								}
								if (v == _MINE) {
									if (this->contains_mine(&v)) {
										this->hit(mine_damage);
									} else {
										this->mines++;
									}
								}
								position = _FSPACE;
								v = _PLAYER;
								this->gx++;
							} else {
								if (v == _EXIT) {
									this->listening = false;
									won = true;
									break;
								}
								if (v == _ENEMY) {
									this->hit(random<size>(15, 25));
								}
							}
							this->update_pinfo();
						}
						break;
					}
					case key::SPACE: {
						if (this->bombs == 0) {
							break;
						}
						bool got_trigger = false;
						if (this->gy != 0) {
							auto& cell = this->_level.grid[this->gy - 1][this->gx];
							if (cell != _FSPACE && cell != _ENEMY) {
								cell = _FSPACE;
								got_trigger = true;
								io.erase(this->x, this->y - 1);
							}
						}
						if (this->gy != this->_level.h - 1) {
							auto& cell = this->_level.grid[this->gy + 1][this->gx];
							if (cell != _FSPACE && cell != _ENEMY) {
								cell = _FSPACE;
								got_trigger = true;
								io.erase(this->x, this->y + 1);
							}
						}
						if (this->gx != 0) {
							auto& cell = this->_level.grid[this->gy][this->gx - 1];
							if (cell != _FSPACE && cell != _ENEMY) {
								cell = _FSPACE;
								got_trigger = true;
								io.erase(this->x - 1, this->y);
							}
						}
						if (this->gx != this->_level.w - 1) {
							auto& cell = this->_level.grid[this->gy][this->gx + 1];
							if (cell != _FSPACE && cell != _ENEMY) {
								cell = _FSPACE;
								got_trigger = true;
								io.erase(this->x + 1, this->y);
							}
						}
						if (got_trigger) {
							this->bombs--;
						}
						this->update_pinfo();
						break;
					}
					case key::X:
					case key::x: {
						if (this->mines > 0 && position != _MINE) {
							position = _MINE;
							this->known_mines.push_back(&position);
							this->mines--;
							this->update_pinfo();
							this->mine_gx = this->gx;
							this->mine_gy = this->gy;
							this->mine_pending = true;
						}
						break;
					}
				}
			}
			if (this->listening) {
				for (auto& _enemy : (this->_level.enemies)) {
					_enemy.automove();
				}
			}
		}
		if (won) {
			this->free([](auto& i, auto& g, auto& _player) -> void {
				i.render_wininfo(1, 1, g.fwidth - 2, g.fheight - 1);
				io.readk(key::ENTER);
				g.ginfo.erase();
				i.clear(1, 1, g.fwidth - 2, g.fheight - 1);
				_player.pinfo_bombs.erase();
				_player.pinfo_coins.erase();
				_player.pinfo_enemies.erase();
				_player.pinfo_health.erase();
				_player.pinfo_mines.erase();
			});
		}
		if (!this->is_alive()) {
			this->free([](auto& i, auto& g, auto& _player) -> void {
				i.render_loseinfo(1, 1, g.fwidth - 2, g.fheight - 1);
				io.readk(key::ENTER);
				g.ginfo.erase();
				i.clear(1, 1, g.fwidth - 2, g.fheight - 1);
				_player.pinfo_bombs.erase();
				_player.pinfo_coins.erase();
				_player.pinfo_enemies.erase();
				_player.pinfo_health.erase();
				_player.pinfo_mines.erase();
			});
		}
	}
	void player::spawn() {
		this->x = this->shx + this->gx;
		this->y = this->shy + this->gy;
		io << symbols::get_color_for(_PLAYER);
		io.at(this->x, this->y, symbols::parse(_PLAYER));
		io << symbols::color::white;
	}
	void player::update_pinfo() {
		static auto tstr = [](auto v) -> string {
			return std::to_string(v);
		};
		this->pinfo_coins.erase();
		this->pinfo_coins.update(_COINS_TEXT + std::to_string(this->coins) + " / " + std::to_string(_level.coins_generated));
		this->pinfo_health.erase();
		this->pinfo_health.update(_HEALTH_TEXT + std::to_string(this->health));
		this->pinfo_bombs.erase();
		this->pinfo_bombs.update(_BOMBS_TEXT + std::to_string(this->bombs));
		this->pinfo_mines.erase();
		this->pinfo_mines.update(_MINES_TEXT + std::to_string(this->mines));
		this->pinfo_enemies.erase();
		this->pinfo_enemies.update(_ENEMIES_TEXT + std::to_string(this->enemies_killed) + " / " + std::to_string(_level.enemies_generated));
	}
	void player::free(void(*callback)(intf&, game&, player&), bool erase_p, bool erase_d) {
		this->listening = false;
		for (auto& _enemy : (this->_level.enemies)) {
			_enemy.kill();
		}
		if (erase_p) {
			this->pinfo_bombs.erase();
			this->pinfo_coins.erase();
			this->pinfo_enemies.erase();
			this->pinfo_health.erase();
			this->pinfo_mines.erase();
		}
		auto& _g = game::get();
		if (erase_d) {
			_g.ginfo.erase();
		}
		intf::get().clear(1, 1, _g.fwidth - 2, _g.fheight - 1);
		if (callback != nullptr) {
			callback(intf::get(), game::get(), *this);
		}
		_g.free();
	}
	bool player::contains_mine(int* mine) {
		for (auto& v : this->known_mines) {
			if (mine == v) {
				return true;
			}
		}
		return false;
	}
	void player::freeze() {
		this->listening = false;
		auto& _g = game::get();
		intf::get().clear(1, 1, _g.fwidth - 2, _g.fheight - 1);
		_g.use_menu(pmenu);
	}
}