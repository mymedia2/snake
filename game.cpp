#include <cstdlib>
#include <string>
#include "common.hpp"
#include "io.hpp"
#include "utils.hpp"

#include "game.hpp"

game::Snake::Snake(const common::GameWorld& world, unsigned speed, const std::string& name)
	: GameObject(world)
	, name_(name)
	, speed_(speed)
	, score_(0)
{
}

void game::Snake::draw(io::Display& screen) const
{
}

void game::Snake::step()
{
}

void game::Food::draw(io::Display& screen) const
{
}

void game::Food::step()
{
	// еда не ходит :)
}

unsigned game::Food::value() const
{
	return 1;
}
