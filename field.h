#ifndef __FIELD__
	#define __FIELD__
	#include "resource.h"
	#include "console.h"
	namespace labyrinth {
		class field final {
			public:
				// xb = X begin
				// yb = Y begin
				// xe = X end
				// ye = Y end
				// cxb = custom X begin
				// cyb = custom Y begin
				field(const coord xb, const coord yb, const coord xe, const coord ye);
				void update(string new_value) const;
				void update(const coord cxb, const coord cyb, string new_value) const;
				void erase() const;
				void erase(const coord cxb, const coord cyb) const;
			private:
				const coord xb;
				const coord yb;
				const coord xe;
				const coord ye;
				size get_string_boundary(const coord cxb, const coord cyb) const;
				bool is_valid_start_boundary(const coord cxb, const coord cyb) const;
		};
	}
#endif
