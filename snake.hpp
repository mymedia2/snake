#ifndef SNAKE_H
#define SNAKE_H

#include <iosfwd>
#include <string>
#include <valarray>

namespace game {

class Snake
{
public:
	virtual void draw(std::ostream&) const = 0;
	std::string name;
private:
	std::valarray<char> screen_;
};

inline std::ostream& operator<< (std::ostream& out, const Snake& obj)
{
	obj.draw(out);
	return out;
}

class Python
	: public Snake
{
public:
	virtual void draw(std::ostream&) const override;
};

class Cobra
	: public Snake
{
public:
	virtual void draw(std::ostream&) const override;
};

class Mamba
	: public Snake
{
public:
	virtual void draw(std::ostream&) const override;
};

class Anaconda
	: public Snake
{
public:
	virtual void draw(std::ostream&) const override;
};

}	// namespace game

#endif // SNAKE_H
