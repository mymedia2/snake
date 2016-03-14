#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include "output.hpp"

namespace game {

template <typename T>
struct Coordinates
{
	typedef T value_type;
	T x = 0;
	T y = 0;
};

class GameObject
{
public:
	virtual void draw(output::Display&) const = 0;
	virtual void step() = 0;
};

class Snake
	: public GameObject
{
public:
	virtual void draw(output::Display&) const override;
	virtual void step() override;
	std::string name;
private:
	std::vector<Coordinates<unsigned>> chain_;
};

class Food
	: public GameObject
{
public:
	virtual void draw(output::Display&) const override;
	virtual void step() override;
private:
	Coordinates<unsigned> position;
};

}	// namespace game

#endif // GAME_HPP
