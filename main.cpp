#include "std.hpp"

#include "common.hpp"
#include "objects.hpp"
#include "io.hpp"

int main()
try {
	io::Display screen;
	common::GameWorld world;
	io::Commander keyboard(world.new_snake());
	world.draw(screen);
	std::cout << io::CLEAR_SCREEN << screen << std::endl;
	for (;;) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		keyboard.transfer(std::cin);
		world.tick();
		screen.clear();
		world.draw(screen);
		std::cout << io::MOVE_CURSOR_TO_TOP_LEFT << screen << std::endl;
	}
} catch (game::Snake::Bump&) {
	std::cout << "GAME OVER" << std::endl;
}
