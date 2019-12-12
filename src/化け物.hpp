#ifndef BAKEMONO_H
#define BAKEMONO_H

#include "character.hpp"

class Bakemono : public Character {
	public:
		Bakemono(int hitpoints, long long speed, int priority) :
			Character(hitpoints, speed, priority) {target = 0;}
		void delta_con(devsim::TotalTime) {}
};

#endif
