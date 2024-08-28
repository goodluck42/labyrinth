#ifndef __RANDOM__
	#define __RANDOM__
	#include "resource.h"
	#include <random>
	namespace labyrinth {
		template<typename T> T random(T min, T max) {
			static size seed = static_cast<size>(std::time(nullptr));
			static std::mt19937_64 generator;
			std::uniform_int_distribution<T> r(min, max);
			generator.seed(seed++);
			return r(generator);
		}
		template<typename T> T random_of(initializer_list<T> values) {
			static auto seed = static_cast<size>(std::time(nullptr));
			std::default_random_engine generator;
			std::uniform_int_distribution random(0, static_cast<int>(values.size() - 1));
			generator.seed(seed++);
			return *(values.begin() + random(generator));
		}
		template<typename T> T random_of(vector<T> values) {
			static auto seed = static_cast<size>(std::time(nullptr));
			std::default_random_engine generator;
			std::uniform_int_distribution random(0, static_cast<int>(values.size() - 1));
			generator.seed(seed++);
			return *(values.begin() + random(generator));
		}
		bool random();
		bool random(const double prob);
	}
#endif