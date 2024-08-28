#include "field_rect.h"

namespace labyrinth {
	field_rect::field_rect(const coord xb, const coord yb, const coord width, const coord height): xb(xb), yb(yb), xe(xb + width - 1), ye(yb + height - 1), width(width), height(height) {}
	void field_rect::update(string new_value) const {
		auto lim = this->get_string_boundary(this->xb, this->yb);
		if (new_value.size() > lim) {
			new_value.erase(lim);
		}
		io.svcpos();
		auto _out = this->split_string(new_value, this->xb, this->yb);
		for (size i = 0; i < _out.size(); i++) {
			io.scpos(this->xb, static_cast<coord>(this->yb + i));
			io << _out[i];
		}
		io.rcpos();
	}
	void field_rect::update(const coord cxb, const coord cyb, string new_value) const {
		if (!this->is_valid_start_boundary(cxb, cyb)) {
			return;
		}
		auto lim = this->get_string_boundary(cxb, cyb);
		if (new_value.size() > lim) {
			new_value.erase(lim);
		}
		io.svcpos();
		auto _out = this->split_string(new_value, cxb, cyb);
		for (size i = 0; i < _out.size(); i++) {
			io.scpos(i == 0 ? cxb : this->xb, static_cast<coord>(cyb + i));
			io << _out[i];
		}
		io.rcpos();
	}
	void field_rect::erase() const {
		io.svcpos();
		for (coord i = 0; i < this->height; i++) {
			io.erase(this->xb, i + this->yb, static_cast<size>(this->width));
		}
		io.rcpos();
	}
	void field_rect::erase(const coord cxb, const coord cyb) const {
		io.svcpos();
		coord i = cyb;
		io.erase(cxb, i, cxb - this->xb - 1);
		i++;
		for (; i <= this->ye; i++) {
			io.erase(this->xb, i, this->width);
		}
		io.rcpos();
	}
	size field_rect::get_string_boundary(const coord cxb, const coord cyb) const {
		if (cxb == this->xb && cyb == this->yb) {
			return (this->width * this->height);
		}
		return (this->xe - cxb) + ((this->ye - cyb) * this->width) + 1;
	}
	bool field_rect::is_valid_start_boundary(const coord cxb, const coord cyb) const {
		if (cyb > this->ye || cyb < this->yb) {
			return false;
		}
		if (cxb > this->xe || cxb < this->xb) {
			return false;
		}
		return true;
	}
	vector<string> field_rect::split_string(string value, const coord cxb, const coord cyb) const {
		vector<string> res;
		const auto end = value.end();
		auto it = value.begin();
		auto last_it = value.begin();
		coord x = cxb == this->xb ? 0 : cxb - this->xb;
		coord y = cyb == this->yb ? 0 : cyb - this->yb;
		if (value.size() / this->width - (cxb - this->xb) == 0) {
			res.push_back(value);
		} else {
			while (it != end  && y < this->height) {
				if (x % this->width == 0 && x != 0) {
					string part;
					part.assign(last_it, it);
					last_it = it;
					res.push_back(part);
					y++;
				}
				it++;
				x++;
			}
			if (last_it != it && y < this->height) {
				string part;
				part.assign(last_it, end);
				res.push_back(part);
			}
		}
		return res;
	}
}