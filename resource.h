#ifndef __RESOURCE__
	#define __RESOURCE__
	#include <vector>
	#include <string>
	#include <map>
	#include <initializer_list>
	#include <ctime>
	#include <algorithm>
	#include <thread>
	#include <future>
	#include <chrono>
	#define _COIN 111
	#define _BLOCK 35
	#define _FSPACE 32
	#define _MINE 234
	#define _ENEMY 88
	#define _PLAYER 64
	#define _HEALTH 43
	#define _BOMB 42
	#define _EXIT 254
	namespace labyrinth {
		using std::string;
		using std::vector;
		using std::map;
		using std::thread;
		using std::initializer_list;
		using std::pair;
		typedef size_t size;
		enum class key {
			BACKSPACE = 8,
			TAB = 9,
			ENTER = 13,
			ESCAPE = 27,
			SPACE = 32,
			_0 = 48, _1, _2, _3, _4, _5, _6, _7, _8, _9,
			A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
		};
		namespace symbols {
			template<typename T> string parse(T s) {
				return string(1, static_cast<char>(s));
			}
			enum class color : unsigned int {
				navy = 1,
				green,
				blue,
				firebrick,
				violet,
				gold,
				white,
				gray,
				blueV2,
				greenV2,
				cyan,
				red,
				violetV2,
				yellow,
				whiteV2
			};
			color get_color_for(const int s);
			enum class border {
				elbow_tr = 201,
				elbow_tl = 187,
				elbow_br = 200,
				elbow_bl = 188,
				tpose_t = 202,
				tpose_r = 204,
				tpose_b = 203,
				tpose_l = 185,
				crossway = 206,
				horizontal = 205,
				vertical = 186,
			};
			enum class block  {
				right = 222,
				left = 221,
				bold = 219,
			};
		}
	}
#endif