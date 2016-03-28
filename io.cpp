#include "std.hpp"

#include "utils.hpp"

#include "io.hpp"

io::Display::Display()
	: matrix_(' ', rows_ * (columns_ + 1))	// временное решение, желательно взять размеры экрана из окружения
{
	// Применён некоторый хак: в матрице выделено на один столбец больше, чем размер экрана,
	// чтобы в этом столбце хранить переводы строк, для упрощения функции вывода
	matrix_[std::slice(columns_, rows_, columns_ + 1)] = '\n';
}

void io::Display::set_cell(char value, utils::Point point)
{
	assert(0 <= point.x and point.x < rows_);
	assert(0 <= point.y and point.y < columns_);
	matrix_[point.x * (columns_ + 1) + point.y] = value;
}

std::ostream& io::operator<< (std::ostream& out, const Display& area)
{
	// послдений элемент матрицы, в котором лежит перевод строки, не выводится
	std::copy(begin(area.matrix_), end(area.matrix_) - 1, std::ostream_iterator<char>(out));
	return out;
}
