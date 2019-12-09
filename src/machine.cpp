#include "machine.hpp"

Machine::Machine(const Machine& other):
	MooreMachine(other),
	p(other.p),
	s(other.s),
	t(other.t),
	input(new devsim::Port<int>(*other.input)),
	output(new devsim::Port<int>(*other.output)) {}

Machine& Machine::operator=(const Machine& other) {
	if(this != &other) {
		MooreMachine::operator=(other);
		p = other.p;
		s = other.s;
		t = other.t;
		*input = *other.input;
		*output = *other.output;
	}

	return *this;
}

void Machine::delta_int(devsim::TotalTime now) {
	p -= 1;
	s = t;
	lastdelta = now;
}

void Machine::delta_ext(devsim::TotalTime now) { 
	s = p == 0 ? t : s - lastdelta.length(now); 
	p += input->get();
	lastdelta = now; 
} 

void Machine::delta_con(devsim::TotalTime now) {
	p += input->get() - 1;
	s = t;
	lastdelta = now;
}
