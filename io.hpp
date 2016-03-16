#ifndef IO_HPP
#define IO_HPP

#include <cstddef>
#include <iosfwd>
#include <valarray>

namespace io {

class Display
{
public:
	Display();
	void set_cell(char value, std::size_t row, std::size_t column);
private:
	std::valarray<char> matrix_;
	static const std::size_t rows_ = 25;
	static const std::size_t columns_ = 80;

	friend std::ostream& operator<< (std::ostream& out, const Display& area);
};

std::ostream& operator<< (std::ostream& out, const Display& area);

}	// namespace io

#endif	// IO_HPP
