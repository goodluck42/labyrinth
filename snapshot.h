#ifndef __SPANSHOT__
	#define __SPANSHOT__
	#include "resource.h"
	namespace labyrinth {
		struct mapdata {
			vector<vector<int>> grid;
			size w = 0;
			size h = 0;
			string name;
		};
		extern vector<mapdata> maps;
		extern const string filename;
		bool snapshot(vector<vector<int>>& raw_level, const string mapname, bool push = true);
		string get_rawsnapshot(const string mapname);
		vector<vector<int>> parsesnap(string snap, size* w, size* h);
		bool is_map_exists(string mapname);
		void fetch_data();
		void savedata();
		void savedata();
		void removemap(const string filename);
	}
#endif
