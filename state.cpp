#include "state.h"
#include <iostream>
#include <string>

string State::NextState(DWORD event)
{
	std::cout << "start:" << state << "|event:" << event << std::endl;
	auto it = states.find(state);
	if (it == states.end()) {
		state = "start";
		return state;
	}
	auto iter = it->second.find(event);
	if (iter == it->second.end()) {
		state = "start";
	}
	else
	{
		state = iter->second;
	}
	return state;
}

void State::Reset()
{
	state = "start";
}

void State::Execute()
{
	tasks->Execute();
}
