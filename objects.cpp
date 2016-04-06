#include "std.hpp"

#include "common.hpp"
#include "io.hpp"
#include "utils.hpp"

#include "objects.hpp"

game::GameObject::~GameObject()
{
}

game::Snake::Snake(const common::GameWorld& world, unsigned speed, const std::string& name)
	: GameObject(world)
	, sceleton_(world.rows, std::vector<Direction>(world.columns))
	, name_(name)
	, speed_(speed)
	, score_(0)
{
	const utils::Point::value_type START_SNAKE_LENGTH = 5;
	// выбор ориентации змеи
	Direction disposition = static_cast<Direction>(std::uniform_int_distribution<char>(
				static_cast<char>(Direction::top), static_cast<char>(Direction::rigth))(world.random));
	// установка головы змеи
	head_ = utils::Point(std::uniform_int_distribution<utils::Point::value_type>(
					1 + START_SNAKE_LENGTH, world.rows - 2 - START_SNAKE_LENGTH)(world.random),
	            std::uniform_int_distribution<utils::Point::value_type>(
					1 + START_SNAKE_LENGTH, world.columns - 2 - START_SNAKE_LENGTH)(world.random));
	// заполняем скелет
	for (utils::Point::value_type i = 0; i < START_SNAKE_LENGTH; i++) {
		if (disposition == Direction::top) {
			sceleton_[head_.x + i][head_.y] = disposition;
		} else if (disposition == Direction::bottom) {
			sceleton_[head_.x - i][head_.y] = disposition;
		} else if (disposition == Direction::left) {
			sceleton_[head_.x][head_.y + i] = disposition;
		} else if (disposition == Direction::rigth) {
			sceleton_[head_.x][head_.y - i] = disposition;
		} else {
			unreachable();
		}
	}
	// устанавливаем хвост
	if (disposition == Direction::top) {
		tail_ = utils::Point(head_.x + START_SNAKE_LENGTH - 1, head_.y);
	} else if (disposition == Direction::bottom) {
		tail_ = utils::Point(head_.x - START_SNAKE_LENGTH + 1, head_.y);
	} else if (disposition == Direction::left) {
		tail_ = utils::Point(head_.x, head_.y + START_SNAKE_LENGTH - 1);
	} else if (disposition == Direction::rigth) {
		tail_ = utils::Point(head_.x, head_.y - START_SNAKE_LENGTH + 1);
	} else {
		unreachable();
	}
}

void game::Snake::draw(io::Display& screen) const
{
	// рисуем голову
	screen.set_cell(get_head_symbol_(), head_);
	// рисуем всё остальное тело
	utils::Point cursor = head_;
	while (cursor != tail_) {
		screen.set_cell(get_body_symbol_and_move_(cursor), cursor);
	}
}

void game::Snake::step()
{
	utils::Point next = slide_(head_);
	const GameObject* obj = world.who_were(next);
	if (const Food* piece = dynamic_cast<const Food*>(obj)) {
		sceleton_[next.x][next.y] = sceleton_[head_.x][head_.y];
		head_ = next;
		increase_(piece->value());
		return;
	}
	if (obj) {
		throw Bump();
	}
	sceleton_[next.x][next.y] = sceleton_[head_.x][head_.y];
	head_ = next;
	sceleton_[tail_.x][tail_.y] = Direction::none;
	tail_ = slide_(tail_);
}

bool game::Snake::are_you_here(utils::Point site) const
{
	return sceleton_[site.x][site.y] != Direction::none;
}

void game::Snake::execute(Direction command)
{
	if (command != Direction::none) {
		/* FIXME: запретить змее разворачиваться */
		sceleton_[head_.x][head_.y] = command;
	}
}

char game::Snake::get_head_symbol_() const
{
	switch (sceleton_[head_.x][head_.y]) {
	case Direction::top:
		return '^';
	case Direction::bottom:
		return 'v';
	case Direction::left:
		return '<';
	case Direction::rigth:
		return '>';
	default:
		unreachable();
		return ' ';
	}
}

char game::Snake::get_body_symbol_and_move_(utils::Point& cursor) const
{
	switch (sceleton_[cursor.x][cursor.y]) {
	case Direction::top:
		cursor.x++;
		return '|';
	case Direction::bottom:
		cursor.x--;
		return '|';
	case Direction::left:
		cursor.y++;
		return '-';
	case Direction::rigth:
		cursor.y--;
		return '-';
	default:
		unreachable();
		return ' ';
	}
}

void game::Snake::increase_(unsigned)
{
}

utils::Point game::Snake::slide_(utils::Point body_part)
{
	switch (sceleton_[head_.x][head_.y]) {
	case Direction::top:
		body_part.x--;
		break;
	case Direction::bottom:
		body_part.x++;
		break;
	case Direction::left:
		body_part.y--;
		break;
	case Direction::rigth:
		body_part.y++;
		break;
	default:
		unreachable();
	}
	return body_part;
}

game::Food::Food(const common::GameWorld& world)
	: GameObject(world)
	, position_(std::uniform_int_distribution<utils::Point::value_type>(1, world.rows - 2)(world.random),
	            std::uniform_int_distribution<utils::Point::value_type>(1, world.columns - 2)(world.random))
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
