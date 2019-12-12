#include "character.hpp"

Character::Character(const Character& other) :
	MooreMachine(other),
	in_actions(new std::vector<devsim::Port<Action>*>()),
	out_actions(new std::vector<devsim::Port<Action>*>()),
	health_port(new devsim::Port<int>(*other.health_port)),
	external(other.external),
	speed(other.speed),
	next(other.next) {
		std::copy(other.in_actions->begin(), other.in_actions->end(), in_actions->begin());
		std::copy(other.out_actions->begin(), other.out_actions->end(), out_actions->begin());
	}

void Character::lambda() {
	if(external) {
		auto temphp = hitpoints;
		for(auto i : *in_actions) {
			if(i->available()) {
				temphp -= i->peek().value;
				health_port->set(temphp);
			}
		}
	}
	
	if(target >= 0 && target < out_actions->size()) {
		out_actions->at(target)->set(Action {100, EARTH});
	}
}

void Character::delta_int(devsim::TotalTime now) {
	next = speed;
	lastdelta = now;
	external = false;
	for(auto i : *in_actions) {
		if(i->available()) {
			hitpoints -= i->get().value;
		}
	}
}

void Character::delta_ext(devsim::TotalTime now) {
	next = speed - lastdelta.length(now);
	lastdelta = now;
	for(auto i : *in_actions) {
		if(i->available()) {
			external = true;
		}
	}
}

void Character::delta_con(devsim::TotalTime now) {
	next = speed;
	lastdelta = now;
	external = false;
	for(auto i : *in_actions) {
		if(i->available()) {
			hitpoints -= i->get().value;
			external = true;
		}
	}
}

Character& Character::operator=(const Character& other) {
	if(this != &other) {
		MooreMachine::operator=(other);
		in_actions->clear();
		out_actions->clear();
		*health_port = *other.health_port;
		external = other.external;
		speed = other.speed;
		next = other.next;
		std::copy(other.in_actions->begin(), other.in_actions->end(), in_actions->begin());
		std::copy(other.out_actions->begin(), other.out_actions->end(), out_actions->begin());
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, Type const& t) {
	std::string type;
	switch(t) {
		case FIRE:
			type = "Fire";
			break;
		case WATER:
			type = "Water";
			break;
		case EARTH:
			type = "Earth";
			break;
		case AIR:
			type = "Air";
		default:
			break;
	}
	return os << type;
}

std::ostream& operator<<(std::ostream& os, Action const& a) {
	return os << a.type << " " << a.value;
}
