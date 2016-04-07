#ifndef COMMON_HPP
#define COMMON_HPP

#include "std.hpp"
#include "objects.hpp"
#include "io.hpp"
#include "utils.hpp"

namespace common {

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
	/* Вызывает метод draw для каждого игрового объекта */
	void draw(io::Display& console) const;
	/* размещает в мире новую змею и еду для неё */
	game::Snake& new_snake(const std::string& name = "", unsigned level = 1);
	/* размещает в мире новую порцию еды */
	game::Food& new_food();
	/* Генератор случайности */
	mutable /* FIXME: убрать mutable */ std::default_random_engine random;
	/* Размеры мира */
	static const unsigned rows = 25;
	static const unsigned columns = 80;
private:
	typedef std::deque<std::unique_ptr<game::GameObject>> Container_;
	Container_::size_type find_(utils::Point, bool&) const;
	Container_ objects_;
};

}	// namespace common

#endif	// COMMON_HPP
