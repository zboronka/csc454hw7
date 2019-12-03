#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "mooremachine.hpp"

enum Type {
	FIRE,
	WATER,
	EARTH,
	AIR
};

struct Attack {
	int value;
	Type type;
};

class Character : public devsim::MooreMachine {
};

#endif
