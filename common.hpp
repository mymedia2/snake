#ifndef COMMON_HPP
#define COMMON_HPP

#include <deque>
#include "game.hpp"
#include "utils.hpp"

namespace common {

/* Общий класс игровых исключений */
class GameException
{
public:
	/* Для удобства делаем объект полиморфным */
	virtual ~GameException();
};

/* Игровой мир, где живут все объекты игры */
class GameWorld
{
public:
	/* Создаёт пустой игровой мир со стенками по краям */
	GameWorld();
	/* Выполняет тик в игре */
	void tick();
	/* Ищет и возвращает объект мира с переданными координатами.
	 * Или в этой точке мира никого нет, возвращает nullptr */
	game::GameObject* who_were(utils::Point);
	/* Делает то же, что и предыдущий метод но для константного объекта */
	const game::GameObject* who_were(utils::Point) const;
private:
	typedef std::deque<game::GameObject> Container_;
	Container_::size_type find_(utils::Point, bool&) const;
	Container_ objects_;
};

}	// namespace common

#endif	// COMMON_HPP
