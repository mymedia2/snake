#ifndef IO_HPP
#define IO_HPP

#include "std.hpp"
#include "_forward.hpp"
#include "objects.hpp"
#include "sys.hpp"
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

/* Класс-абстрация клавиатуры */
class Commander
{
public:
	/* Указывает входной поток и управляемую змейку. Также отключает ввод с
	 * эхом и буферизацию, если было запрошено, и если это возможно */
	explicit Commander(game::Snake& slave, int in_fd = 0, bool set_console = true);
	/* Обратно включает эхо при вводе и буферизацию */
	~Commander();
	/* Извлекает и интерпретирует ввод и, если надо, передаёт команду змейке */
	void transfer(std::istream& in);
	/* Команды */
	std::vector<game::Snake::Direction> commands;
private:
	game::Snake& slave_;
	int fd_;
	sys::termios old_termianl_settings_;
};

/* Выводит всё, что нарисовано на консоли в поток вывода */
std::ostream& operator<< (std::ostream& out, const Display& area);

/* Константы для управлением курсором на экране */
const char* const CLEAR_SCREEN = "\x1b[2J\x1b[H\r";
const char* const MOVE_CURSOR_TO_TOP_LEFT = CLEAR_SCREEN + 4;

}	// namespace io

#endif	// IO_HPP
