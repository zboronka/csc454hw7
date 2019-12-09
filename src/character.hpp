#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <algorithm>

#include "mooremachine.hpp"
#include "port.hpp"

enum Type {
	FIRE,
	WATER,
	EARTH,
	AIR
};

struct Action {
	int value;
	Type type;
}; 

class Character : public devsim::MooreMachine {
	public:
		std::vector<devsim::Port<Action>*>* in_actions;
		std::vector<devsim::Port<Action>*>* out_actions;
		Character() :
			in_actions(new std::vector<devsim::Port<Action>*>()),
			out_actions(new std::vector<devsim::Port<Action>*>()) {}
		Character(const Character&);
		~Character() { delete in_actions; delete out_actions; }
		Character& operator=(const Character&);
};

#endif
