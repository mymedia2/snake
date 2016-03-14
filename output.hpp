#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <cstddef>
#include <iosfwd>
#include <valarray>

namespace output {

class Display
{
public:
	Display();
	void set_cell(char value, std::size_t row, std::size_t column);
private:
	std::valarray<char> matrix_;
};

std::ostream& operator<< (std::ostream& out, const Display& area);

}	// namespace output

#endif	// OUTPUT_HPP
