#ifndef __DLOGGER__
	#define __DLOGGER__
	#include <sstream>
	#include <fstream>
	#include <filesystem>
	#include "resource.h"
	namespace debug {
		using namespace labyrinth;
		template<typename T> string to_string(T value) {
			std::stringstream stream;
			stream << value;
			return stream.str();
		}
		template<> string to_string(char v);
		template<> string to_string(bool v);
		string get_current_directory();
		class dlogger final {
			public:
			dlogger();
			~dlogger();
			dlogger(string path, string filename);
			template<typename T> dlogger& operator<<(T _) {
				string __ = to_string<T>(_);
				if (this->file.is_open()) {
					this->file.write(__.c_str(), __.size());
				}
				return *this;
			}
			template<> dlogger& operator<<(string _) {
				if (this->file.is_open()) {
					this->file.write(_.c_str(), _.size());
				}
				return *this;
			}
			template<> dlogger& operator<<(const char* _) {
				return *this << string(_);
			}
			private:
			const string path;
			string fullpath;
			std::fstream file;
		};
	}
#endif