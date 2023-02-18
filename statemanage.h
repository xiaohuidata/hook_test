#pragma once

#include "state.h"
#include <vector>

class StateManage
{
public:
	static StateManage* GetStateManage();
	void AddState(State* state);
	void PushEvent(DWORD event);

private:
	StateManage() {};
	~StateManage() {};

	std::vector<State*> vecStates;
	static StateManage* manage;
};