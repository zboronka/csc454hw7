#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <algorithm>
#include <vector>

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

std::ostream& operator<<(std::ostream&, Type const&);
std::ostream& operator<<(std::ostream&, Action const&);

class Character : public devsim::MooreMachine {
	protected:
		long long speed;
		long long next;
		int hitpoints;
		int target;
		bool external;
		Character(int hitpoints, long long speed, int priority) :
			MooreMachine(priority),
			in_actions(new std::vector<devsim::Port<Action>*>()),
			out_actions(new std::vector<devsim::Port<Action>*>()),
			health_port(new devsim::Port<int>()),
			speed(speed),
			next(speed),
			hitpoints(hitpoints),
			target(-1),
			external(false) {}
		Character(const Character&);

	public:
		std::vector<devsim::Port<Action>*>* in_actions;
		std::vector<devsim::Port<Action>*>* out_actions;
		devsim::Port<int>* health_port;
		~Character();
		Character& operator=(const Character&);

		long long ta() { return hitpoints > 0 ? external ? 0 : next : -1; }

		void lambda();

		void delta_int(devsim::TotalTime);
		void delta_ext(devsim::TotalTime);
		void delta_con(devsim::TotalTime);
};

#endif
