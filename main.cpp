#include "std.hpp"

#include "common.hpp"
#include "game.hpp"
#include "io.hpp"

int main()
{
	io::Display screen;
	common::GameWorld world;
	world.new_snake();
	world.draw(screen);
	std::cout << io::CLEAR_SCREEN << screen << std::endl;
	for (;;) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//world.tick();
		world.new_snake();
		world.draw(screen);
		std::cout << io::MOVE_CURSOR_TO_TOP_LEFT << screen << std::endl;
	}
}
