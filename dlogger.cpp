#include "dlogger.h"

namespace debug {
	template<> string to_string(char v) {
		string _;
		_.push_back(v);
		return _;
	}
	template<> string to_string(bool v) {
		return v ? "true" : "false";
	}
	string get_current_directory() {
		std::wstring path = std::filesystem::current_path().c_str();
		return string(path.begin(), path.end());
	}
	dlogger::dlogger(): dlogger(get_current_directory(), "debug.txt") {}
	dlogger::~dlogger() {
		if (this->file.is_open()) {
			this->file.close();
		}
	}
	dlogger::dlogger(string path, string filename): path(path) {
		this->fullpath = path + "\\" + filename;
		this->file = std::fstream();
		this->file.open(this->fullpath, std::ios::in | std::ios::app);
	}
}