#include "level.h"

namespace labyrinth {
	level::level(const size w, const size h): w(w), h(h), is_grid_generated(false), is_coins_generated(false), coins_generated(0), enemies_generated(0) {
		this->grid.resize(h);
		for (auto& col : this->grid) {
			col.resize(w);
			for (auto& v : col) {
				v = _FSPACE;
			}
		}
	}
	level::level(const size w, const size h, vector<vector<int>>& grid): w(w), h(h), is_grid_generated(true), is_coins_generated(false), coins_generated(0), enemies_generated(0), grid(grid) { }
	void level::generate() {
		if (this->is_grid_generated) {
			return;
		}
		auto breakacell_x = [this](const size xb, const size xe, const size y) -> void {
			do {
				auto r = random<size>(xb, xe);
				if (this->grid[y - 1][r] != _BLOCK && this->grid[y + 1][r] != _BLOCK) {
					this->grid[y][r] = _FSPACE;
					break;
				}
			} while(true);
		};
		auto breakacell_y = [this](const size yb, const size ye, const size x) -> void {
			do {
				auto r = random<size>(yb, ye);
				if (this->grid[r][x + 1] != _BLOCK && this->grid[r][x - 1]) {
					this->grid[r][x] = _FSPACE;
					break;
				}
			} while(true);
		};
		for (size x = 0; x <= 54; x += 18) {
			for (size y = 0; y <= 24; y += 4) {
				this->generate2(x, y, (this->w / 4) - 1, (this->h / 4) - 1);
			}
		}
		for (double d = 0.75; d > 0; d -= 0.25) { 
			this->generate2((this->w * d) - 1, 0, 1, this->h - 2);
		}
		for (size i = 0; i < 3; i++) {
			this->generate2(this->w - i - 1, 0, 1, this->h - 1);
		}
 		this->generate2(this->w - 3, 0, 1, this->h - 2);
 		this->generate2(0, this->h - 2, this->w - 2, 0);
 		this->generate2(0, this->h - 1, this->w, 0);
		breakacell_y(this->h / 2, this->h - 1 - 2, this->w / 2 - 1); //Main Bottom
		breakacell_x(0, (this->w / 2) - 1 - 1, this->h / 2 - 1); //Main Left
		breakacell_x(this->w / 2, this->w - 1 - 3, this->h / 2 - 1); //Main Right
		////////////
		breakacell_y(this->h * 0.25, this->h / 2 - 2, 17); //Sub LeftTop Bottom 
		breakacell_x(0, this->w * 0.25 - 2, 7); //Sub LeftTop Left
		breakacell_x(this->w * 0.25, this->w / 2 - 2, 7); //Sub LeftTop Right
		////////////
		breakacell_y(this->h * 0.25, this->h / 2 - 2, 53); //Sub RightTop Bottom 
		breakacell_x(this->w / 2, this->w / 2 * 1.5 - 2, 7); //Sub RightTop Left
		breakacell_x(this->w / 2 * 1.5, this->w - 1 - 3, 7); //Sub RightTop Right
		////////////
		breakacell_y(this->h / 2 * 1.5, this->h - 1 - 2, 17); //Sub LeftBottom Bottom 
		breakacell_x(0, this->w * 0.25 - 2, 23); //Sub LeftBottom Left
		breakacell_x(this->w * 0.25, this->w / 2 - 2, 23); //Sub LeftBottom Right
		////////////
		breakacell_y(this->h / 2 * 1.5, this->h - 1 - 2, 53); //Sub RightBottom Bottom
		breakacell_x(this->w / 2, this->w / 2 * 1.5 - 2, 23); //Sub RightBottom Left
		breakacell_x(this->w / 2 * 1.5, this->w - 1 - 3, 23); //Sub RightBottom Right
		////////////
		breakacell_y(this->h * 0.25 * 0.5, this->h * 0.25 * 0.5 + 2, 8); //1 Crossway (Left Top)
		breakacell_x(0, this->w * 0.25 * 0.5 - 2, 3);
		breakacell_x(this->w * 0.25 * 0.5, this->w * 0.25 * 0.90, 3);
		////////////
		breakacell_y(this->h * 0.25 * 0.5, this->h * 0.25 * 0.5 + 2, 26); //2 Crossway
		breakacell_x(this->w * 0.25 * 0.90 + 2, this->w / 2 * 0.75 - 2, 3);
		breakacell_x(this->w / 2 * 0.75, this->w / 2 - 2, 3);
		////////////
		breakacell_y(this->h * 0.4 - 1, this->h * 0.5 - 2, 8); //3 Crossway
		breakacell_x(0, this->w * 0.25 * 0.5 - 2, 11);
		breakacell_x(this->w * 0.25 * 0.5, this->w * 0.25 * 0.90, 11);
		////////////
		breakacell_y(this->h * 0.4 - 1, this->h * 0.5 - 2, 26); //4 Crossway
		breakacell_x(this->w * 0.25 * 0.90 + 2, this->w / 2 * 0.75 - 2, 11);
		breakacell_x(this->w / 2 * 0.75, this->w / 2 - 2, 11);
		////////////
		breakacell_y(this->h * 0.25 * 0.5, this->h * 0.25 * 0.5 + 2, 44); //5 Crossway (Right Top)
		breakacell_x(this->w * 0.5, this->w * 0.6, 3);
		breakacell_x(this->w * 0.6 + 2, this->w * 0.70 + 1, 3);
		////////////
		breakacell_y(this->h * 0.25 * 0.5, this->h * 0.25 * 0.5 + 2, 62); //6 Crossway 
		breakacell_x(this->w * 0.75, this->w * 0.85 - 1, 3);
		breakacell_x(this->w * 0.85 + 1, this->w - 1 - 3, 3);
		////////////
		breakacell_y(this->h * 0.40 - 1, this->h * 0.40 + 1, 44); //7 Crossway
		breakacell_x(this->w * 0.5, this->w * 0.6, 11);
		breakacell_x(this->w * 0.6 + 2, this->w * 0.70 + 1, 11);
		////////////
		breakacell_y(this->h * 0.40 - 1, this->h * 0.40 + 1, 62); //8 Crossway 
		breakacell_x(this->w * 0.75, this->w * 0.85 - 1, 11);
		breakacell_x(this->w * 0.85 + 1, this->w - 1 - 3, 11);
		///////////
		breakacell_y(this->h / 2 * 1.25, this->h / 2 * 1.4, 8); //9 Crossway (Bottom Left)
		breakacell_x(0, this->h / 2 * 0.5 - 1, 19);
		breakacell_x(this->h / 2 * 0.5 + 1, this->h / 2, 19);
		///////////
		breakacell_y(this->h / 2 * 1.25, this->h / 2 * 1.4, 26); //10 Crossway
		breakacell_x(this->w * 0.25 * 0.90 + 2, this->w / 2 * 0.75 - 2, 19);
		breakacell_x(this->w / 2 * 0.75, this->w / 2 - 2, 19);
		///////////
		breakacell_y(this->h / 2 * 1.75, this->h - 3, 8); //11 Crossway
		breakacell_x(0, this->h / 2 * 0.5 - 1, 27);
		breakacell_x(this->h / 2 * 0.5 + 1, this->h / 2, 27);
		///////////
		breakacell_y(this->h / 2 * 1.75, this->h - 3, 26); //12 Crossway
		breakacell_x(this->w * 0.25 * 0.90 + 2, this->w / 2 * 0.75 - 2, 27);
		breakacell_x(this->w / 2 * 0.75, this->w / 2 - 2, 27);
		///////////
		breakacell_y(this->h / 2 * 1.25, this->h / 2 * 1.4, 44); //13 Crossway (Bottom Right)
		breakacell_x(this->w * 0.5, this->w * 0.6, 19);
		breakacell_x(this->w * 0.6 + 2, this->w * 0.70 + 1, 19);
		///////////
		breakacell_y(this->h / 2 * 1.25, this->h / 2 * 1.4, 62);//14 Crossway
		breakacell_x(this->w * 0.75, this->w * 0.85 - 1, 19);
		breakacell_x(this->w * 0.85 + 1, this->w - 1 - 3, 19);
		///////////
		breakacell_y(this->h / 2 * 1.75, this->h - 3, 44); //15 Crossway
		breakacell_x(this->w * 0.5, this->w * 0.6, 27);
		breakacell_x(this->w * 0.6 + 2, this->w * 0.70 + 1, 27);
		///////////
		breakacell_y(this->h / 2 * 1.75, this->h - 3, 62); //16 Crossway
		breakacell_x(this->w * 0.75, this->w * 0.85 - 1, 27);
		breakacell_x(this->w * 0.85 + 1, this->w - 1 - 3, 27);
		this->is_grid_generated = true;
		auto gblocks_idxs = [this](const size y) -> vector<size> {
			vector<size> result;
			size index = 0;
			for (auto& v : this->grid[y]) {
				if (v == _BLOCK) {
					result.push_back(index);
				}
				index++;
			}
			return result;
		};
		auto mbreakacell_x = [this](const size xb, const size xe, const size y) -> void {
			if (xb == xe) {
				this->grid[y][xe] = _FSPACE;
				return;
			}
			do {
				auto r = random<size>(xb, xe);
				if (this->grid[y][r] != _FSPACE) {
					this->grid[y][r] = _FSPACE;
					break;
				}
			} while(true);
		};
		auto checkr = [this](const size x, const size y) -> bool {
			return this->grid[y][x + 1] != _FSPACE;
		};
		auto genr = [this](const size xb, const size xe, const size y) -> void {
			for (size x = xb; x < xe; x++) {
				if (this->grid[y][x] == _FSPACE) {
					if (this->grid[y + 1][x] == _BLOCK && this->grid[y - 1][x] == _BLOCK) {
						if (x > 0) {
							this->grid[y][x - 1] = _FSPACE;
							this->grid[y][x + 1] = _FSPACE;
							x++;
						}
						continue;
					}
					this->grid[y][x] = _BLOCK;
				}
			}
		};
		for (size y = 1; y < this->h - 3; y += 4) {
			vector<size> idxs = gblocks_idxs(y);
			size p = 0;
			for (auto& bx : idxs) {
				if (bx == idxs[idxs.size() - 2]) {
					break;
				}
				genr(p, bx, y);
				if (checkr(bx + 1, y)) {
					mbreakacell_x(p + 1, bx - 1, y);
				} else {
					mbreakacell_x(bx - 1, bx - 1, y);
				}
				p = bx;
			}
		}
		this->raw_grid = grid;
	}
	void level::generate(const size xb, const size yb, const size w, const size h) {
		if (w <= 2 || h <= 2) {
			return;
		}
		size dvw = w / 2;
		size dvh = h / 2;
		size nx = xb + dvw;
		size ny = yb + dvh;
		size x_i = (dvw / 2 == 0 ? 1 : 0) + xb;
		size y_i = (dvh / 2 == 0 ? 1 : 0) + yb;
		size x_lim = (dvw / 2 == 0 ? 1 : 0) + xb + w;
		size y_lim = (dvh / 2 == 0 ? 1 : 0) + yb + h;
		for (; y_i < y_lim; y_i++) {
			this->grid[y_i][nx] = _BLOCK;
		} 
		for (; x_i < x_lim; x_i++) {
			this->grid[ny][x_i] = _BLOCK;
		}
		this->grid[ny][random(xb + 1, xb + dvw - 1)] = _FSPACE; //LEFT HOLE
		this->grid[ny][random(xb + dvw + 1, xb + w - 1)] = _FSPACE; //RIGHT HOLE
		this->grid[random(yb + dvh + 1, yb + h - 1)][nx] = _FSPACE; //BOTTOM HOLE
		this->generate(xb, yb, dvw, dvh);
		//this->generate(xb + dvw, yb, dvw, dvh);
		//this->generate(xb, yb + dvh, dvw, dvh);
		//this->generate(xb + dvw, yb + dvh, dvw, dvh);
	}
	void level::generate2(const size xb, const size yb, const size w, const size h) {
		const size dw = w / 2;
		const size dh = (h / 2);
		const size nx = xb + dw;
		const size ny = yb + dh;
		if (h != 0) {
			for (size y = yb; y < yb + h; y++) {
				this->grid[y][nx] = _BLOCK;
			}
		}
		if (w != 0) {
			for (size x = xb; x < xb + w; x++) {
				this->grid[ny][x] = _BLOCK;
			}
		}
	}
	int& level::operator()(const size r, const size c) {
		return this->grid[r][c];
	}
	void level::generate_coins() {
		if (this->is_grid_generated && !this->is_coins_generated) {
			for (size i = 0; i < this->h; i++) {
				for (size j = 0; j < this->w; j++) {
					if (random(0.02) && this->grid[i][j] == _FSPACE) {
						this->grid[i][j] = _COIN;
						this->coins_generated++;
					}
				}
			}
			this->is_coins_generated = true;
		}
	}
	void level::generate_enemies(const size shx, const size shy, const size count, player& _player) {
		size generated = 0;
		for (size i = this->h * 0.1; i < this->h; i++) {
			bool flag = false;
			for (size j = this->w * 0.1; j < this->w; j++) {
				if (random(0.02) && this->grid[i][j] == _FSPACE) {
					this->grid[i][j] = _ENEMY;
					this->enemies.push_back(enemy(&_player, shx, shy, j, i, this->grid));
					this->enemies_generated++;
					generated++;
				}
				if (generated == count) {
					flag = true;
					break;
				}
			}
			if (flag) {
				return;
			}
		}
	}
	void level::spawn_enemies() {
		for (size i = 0; i < this->enemies.size(); i++) {
			this->enemies[i].spawn();
			this->enemies[i].automove();
		}
	}
	void level::spawn_exit() {
		static auto get_rnd_exit = [](level& lv) -> auto {
			return random_of<size>({lv.h - 3, lv.h - 5, 2, 0});
		};
		this->grid[get_rnd_exit(*this)][this->w - 4] = _EXIT;
	}
	void level::generate_aidkits(const size count) {
		size generated = 0;
		for (size i = 0; i < this->h; i++) {
			bool flag = false;
			for (size j = 0; j < this->w; j++) {
				if (random(0.01) && random(0.5) && this->grid[i][j] == _FSPACE) {
					this->grid[i][j] = _HEALTH;
					generated++;
				}
				if (generated == count) {
					flag = true;
					break;
				}
			}
			if (flag) {
				return;
			}
		}
	}
	void level::generate_bombs(const size count) {
		size generated = 0;
		for (size i = 0; i < this->h; i++) {
			bool flag = false;
			for (size j = 0; j < this->w; j++) {
				if (random(0.01) && this->grid[i][j] == _FSPACE) {
					this->grid[i][j] = _BOMB;
					generated++;
				}
				if (generated == count) {
					flag = true;
					break;
				}
			}
			if (flag) {
				return;
			}
		}
	}
	void level::generate_mines(const size count) {
		size generated = 0;
		for (size i = 0; i < this->h; i++) {
			bool flag = false;
			for (size j = 0; j < this->w; j++) {
				if (random(0.01) && random(0.5) && this->grid[i][j] == _FSPACE) {
					this->grid[i][j] = _MINE;
					generated++;
				}
				if (generated == count) {
					flag = true;
					break;
				}
			}
			if (flag) {
				return;
			}
		}
	}
}