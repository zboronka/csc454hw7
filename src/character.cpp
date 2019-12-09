#include "character.hpp"

Character::Character(const Character& other) :
	MooreMachine(other),
	in_actions(new std::vector<devsim::Port<Action>*>()),
	out_actions(new std::vector<devsim::Port<Action>*>())
{
	std::copy(other.in_actions->begin(), other.in_actions->end(), in_actions->begin());
	std::copy(other.out_actions->begin(), other.out_actions->end(), out_actions->begin());
}

Character& Character::operator=(const Character& other) {
	if(this != &other) {
		MooreMachine::operator=(other);
		in_actions->clear();
		out_actions->clear();
		std::copy(other.in_actions->begin(), other.in_actions->end(), in_actions->begin());
		std::copy(other.out_actions->begin(), other.out_actions->end(), out_actions->begin());
	}

	return *this;
}
