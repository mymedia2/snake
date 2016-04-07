#include "std.hpp"

#include "objects.hpp"
#include "sys.hpp"
#include "utils.hpp"

#include "io.hpp"

io::Display::Display()
	/* временное решение, желательно взять размеры экрана из окружения */
	: matrix_(rows_ * (columns_ + 1))
{
	clear();
}

void io::Display::set_cell(char value, utils::Point point)
{
	assert(0 <= point.x and point.x < rows_);
	assert(0 <= point.y and point.y < columns_);
	matrix_[point.x * (columns_ + 1) + point.y] = value;
}

void io::Display::clear()
{
	matrix_ = ' ';
	/* Применён некоторый хак: в матрице выделено на один столбец больше, чем размер экрана,
	 * чтобы в этом столбце хранить переводы строк, для упрощения функции вывода */
	matrix_[std::slice(columns_, rows_, columns_ + 1)] = '\n';
}

io::Commander::Commander(game::Snake& slave, int fd, bool disable_echo)
	: commands(std::numeric_limits<unsigned char>::max())
	, slave_(slave)
	, fd_(fd)
{
	commands[static_cast<unsigned char>('w')] = game::Snake::Direction::top;
	commands[static_cast<unsigned char>('a')] = game::Snake::Direction::left;
	commands[static_cast<unsigned char>('s')] = game::Snake::Direction::bottom;
	commands[static_cast<unsigned char>('d')] = game::Snake::Direction::rigth;
	sys::tcgetattr(fd_, &old_termianl_settings_);
	if (disable_echo) {
		sys::termios st = old_termianl_settings_;
		st.c_lflag &= ~ECHO;	// отключает эхо
		st.c_lflag &= ~ICANON;	// перестаёт требовать нажатия Enter для получения ввода
		st.c_cc[VMIN] = 0;	// отключает буферизацию
		sys::tcsetattr(fd_, TCSANOW, &st);
	}
}

io::Commander::~Commander()
{
	/* TODO: очистить входной канал */
	sys::tcsetattr(fd_, TCSANOW, &old_termianl_settings_);
}

void io::Commander::transfer(std::istream& in)
{
	char buf;
	if (sys::read(fd_, &buf, 1) > 0) {
		slave_.execute(commands[static_cast<unsigned char>(buf)]);
	}
}

std::ostream& io::operator<< (std::ostream& out, const Display& area)
{
	// послдений элемент матрицы, в котором лежит перевод строки, не выводится
	std::copy(begin(area.matrix_), end(area.matrix_) - 1, std::ostream_iterator<char>(out));
	return out;
}
