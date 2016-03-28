#include <cstdlib>
#include <string>
#include "common.hpp"
#include "io.hpp"
#include "utils.hpp"

#include "game.hpp"

game::GameObject::~GameObject()
{
}

game::Snake::Snake(const common::GameWorld& world, unsigned speed, const std::string& name)
	: GameObject(world)
	, sceleton_(world.rows, std::vector<Direction_>(world.columns))
	, name_(name)
	, speed_(speed)
	, score_(0)
{
	for (std::size_t j = 10; j < 15; j++) {
		sceleton_[10][j] = Direction_::rigth;
	}
	head_ = utils::Point(10, 14);
	tail_ = utils::Point(10, 10);
}

void game::Snake::draw(io::Display& screen) const
{
	char ch = ' ';
	// рисуем голову
	switch (sceleton_[head_.x][head_.y]) {
	case Direction_::top:
		ch = '^';
		break;
	case Direction_::bottom:
		ch = 'v';
		break;
	case Direction_::rigth:
		ch = '>';
		break;
	case Direction_::left:
		ch = '<';
		break;
	default:
		unreachable();
	}
	assert(ch != ' ');
	screen.set_cell(ch, head_);
	// рисуем всё остальное тело
	utils::Point cursor = head_;
	while (cursor != tail_) {
		switch (sceleton_[cursor.x][cursor.y]) {
		case Direction_::top:
			ch = '|';
			cursor.x--;
			break;
		case Direction_::bottom:
			ch = '|';
			cursor.x++;
			break;
		case Direction_::rigth:
			ch = '-';
			cursor.y--;
			break;
		case Direction_::left:
			ch = '-';
			cursor.y++;
			break;
		default:
			unreachable();
		}
		assert(ch != ' ');
		screen.set_cell(ch, cursor);
	}
}

void game::Snake::step()
{
}

bool game::Snake::are_you_here(utils::Point site) const
{
	return sceleton_[site.x][site.y] != Direction_::none;
}

game::Food::Food(const common::GameWorld& world)
	: GameObject(world)
	, position_(utils::Point(std::rand() % (world.rows - 2) + 1, std::rand() % (world.columns - 2) + 1))	// TODO
{
}

void game::Food::draw(io::Display& screen) const
{
	screen.set_cell('.', position_);
}

void game::Food::step()
{
	// еда не ходит :)
}

bool game::Food::are_you_here(utils::Point site) const
{
	return site == position_;
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
