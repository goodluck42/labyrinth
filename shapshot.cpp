#include "snapshot.h"
#include <fstream>
namespace labyrinth {
	vector<mapdata> maps;
	vector<string> split(string str, string separator) {
		size pos = 0;
		std::string token;
		vector<string> result;
		while ((pos = str.find(separator)) != std::string::npos) {
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + separator.length());
		}
		result.push_back(str);
		return result;
	}
	string join(vector<string> values, string separator) {
		string res;
		size i = 0;
		for (auto v : values) {
			res += v;
			if (i != values.size() - 1) {
				res += separator;
			}
			i++;
		}
		return res;
	}
	const string filename = "snaps.txt";
	string get_rawsnapshot(const string mapname) {
		std::ifstream file;
		vector<string> lines;
		string line;
		bool found = false;
		file.open(filename);
		if (file.is_open()) {
			while (std::getline(file, line)) {
				if (found) {
					return line;
				}
				auto s = split(line, " ");
				if (s.size() == 2 && s[0] == "$mapname" && s[1] == mapname) {
					found = true;
					continue;
				}
			}
			file.close();
		}
		return string("NOT");
	}
	vector<vector<int>> parsesnap(string snap, size* w, size* h) {
		vector<vector<int>> map;
		const auto rows = split(snap, ":");
		if (h != nullptr) {
			*h = rows.size();
		}
		bool width_defined = false;
		size width = 0;
		for (auto& r : rows) {
			const auto values = split(r, " ");
			vector<int> row;
			for (auto& v : values) {
				if (v == "$map") {
					continue;
				}
				const auto cnt = split(v, "x");
				const int smb = cnt[0] == "0" ? _FSPACE : _BLOCK;
				const size length = std::atoll(cnt[1].c_str());
				for (size i = 0; i < length; i++) {
					row.push_back(smb);
					if (!width_defined) {
						width++;
					}
				}
			}
			if (!width_defined) {
				width_defined = true;
				if (w != nullptr) {
					*w = width;
				}
			}
			map.push_back(row);
		}
		return map;
	}
	bool snapshot(vector<vector<int>>& raw_level, const string mapname, bool push) {
		if (push) {
			mapdata newmap;
			newmap.name = mapname;
			newmap.grid = raw_level;
			newmap.h = raw_level.size();
			newmap.w = raw_level[0].size();
			maps.push_back(newmap);
		}
		std::ofstream file;
		file.open(filename, std::ios::app);
		if (file.is_open()) {
			file << "$mapname " << mapname << '\n';
			file << "$map ";
			for (size i = 0; i < raw_level.size(); i++) {
				size spaces = 0;
				size blocks = 0;
				for (size j = 0; j < raw_level[i].size(); j++) {
					auto& v = raw_level[i][j];
					if (v == _FSPACE) {
						if (blocks > 0) {
							file << "1x" << blocks << ' ';
							blocks = 0;
						}
						spaces++;
						continue;
					}
					if (v == _BLOCK) {
						if (spaces > 0) {
							file << "0x" << spaces << ' ';
							spaces = 0;
						}
						blocks++;
					}
				}
				if (spaces != 0) {
					file << "0x" << spaces;
				}
				if (blocks != 0) {
					file << "1x" << blocks ;
				}
				if (i != raw_level.size() - 1) {
					file << ':';
				}
			}
			file << '\n' << "$end" << '\n';
			file.close();
			return true;
		}
		return false;
	}
	void fetch_data() {
		std::ifstream file;
		string line;
		maps.clear();
		file.open(filename);
		if (file.is_open()) {
			size i = 0;
			string p_mapname = "undefined";
			mapdata data;
			while (std::getline(file, line)) {
				if (i == 12) {
					break;
				}
				auto v = split(line, " ");
				if (v[0] == "$mapname") {
					p_mapname = v[1];
					data.name = v[1];
					continue;
				}
				if (v[0] == "$map") {
					data.grid = parsesnap(get_rawsnapshot(p_mapname), &data.w, &data.h);
					continue;
				}
				if (v[0] == "$end") {
					maps.push_back(data);
					i++;
				}
			}
			file.close();
		}
	}
	bool is_map_exists(string mapname) {
		for (auto& v : maps) {
			if (v.name == mapname) {
				return true;
			}
		}
		return false;
	}
	void savedata() {
		std::ofstream ofs;
		ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
		if (ofs.is_open()) {
			ofs.close();
			for (auto& m : maps) {
				snapshot(m.grid, m.name, false);
			}
		}
	}
	void removemap(const string mapname) {
		size it = 0;
		for (auto& m : maps) {
			if (m.name == mapname) {
				maps.erase(maps.begin() + it);
				return;
			}
			it++;
		}
	}
}