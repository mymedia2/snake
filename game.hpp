#ifndef GAME_HPP
#define GAME_HPP

#include <exception>
#include <string>
#include "common.fwd"
#include "io.hpp"
#include "utils.hpp"

namespace game {

/* Абстрактный класс игрового объекта. Вот примеры объектов, которые
 * могут быть представлены в игре: змейка, еда, бонусы, стенки */
class GameObject
{
public:
	/* Создаёт игровой объект в указанном игровом мире */
	inline GameObject(const common::GameWorld& world);
	/* Должен отрисовывать объект на консоли. Вызывается, когда требуется
	 * нарисовать объект заново, или после тика */
	virtual void draw(io::Display& console) const = 0;
	/* Должен выполнять игровые действия. Вызывается на каждый тик */
	virtual void step() = 0;
protected:
	/* Ссылка на игровой мир, в котором действует объект */
	const common::GameWorld& world;
};

/* Змейка. То, ради чего всё и делается */
class Snake
	: public GameObject
{
public:
	/* Класс исключения столкновения змеи со стеной или с другой змейкой.
	 * Наследует GameException */
	class Bump;
	/* Подходящим образом размещает новую змейку в указанном мире */
	Snake(const common::GameWorld& world, unsigned speed, const std::string& name = "");
	/* Рисует змейку на консоли */
	virtual void draw(io::Display& console) const override;
	/* Передвигает змейку на шаг вперёд. При столкновении генерирует исключение Bump */
	virtual void step() override;
	/* Возвращет имя змейки */
	inline const std::string& name() const;
	/* Возвращает количество очков, набранное змейкой */
	inline unsigned score() const;
private:
	void increase_(unsigned = 1);
	void slide_(unsigned = 1);
	std::vector<bool> sceleton_;
	utils::Point direction_;
	utils::Point head_;
	utils::Point tail_;
	std::string name_;
	unsigned speed_;
	unsigned score_;
};

/* Еда. Представляет еду, которую есть змейка */
class Food
	: public GameObject
{
public:
	/* Случайным образом размещает новую кусочек еды в указанном мире */
	Food(const common::GameWorld&);
	/* Отрисовывает кусочек еды на консоли */
	virtual void draw(io::Display&) const override;
	/* Ничего не делает */
	virtual void step() override;
	/* Возвращает количество очков, приносимое этим экземпляром еды */
	virtual unsigned value() const;
private:
	utils::Point position;
};

}	// namespace game

inline game::GameObject::GameObject(const common::GameWorld& world)
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
