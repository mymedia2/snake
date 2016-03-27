#include <iomanip>
#include <iostream>
#include "common.hpp"
#include "game.hpp"
#include "io.hpp"

int main()
{
	io::Display screen;
	common::GameWorld world;
	world.new_snake();
	world.draw(screen);
	std::cout << screen << std::endl;
}
