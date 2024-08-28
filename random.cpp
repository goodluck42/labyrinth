#include "random.h"

namespace labyrinth {
	bool random() {
		return random(0.5);
	}
	bool random(const double prob) {
		static auto seed = static_cast<size>(std::time(nullptr));
		std::default_random_engine generator;
		std::uniform_int_distribution<int> random(0, 100);
		generator.seed(seed++);
		return prob > (static_cast<double>(random(generator)) / 100);
	}
}