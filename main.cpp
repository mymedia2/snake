#include <iostream>
#include "game.hpp"
#include "io.hpp"

int main()
{
	io::Display screen;
	game::Snake gamers[] = { {{1, 1}, {4, 4}}, {{2, 6}, {2, 3}}, {{5, 10}, {10, 10}}, {{9, 0}, {0, 9}} };
	for (game::Snake& anilmal : gamers) {
		anilmal.draw(screen);
	}
	std::cout << screen << std::endl;
}
