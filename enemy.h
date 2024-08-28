#ifndef __ENEMY__
	#define __ENEMY__
	#include "resource.h"
	#include "console.h"
	#include "random.h"
	namespace labyrinth {
		class player;
		class enemy {
			public:
				enum class direction {
					none,
					top,
					right,
					bottom,
					left
				};
				enemy() = delete;
				enemy(player* _player, const size shx, const size shy, const size gx, const size gy, vector<vector<int>>& grid);
				void automove();
				void updatepos();
				void kill();
				void spawn();
				bool is_alive();
				coord x;
				coord y;
			private:
				direction player_dir;
				const size shx;
				const size shy;
				bool alive;
				vector<vector<int>>& grid;
				size gx;
				size gy;
				size px;
				size py;
				direction pdir;
				player* _player;
				bool trigger_kill;
				size player_hit;
		};
	}
#endif // !__ENEMY__
