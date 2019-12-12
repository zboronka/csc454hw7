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
#include "化け物.hpp"

int main() {
	auto a = new Player(300, devsim::SECOND*1, 0);
	a->in_actions->push_back(new devsim::Port<Action>());
	a->in_actions->push_back(new devsim::Port<Action>());
	a->out_actions->push_back(new devsim::Port<Action>());
	a->out_actions->push_back(new devsim::Port<Action>());

	auto b = new Bakemono(150, devsim::SECOND*2, 1);
	b->in_actions->push_back(new devsim::Port<Action>());
	b->out_actions->push_back(new devsim::Port<Action>());

	auto c = new Bakemono(150, devsim::SECOND*2, 2);
	c->in_actions->push_back(new devsim::Port<Action>());
	c->out_actions->push_back(new devsim::Port<Action>());

	auto names = new std::unordered_map<void*, std::string>();
	(*names)[a] = "Player 1";
	(*names)[b] = "化け物一";
	(*names)[c] = "化け物二";

	auto pipes = new std::vector<devsim::Pipe*>();
	pipes->push_back(new devsim::Pipe(a->out_actions->at(0), b->in_actions->at(0), a, b));
	pipes->push_back(new devsim::Pipe(b->out_actions->at(0), a->in_actions->at(0), b, a));
	pipes->push_back(new devsim::Pipe(a->out_actions->at(1), c->in_actions->at(0), a, c));
	pipes->push_back(new devsim::Pipe(c->out_actions->at(0), a->in_actions->at(1), c, a));

	auto healths = new std::vector<devsim::Pipe*>();
	healths->push_back(new devsim::Pipe(a->health_port, new devsim::Port<int>(), a, nullptr));
	healths->push_back(new devsim::Pipe(b->health_port, new devsim::Port<int>(), b, nullptr));
	healths->push_back(new devsim::Pipe(c->health_port, new devsim::Port<int>(), c, nullptr));

	auto actions = new std::vector<devsim::Pipe*>();
	actions->push_back(new devsim::Pipe(c->out_actions->at(0), new devsim::Port<Action>(), c, nullptr));
	actions->push_back(new devsim::Pipe(b->out_actions->at(0), new devsim::Port<Action>(), b, nullptr));
	actions->push_back(new devsim::Pipe(a->out_actions->at(1), new devsim::Port<Action>(), a, nullptr));
	actions->push_back(new devsim::Pipe(a->out_actions->at(0), new devsim::Port<Action>(), a, nullptr));

	auto ainput = new devsim::Port<int>();
	auto in = new devsim::Pipe(ainput, a->input, nullptr, a);

	auto pqueue = new std::vector<devsim::Event>();
	{
		pqueue->push_back(devsim::Event(devsim::EXT, devsim::TotalTime(0, 0), a));
		pqueue->push_back(devsim::Event(devsim::EXT, devsim::TotalTime(0, 0), b));
		pqueue->push_back(devsim::Event(devsim::EXT, devsim::TotalTime(0, 0), c));
	}

	auto trajectory = new std::map<devsim::Event, int>();

	std::regex input("\\((\\d*\\.\\d*|\\d*),(\\d*)\\)");
	std::smatch matches;

	std::string command;
	for(std::cin >> command; command != "run"; std::cin >> command) {
		auto words_begin = std::sregex_iterator(command.begin(), command.end(), input);
		auto words_end = std::sregex_iterator();
		for(std::sregex_iterator i = words_begin; i != words_end; i++) {
			auto time = (long long)devsim::SECOND*atof((*i)[1].str().c_str());
			auto e = devsim::Event(devsim::EXT, devsim::TotalTime(time, 0), a);
			pqueue->push_back(e);
			make_heap(pqueue->begin(), pqueue->end(), devsim::Event::compare);
			(*trajectory)[e] = atoi((*i)[2].str().c_str());
		}
	}

	while(!pqueue->empty() && pqueue->front().time.get_real() < devsim::SECOND*10) {
		auto e = pqueue->front();
		pqueue->erase(pqueue->begin());
		make_heap(pqueue->begin(), pqueue->end(), devsim::Event::compare);

		auto search = trajectory->find(e);
		if(search != trajectory->end()) {
			ainput->set(search->second);
			in->pipe();
		}

		if(!pqueue->empty() && e.time == pqueue->front().time && e.target == pqueue->front().target) {
			pqueue->erase(pqueue->begin());
			pqueue->push_back(devsim::Event(devsim::CON, e.time, e.target));
			make_heap(pqueue->begin(), pqueue->end(), devsim::Event::compare);
			continue;
		}

		std::cout << e << std::endl << names->at(e.target) << std::endl << std::endl;

		switch(e.delta) {
			case devsim::EXT:
				e.target->delta_ext(e.time);
				if(e.target->get_internal() != nullptr) {
					for(auto i = pqueue->begin(); i < pqueue->end(); i++) {
						if(*e.target->get_internal()==*i) {
							pqueue->erase(i);
							make_heap(pqueue->begin(), pqueue->end(), devsim::Event::compare);
						}
					}
				}
				break;
			case devsim::INT:
				e.target->lambda();
				e.target->delta_int(e.time);
				break;
			case devsim::CON:
				e.target->lambda();
				e.target->delta_con(e.time);
			default:
				break;
		}

		for(auto pipe : *pipes) {
			if(pipe->pipe() && pipe->output_machine != nullptr) {
				pqueue->push_back(devsim::Event(devsim::EXT, e.time, pipe->output_machine));
				make_heap(pqueue->begin(), pqueue->end(), devsim::Event::compare);
			}
		}

		if(e.target->ta() >= 0) {
			e.target->set_internal(devsim::Event(devsim::INT, e.time.advance(e.target->ta()), e.target));
			pqueue->push_back(*e.target->get_internal());
			make_heap(pqueue->begin(), pqueue->end(), devsim::Event::compare);
		}

		for(auto i : *actions) {
			i->pipe();
			if(((devsim::Port<Action>*)i->output_port)->available()) {
				std::cout << names->at(i->input_machine) << " attacks with " << ((devsim::Port<Action>*)i->output_port)->get() << std::endl;
			}
		}

		for(auto i : *healths) {
			i->pipe();
			if(((devsim::Port<int>*)i->output_port)->available()) {
				std::cout << names->at(i->input_machine) << " health is " << ((devsim::Port<int>*)i->output_port)->get() << std::endl;
			}
		}
	}
}

