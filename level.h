#ifndef __LEVEL__
	#define __LEVEL__
	#include "resource.h"
	#include "console.h"
	#include "random.h"
	#include "enemy.h"
	namespace labyrinth {
		class level {
			friend class player;
			friend class enemy;
			public:
				level() = delete;
				level(const size w, const size h);
				level(const size w, const size h, vector<vector<int>>& grid);
				void generate();
				const size w;
				const size h;
				int& operator()(const size r, const size c);
				void generate_coins();
				void generate_enemies(const size shx, const size shy, const size count, player& _player);
				void spawn_enemies();
				void spawn_exit();
				void generate_aidkits(const size count);
				void generate_bombs(const size count);
				void generate_mines(const size count);
				vector<enemy> enemies;
				vector<vector<int>> raw_grid;
			private:
				bool is_grid_generated;
				bool is_coins_generated;
				size coins_generated;
				size enemies_generated;
				void generate(const size xb, const size yb, const size w, const size h);
				void generate2(const size xb, const size yb, const size w, const size h);
				vector<vector<int>> grid;
				
		};
	}
#endif 
