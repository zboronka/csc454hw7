#include "mooremachine.hpp"

namespace devsim {
	MooreMachine::MooreMachine(int priority) :
		lastdelta(TotalTime()),
		priority(priority),
		internal(new Event()) {}

	MooreMachine::MooreMachine() :
		lastdelta(TotalTime()),
		internal(new Event()) {}

	MooreMachine::MooreMachine(const MooreMachine& other) :
		priority(other.priority),
		lastdelta(other.lastdelta),
		internal(new Event()) 
	{
		*internal = *other.internal;
	}

	MooreMachine::~MooreMachine() {
		delete internal;
	}

	MooreMachine& MooreMachine::operator=(const MooreMachine& other) {
		if(this != &other) {
			priority = other.priority;
			lastdelta = other.lastdelta;
			*internal = *other.internal;
		}

		return *this;
	}

	void MooreMachine::set_internal(Event internal) {
		*this->internal = internal;
	}

	std::ostream& operator<<(std::ostream& strm, MooreMachine const& m) {
		return strm << m.insertion();
	}

	bool operator<(MooreMachine const& a, MooreMachine const& b) {
		return a.priority < b.priority;
	}

	bool operator>(MooreMachine const& a, MooreMachine const& b) {
		return a.priority > b.priority;
	}
	bool operator==(MooreMachine const& a, MooreMachine const& b) {
		return a.priority == b.priority;
	}
}
