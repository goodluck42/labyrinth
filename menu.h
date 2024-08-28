#ifndef __MENU__
	#define __MENU__
	#include "resource.h"
	#include "console.h"
	#include "snapshot.h"
	namespace labyrinth {
		class menu {
			friend class game;
			public:
				enum class alignment {
					center,
					left
				};
				class row {
					public:
						typedef void(*cb)();
						typedef void(*cb2)(mapdata* d_ptr);
						row() = delete;
						row(const string text, const string rgrapple, const string lgrapple, cb callback);
						row(mapdata* data, const string rgrapple, const string lgrapple, cb2 callback);
						mapdata* data;
						const string text;
						const cb callback;
						const cb2 callback2;
						coord xb;
						coord xe;
						coord y;
						const string lgrapple;
						const string rgrapple;
						coord lgrapple_x;
						coord rgrapple_x;
				};
				menu() = delete;
				menu(const size width, const size height, vector<row>& rows, alignment align = alignment::center);
				void listen(bool handle_esc = false);
				void free();
				vector<row> rows;
				const size width;
				const size height;
				alignment align;
				size index;
			private:
				const size mindex;
				bool listening;
		};
	}
#endif
