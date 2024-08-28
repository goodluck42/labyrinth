#include "field.h"

namespace labyrinth {
	field::field(const coord xb, const coord yb, const coord xe, const coord ye): xb(xb), yb(yb), xe(xe), ye(ye) {}
	void field::update(string new_value) const {
		auto lim = this->get_string_boundary(this->xb, this->yb);
		if (new_value.size() > lim) {
			new_value.erase(lim);
		}
		io.svcpos();
		io.scpos(this->xb, this->yb);
		io << new_value;
		io.rcpos();
	}
	void field::update(const coord cxb, const coord cyb, string new_value) const {
		if (!this->is_valid_start_boundary(cxb, cyb)) {
			return;
		}
		auto lim = this->get_string_boundary(cxb, cyb);
		if (new_value.size() > lim) {
			new_value.erase(lim);
		}
		io.svcpos();
		io.scpos(cxb, cyb);
		io << new_value;
		io.rcpos();
	}
	void field::erase() const {
		io.erase(this->xb, this->yb, this->get_string_boundary(this->xb, this->yb));
	}
	void field::erase(const coord cxb, const coord cyb) const {
		if (!this->is_valid_start_boundary(cxb, cyb)) {
			return;
		}
		io.erase(cxb, cyb, this->get_string_boundary(cxb, cyb));
	}
	size field::get_string_boundary(const coord cxb, const coord cyb) const {
		coord xbnd;
		io.gbnd(&xbnd, nullptr);
		if (cyb == this->ye) {
			if (cxb == this->xe) {
				return 1;
			}
			return this->xe - cxb;
		}
		return (xbnd - cxb) + ((std::abs(cyb - this->ye) - 1) * xbnd) + (this->xe + 1);
	}
	bool field::is_valid_start_boundary(const coord cxb, const coord cyb) const {
		if (cyb < this->yb || cyb > this->ye) {
			return false;
		}
		if ((cyb == this->yb && cxb < this->xb) || cyb == this->ye && cxb > this->xe) {
			return false;
		}
		return true;
	}
}