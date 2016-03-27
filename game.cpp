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

bool game::Food::are_you_here(utils::Point site) const
{
	return false;
}

unsigned game::Food::value() const
{
	return 1;
}

game::Wall::Wall(const common::GameWorld& world, game::Wall::Position position)
	: GameObject(world)
	, position_(position)
{
}

void game::Wall::draw(io::Display& console) const
{
	// TODO: упростить код
	switch (position_) {
	case Position::north:
		for (unsigned j = 1; j < world.columns - 1; j++) {
			console.set_cell('-', utils::Point(0, j));
		}
		console.set_cell('+', utils::Point(0, 0));
		console.set_cell('+', utils::Point(0, world.columns - 1));
		break;
	case Position::east:
		for (unsigned i = 1; i < world.rows - 1; i++) {
			console.set_cell('|', utils::Point(i, world.columns - 1));
		}
		console.set_cell('+', utils::Point(0, world.columns - 1));
		console.set_cell('+', utils::Point(world.rows - 1, world.columns - 1));
		break;
	case Position::south:
		for (unsigned j = 1; j < world.columns - 1; j++) {
			console.set_cell('-', utils::Point(world.rows - 1, j));
		}
		console.set_cell('+', utils::Point(world.rows - 1, 0));
		console.set_cell('+', utils::Point(world.rows - 1, world.columns - 1));
		break;
	case Position::west:
		for (unsigned i = 1; i < world.rows - 1; i++) {
			console.set_cell('|', utils::Point(i, 0));
		}
		console.set_cell('+', utils::Point(0, 0));
		console.set_cell('+', utils::Point(world.rows - 1, 0));
		break;
	default:
		unreachable();
	}
}

void game::Wall::step()
{
	// стенки не двигаются
}

bool game::Wall::are_you_here(utils::Point site) const
{
	return position_ == Position::north and site.x == 0
	    or position_ == Position::east and site.y == world.columns - 1
		or position_ == Position::south and site.x == world.rows - 1
		or position_ == Position::west and site.y == 0;
}
