#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"

class Player : public Character {
	public:
		devsim::Port<int>* input;
		Player(int hitpoints, long long speed, int priority) :
			Character(hitpoints, speed, priority),
			input(new devsim::Port<int>()) {}
		Player(const Player&);
		~Player() { delete input; }
		Player& operator=(const Player&);

		void lambda();

		void delta_int(devsim::TotalTime);
		void delta_ext(devsim::TotalTime);
		void delta_con(devsim::TotalTime) {}
};

#endif
