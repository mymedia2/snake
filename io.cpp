#include <algorithm>
#include <cstddef>
#include <iterator>
#include <valarray>

#include "io.hpp"

io::Display::Display()
	: matrix_(' ', rows_ * (columns_ + 1))	// временное решение, желательно взять размеры экрана из окружения
{
	// Применён некоторый хак: в матрице выделено на один столбец больше, чем размер экрана,
	// чтобы в этом столбце хранить переводы строк, для упрощения функции вывода
	matrix_[std::slice(columns_, rows_, columns_ + 1)] = '\n';
}

void io::Display::set_cell(char value, std::size_t row, std::size_t column)
{
	matrix_[row * (columns_ + 1) + column] = value;
}

std::ostream& io::operator<< (std::ostream& out, const Display& area)
{
	// послдений элемент матрицы, в котором лежит перевод строки, не выводится
	std::copy(begin(area.matrix_), end(area.matrix_) - 1, std::ostream_iterator<char>(out));
	return out;
}
