#include "std.hpp"

#include "game.hpp"
#include "utils.hpp"

#include "common.hpp"

common::GameException::~GameException()
{
}

common::GameWorld::GameWorld()
	: objects_(4)	// TODO: если будет поддерживаться в будущих версиях С++, инициализировать через initializator_list
#ifdef NDEBUG
	, random(std::chrono::system_clock::now().time_since_epoch().count())
#endif
{
	objects_[0] = std::make_unique<game::Wall>(*this, game::Wall::Position::north);
	objects_[1] = std::make_unique<game::Wall>(*this, game::Wall::Position::east);
	objects_[2] = std::make_unique<game::Wall>(*this, game::Wall::Position::south);
	objects_[3] = std::make_unique<game::Wall>(*this, game::Wall::Position::west);
}

void common::GameWorld::tick()
{
	for (auto& obj : objects_) {
		obj->step();
	}
}

game::GameObject* common::GameWorld::who_were(utils::Point site)
{
	bool success;
	auto index = find_(site, success);
	return success ? objects_[index].get() : nullptr;
}

const game::GameObject* common::GameWorld::who_were(utils::Point site) const
{
	bool success;
	auto index = find_(site, success);
	return success ? objects_[index].get() : nullptr;
}

void common::GameWorld::draw(io::Display& console) const
{
	for (auto& ptr : objects_) {
		ptr->draw(console);
	}
}

game::Snake& common::GameWorld::new_snake(const std::string& name, unsigned level)
{
	game::Snake* obj = new game::Snake(*this, level, name);
	objects_.push_back(std::unique_ptr<game::Snake>(obj));
	new_food();
	return *obj;
}

game::Food& common::GameWorld::new_food()
{
	game::Food* obj = new game::Food(*this);
	objects_.push_back(std::unique_ptr<game::Food>(obj));
	return *obj;
}

common::GameWorld::Container_::size_type common::GameWorld::find_(utils::Point site, bool& success) const
{
	for (common::GameWorld::Container_::size_type i = 0; i < objects_.size(); i++) {
		if (objects_[i]->are_you_here(site)) {
			success = true;
			return i;
		}
	}
	return success = false;
}
