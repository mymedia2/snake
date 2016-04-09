#include "std.hpp"

#include "common.hpp"
#include "io.hpp"
#include "utils.hpp"

#include "objects.hpp"

game::GameObject::~GameObject()
{
}

game::Snake::Snake(common::GameWorld& world, unsigned speed, const std::string& name)
	: GameObject(world)
	, sceleton_(world.rows, std::vector<Direction>(world.columns))
	, name_(name)
	, speed_(speed)
	, score_(0)
{
	/* FIXME: всё плохо, если нету места, и мы случайно наехали на другие объекты */
	const utils::Point::value_type START_SNAKE_LENGTH = 5;
	// выбор ориентации змеи
	Direction disposition = static_cast<Direction>(std::uniform_int_distribution<char>(
				static_cast<char>(Direction::top), static_cast<char>(Direction::rigth))(world.random));
	where_are_going_ = disposition;
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
	dump_sceleton_(std::clog);
	// рисуем голову
	screen.set_cell(get_head_symbol_(), head_);
	// рисуем всё остальное тело
	utils::Point cursor = head_;
	while (sceleton_[cursor.x][cursor.y] != Direction::none) {
		screen.set_cell(get_body_symbol_(cursor), cursor);
		cursor = move_(cursor);
	}
}

void game::Snake::step()
{
	utils::Point next = head_;
	Direction direct = sceleton_[head_.x][head_.y];
	switch (where_are_going_) {
	case Direction::top:
		next.x--;
		direct = Direction::top;
		break;
	case Direction::bottom:
		next.x++;
		direct = Direction::bottom;
		break;
	case Direction::left:
		next.y--;
		direct = Direction::left;
		break;
	case Direction::rigth:
		next.y++;
		direct = Direction::rigth;
		break;
	default:
		unreachable();
	}
	const GameObject* obj = world.who_were(next);
	if (dynamic_cast<const Wall*>(obj) or dynamic_cast<const Snake*>(obj)) {
		throw Bump();
	} else if (const Food* piece = dynamic_cast<const Food*>(obj)) {
		sceleton_[next.x][next.y] = direct;
		head_ = next;
		increase_(piece->value());
	} else {
		sceleton_[next.x][next.y] = direct;
		head_ = next;
		utils::Point tmp = tail_;
		switch (sceleton_[tail_.x][tail_.y]) {
		case Direction::top:
			tail_.x--;
			break;
		case Direction::bottom:
			tail_.x++;
			break;
		case Direction::left:
			tail_.y--;
			break;
		case Direction::rigth:
			tail_.y++;
			break;
		default:
			//unreachable();
			break;
		}
		sceleton_[tmp.x][tmp.y] = Direction::none;
	}
}

bool game::Snake::are_you_here(utils::Point site) const
{
	return sceleton_[site.x][site.y] != Direction::none;
}

void game::Snake::execute(Direction command)
{
	Direction next = sceleton_[head_.x][head_.y];
	if (command != Direction::none and (next == Direction::top and command != Direction::bottom
				or next == Direction::bottom and command != Direction::top
				or next == Direction::rigth and command != Direction::left
				or next == Direction::left and command != Direction::rigth)) {
		where_are_going_ = command;
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

char game::Snake::get_body_symbol_(utils::Point cursor) const
{
	switch (sceleton_[cursor.x][cursor.y]) {
	case Direction::top:
	case Direction::bottom:
		return '|';
	case Direction::left:
	case Direction::rigth:
		return '-';
	default:
		unreachable();
		return ' ';
	}
}

utils::Point game::Snake::move_(utils::Point cursor) const
{
	switch (sceleton_[cursor.x][cursor.y]) {
	case Direction::top:
		cursor.x++;
		break;
	case Direction::bottom:
		cursor.x--;
		break;
	case Direction::left:
		cursor.y++;
		break;
	case Direction::rigth:
		cursor.y--;
		break;
	default:
		unreachable();
	}
	return cursor;
}

void game::Snake::increase_(unsigned value)
{
	score_ += value;
	world.new_food();
}

utils::Point game::Snake::slide_(utils::Point body_part) const
{
	switch (sceleton_[body_part.x][body_part.y]) {
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

void game::Snake::dump_sceleton_(std::ostream& out) const
{
#ifndef NDEBUG
	for (auto row : sceleton_) {
		for (Direction el : row) {
			out << static_cast<int>(el);
		}
		out << '\n';
	}
	out << std::endl;
#endif
}

game::Food::Food(common::GameWorld& world)
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

game::Wall::Wall(common::GameWorld& world, game::Wall::Position position)
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
