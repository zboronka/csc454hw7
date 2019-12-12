#include "player.hpp"
#include <iostream>

Player::Player(const Player& other) :
	Character(other),
	input(new devsim::Port<int>(*other.input)) {}

Player& Player::operator=(const Player& other) {
	if(this != &other) {
		Character::operator=(other);
		*input = *other.input;
	}

	return *this;
}

void Player::lambda() {
	Character::lambda();
}

void Player::delta_int(devsim::TotalTime now) {
	Character::delta_int(now);
	target = -1;
}

void Player::delta_ext(devsim::TotalTime now) {
	Character::delta_ext(now);
	if(input->available()) {
		target = input->get();
		external = true;
	}
}
