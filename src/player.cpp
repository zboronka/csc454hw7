#include "player.hpp"

Player::Player(const Player& other) :
	Character(other),
	speed(other.speed),
	elapse(other.elapse),
	input(new devsim::Port<std::string>(*other.input)) {}

Player& Player::operator=(const Player& other) {
	if(this != &other) {
		Character::operator=(other);
		speed = other.speed;
		elapse = other.elapse;
		*input = *other.input;
	}

	return *this;
}
