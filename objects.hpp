#ifndef GAME_HPP
#define GAME_HPP

#include "std.hpp"
#include "_forward.hpp"
#include "utils.hpp"

namespace game {

/* Абстрактный класс игрового объекта. Вот примеры объектов, которые
 * могут быть представлены в игре: змейка, еда, бонусы, стенки */
class GameObject
{
public:
	/* Создаёт игровой объект в указанном игровом мире */
	inline explicit GameObject(common::GameWorld& world);
	/* Ничего не делает. Требуется для удаления указателей. */
	virtual ~GameObject();
	/* Должен отрисовывать объект на консоли. Вызывается, когда требуется
	 * нарисовать объект заново, или после тика */
	virtual void draw(io::Display& console) const = 0;
	/* Должен выполнять игровые действия. Вызывается на каждый тик */
	virtual void step() = 0;
	/* Должен возвращать true, если объект считает, что он находится
	 * в указанной точке игрового мира. Иначе false */
	virtual bool are_you_here(utils::Point site) const = 0;
protected:
	/* Ссылка на игровой мир, в котором действует объект */
	common::GameWorld& world;
};

/* Змейка. То, ради чего всё и делается */
class Snake
	: public GameObject
{
public:
	/* Класс исключения столкновения змеи со стеной или с другой змейкой.
	 * Наследует GameException */
	class Bump
		: utils::GameException
	{
	};
	/* Направление движения змейки */
	enum class Direction : char { none, top, bottom, left, rigth, top_left, top_right, bottom_left, bottom_rigth };
	/* Подходящим образом размещает новую змейку в указанном мире */
	explicit Snake(common::GameWorld& world, unsigned speed = 1, const std::string& name = "");
	/* Рисует змейку на консоли */
	virtual void draw(io::Display& console) const override;
	/* Передвигает змейку на шаг вперёд. При столкновении генерирует исключение Bump */
	virtual void step() override;
	/* Проверяет, находится ли хотя бы одна часть змейки в заданной точке */
	virtual bool are_you_here(utils::Point site) const override;
	/* Возвращет имя змейки */
	inline const std::string& name() const;
	/* Возвращает количество очков, набранное змейкой */
	inline unsigned score() const;
	/* Выплняет команду от клавиатуры */
	void execute(Direction command);
private:
	char get_head_symbol_() const;
	char get_body_symbol_(utils::Point) const;
	utils::Point move_(utils::Point) const;
	void increase_(unsigned = 1);
	utils::Point slide_(utils::Point body_part) const;
	void dump_sceleton_(std::ostream&) const;
	std::vector<std::vector<Direction>> sceleton_;
	utils::Point head_;
	utils::Point tail_;
	Direction where_are_going_;
	std::string name_;
	unsigned speed_;
	unsigned score_;
};

/* Еда. Представляет еду, которую есть змейка */
class Food
	: public GameObject
{
public:
	/* Случайным образом размещает новый кусочек еды в указанном мире */
	explicit Food(common::GameWorld& world);
	/* Отрисовывает кусочек еды на консоли */
	virtual void draw(io::Display& console) const override;
	/* Ничего не делает */
	virtual void step() override;
	/* Проверяет, лежит ли еда в точке с заданными координатами */
	virtual bool are_you_here(utils::Point site) const override;
	/* Возвращает количество очков, приносимое этим экземпляром еды */
	virtual unsigned value() const;
private:
	utils::Point position_;
};

/* Стенка в мире */
class Wall
	: public GameObject
{
public:
	/* Пердставляет позицию стенки для убного задания местоположения */
	enum class Position { north, east, south, west };
	/* Создаёт стенку на заданной стороне */
	Wall(common::GameWorld& world, Position position);
	/* Рисует стенку где надо */
	virtual void draw(io::Display& console) const override;
	/* Ничего не делает */
	virtual void step() override;
	/* Находится ли стенка в указанной точке */
	virtual bool are_you_here(utils::Point site) const override;
private:
	Position position_;
};

}	// namespace game

inline game::GameObject::GameObject(common::GameWorld& world)
	: world(world)
{
}

inline const std::string& game::Snake::name() const
{
	return name_;
}

inline unsigned game::Snake::score() const
{
	return score_;
}

#endif // GAME_HPP
