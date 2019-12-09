#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include <regex>

#include "port.hpp"
#include "pipe.hpp"
#include "totaltime.hpp"
#include "event.hpp"
#include "colors.hpp"
#include "player.hpp"

int main() {
	Player a = Player(4, 1);
	a.input->link();
	Player b = Player(a);
	std::vector<Player> v = std::vector<Player>();
	//v.push_back(c);
}
