#ifndef __FIELD_RECT__
	#define __FIELD_RECT__
	#include "resource.h"
	#include "console.h"
	namespace labyrinth {
		class field_rect final {
			public:
				field_rect(const coord xb, const coord yb, const coord width, const coord height);
				void update(string new_value) const;
				void update(const coord cxb, const coord cyb, string new_value) const;
				void erase() const;
				void erase(const coord cxb, const coord cyb) const;
			private:
				const coord xb;
				const coord yb;
				const coord xe;
				const coord ye;
				const coord width;
				const coord height;
				size get_string_boundary(const coord cxb, const coord cyb) const;
				bool is_valid_start_boundary(const coord cxb, const coord cyb) const;
				vector<string> split_string(string value, const coord cxb, const coord cyb) const;
		};
	}
#endif
