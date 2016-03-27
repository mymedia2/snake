#include <cstdlib>
#include <string>
#include "io.hpp"
#include "utils.hpp"

#include "game.hpp"

namespace {

// TODO: сделать это методом Display, а Coordinates переместить в отдельный модуль
void _plot_line(io::Display& screen, utils::Point from, utils::Point to)
{
	const char ch = from.x == to.x ? '-' : from.y == to.y ? '|' :
		(from.x - to.x) * (from.y - to.y) < 0 ? '/' : '\\';
	const int step_x = from.x == to.x ? 0 : -(from.x - to.x) / std::abs(from.x - to.x);
	const int step_y = from.y == to.y ? 0 : -(from.y - to.y) / std::abs(from.y - to.y);
	std::size_t i, j;
	for (i = from.x, j = from.y; i != to.x and j != to.y; i += step_x, j += step_y) {
		screen.set_cell(ch, i, j);
	}
	assert(i == to.x and j == to.y);
}

}	// namespace

game::GameException::~GameException()
{
}

game::Snake::Snake(const game::GameWorld& world, unsigned speed, const std::string& name)
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
