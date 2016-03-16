#include <cstddef>
#include <cassert>
#include "io.hpp"

#include "game.hpp"

namespace {

// TODO: сделать это методом Display, а Coordinates переместить в отдельный модуль
void _plot_line(io::Display& screen, game::Coordinates<std::size_t> from, game::Coordinates<std::size_t> to)
{
	if (from.x == to.x) {	// рисуем минусами
		int step = from.y < to.y ? +1 : -1;
		for (std::size_t i = from.y; i != to.y; i += step) {
			screen.set_cell('-', from.x, i);
		}
	} else if (from.y == to.y) {	// рисуем вертикальными чёрточками
		int step = from.x < to.x ? +1 : -1;
		for (std::size_t i = from.x; i != to.y; i += step) {
			screen.set_cell('|', i, from.y);
		}
	} else if ((from.x - to.x) * (from.y - to.y) > 0) {	// рисуем в первой или третьей "четверти"
		int step = from.x < to.x ? +1 : -1;
		std::size_t i, j;
		for (i = from.x, j = from.y; i != to.x and j != to.y; i++, j++) {
			screen.set_cell('/', i, j);
		}
		assert(i == to.x and j == to.y);
	} else if ((from.x - to.x) * (from.y - to.y) < 0) {	// рисуем во второй или четвёртой "четверти"
		// TODO: объединить с предыдущей веткой
		int step = from.x < to.x ? +1 : -1;
		std::size_t i, j;
		for (i = from.x, j = from.y; i != to.x and j != to.y; i++, j++) {
			screen.set_cell('\\', i, j);
		}
		assert(i == to.x and j == to.y);
	} else {
		assert("Unreachable");
	}
}

}	// namespace

void game::Snake::draw(io::Display& screen) const
{
	for (std::size_t i = 0; i < sceleton_.size() - 1; i++) {	// рисуем линии скелета
		_plot_line(screen, sceleton_[i], sceleton_[i + 1]);
	}
	screen.set_cell('*', sceleton_[0].x, sceleton_[0].y);	// рисуем голову змеи
	for (std::size_t i = 1; i < sceleton_.size(); i++) {	// рисуем соединения линий
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
