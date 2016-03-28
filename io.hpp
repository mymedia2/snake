#ifndef IO_HPP
#define IO_HPP

#include "std.hpp"

#include "utils.hpp"

namespace io {

/* Класс-абстракция консоли */
class Display
{
	friend std::ostream& operator<< (std::ostream& out, const Display& area);
public:
	/* Создаёт новый объект консоли, вычисляя необходимые размеры */
	Display();
	/* Устанавливает в точке заданное значение */
	void set_cell(char value, utils::Point point);
private:
	std::valarray<char> matrix_;
	static const std::size_t rows_ = 25;
	static const std::size_t columns_ = 80;
};

/* Выводит всё, что нарисовано на консоли в поток вывода */
std::ostream& operator<< (std::ostream& out, const Display& area);

}	// namespace io

#endif	// IO_HPP
