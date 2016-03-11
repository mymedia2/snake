#ifndef SNAKE_H
#define SNAKE_H

#include <iosfwd>
#include <string>
#include <valarray>

namespace game {

class OutputEntity
{
protected:
	OutputEntity()
	{
	}
	OutputEntity(const OutputEntity&)
	{
	}
};

class CharSymbol
	: public OutputEntity
{
public:
	CharSymbol(char& ch)
		: ch_(ch)
	{
	}
	operator char&()
	{
		return ch_;
	}
private:
	char& ch_;
};

class Area
{
private:
	class Indexer {
	public:
		OutputEntity& operator[] (std::size_t col)
		{
			return matrix_.by_indexes(row_, col);
		}
	private:
		Indexer(Area& matrix, std::size_t row)
			: matrix_(matrix)
			, row_(row)
		{
		}
		Area& matrix_;
		std::size_t row_;
		friend class Area;
	};
	friend class Indexer;
protected:
	virtual OutputEntity& by_indexes(std::size_t row, std::size_t col) = 0;
public:
	Indexer operator[] (std::size_t row)
	{
		return Indexer(*this, row);
	}
};

class Console
	: public Area
	//, public Inputer
{
public:
	static const std::size_t screen_rows = 24;
	static const std::size_t screen_cols = 80;
	Console()
		: screen_(' ', screen_rows * screen_cols)
	{
	}
	const Command& get_last_command();
protected:
	virtual CharSymbol& by_indexes(std::size_t row, std::size_t col)
	{
		return screen_[row * screen_cols + col];
	}
private:
	std::valarray<char> screen_;
	std::string last_command_;
	friend std::istream& operator>> (std::istream& in, Console& cl);
	friend std::ostream& operator<< (std::ostream& out, const Console& cl);
};

std::istream& operator>> (std::istream& in, Console& cl);
std::ostream& operator<< (std::ostream& out, const Console& cl);

class Snake
{
public:
	virtual void draw(Area&) = 0;
	std::string name;
};

}	// namespace game

#endif // SNAKE_H
