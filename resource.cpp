#include "resource.h"
namespace labyrinth {
	symbols::color symbols::get_color_for(const int s) {
		switch (s) {
			case _HEALTH: {
				return color::green;
			}
			case _COIN: {
				return color::gold;
			}
			case _BOMB: {
				return color::firebrick;
			}
			case _ENEMY: {
				return color::red;
			}
			case _EXIT: {
				return color::green;
			}
			case _PLAYER: {
				return color::yellow;
			}
			case _MINE: {
				return color::violetV2;
			}
			default: {
				return color::whiteV2;
			}
		}
	}
}
