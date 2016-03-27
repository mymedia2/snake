#include <algorithm>
#include "game.hpp"
#include "utils.hpp"

#include "common.hpp"

common::GameException::~GameException()
{
}

// FIXME: возможно выбрасывание исключения без очистки памяти
common::GameWorld::GameWorld()
	: objects_({ new game::Wall(*this, game::Wall::Position::north),
	             new game::Wall(*this, game::Wall::Position::east),
	             new game::Wall(*this, game::Wall::Position::south),
	             new game::Wall(*this, game::Wall::Position::west) })
{
}

// TODO: переделать с использованием unique_ptr
common::GameWorld::~GameWorld()
{
	for (game::GameObject* ptr : objects_) {
		delete ptr;
	}
}

game::GameObject* common::GameWorld::who_were(utils::Point site)
{
	bool success;
	auto index = find_(site, success);
	return success ? objects_[index] : nullptr;
}

const game::GameObject* common::GameWorld::who_were(utils::Point site) const
{
	bool success;
	auto index = find_(site, success);
	return success ? objects_[index] : nullptr;
}

void common::GameWorld::draw(io::Display& console) const
{
	for (game::GameObject* ptr : objects_) {
		ptr->draw(console);
	}
}

common::GameWorld::Container_::size_type common::GameWorld::find_(utils::Point site, bool& success) const
{
	Container_::const_iterator iter = std::find_if(objects_.begin(), objects_.end(),
			[site](const game::GameObject* obj) { return obj->are_you_here(site); });
	success = iter != objects_.end();
	return iter - objects_.begin();
}
