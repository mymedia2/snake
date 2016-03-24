#include <cassert>
#include <cstddef>
#include <cstdlib>
#include "io.hpp"

#include "game.hpp"

namespace {

// TODO: сделать это методом Display, а Coordinates переместить в отдельный модуль
void _plot_line(io::Display& screen, game::Coordinates<std::size_t> from, game::Coordinates<std::size_t> to)
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

game::Snake::Snake(Coordinates<std::size_t> head, Coordinates<std::size_t> tail)
	: sceleton_({head, tail})
{
}

void game::Snake::draw(io::Display& screen) const
{
	for (std::size_t i = 0; i < sceleton_.size() - 1; i++) {	// рисуем линии скелета
		_plot_line(screen, sceleton_[i], sceleton_[i + 1]);
	}
	screen.set_cell('*', sceleton_[0].x, sceleton_[0].y);	// рисуем голову змеи
	for (std::size_t i = 1; i < sceleton_.size() - 1; i++) {	// рисуем соединения линий
		screen.set_cell('+', sceleton_[i].x, sceleton_[i].y);
	}
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
