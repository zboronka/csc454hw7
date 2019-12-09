#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"

class Player : public Character {
	private:
		long long speed;
		long long elapse;
	public:
		devsim::Port<std::string>* input;
		Player(long long speed, int priority):
			input(new devsim::Port<std::string>()),
			speed(speed) { this->set_priority(priority); }
		Player(const Player&);
		~Player() { delete input; }
		Player& operator=(const Player&);
		long long ta() { return speed - elapse; }

		void lambda() {}

		void delta_int(devsim::TotalTime) {}
		void delta_ext(devsim::TotalTime) {}
		void delta_con(devsim::TotalTime) {}
};

#endif
