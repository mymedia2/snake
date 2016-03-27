#include <iomanip>
#include <iostream>
#include "common.hpp"
#include "game.hpp"
#include "io.hpp"

int main()
{
	io::Display screen;
	common::GameWorld world;
	game::Snake liver(world);
	world.draw(screen);
	std::cout << screen << std::endl;
}
