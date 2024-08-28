#include "console.h"
#include "resource.h"
#include "random.h"
#include "game.h"

int main() {
	using namespace labyrinth;
	auto& g = game::get();
	g.init();
	return 0;
}
