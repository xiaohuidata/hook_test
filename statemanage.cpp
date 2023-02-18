#include "statemanage.h"
#include "state.h"
#include <thread>

StateManage* StateManage::manage = nullptr;
StateManage * StateManage::GetStateManage()
{
	if (manage != nullptr) {
		return manage;
	}
	manage = new StateManage();
	return manage;
}

void StateManage::AddState(State* state)
{
	vecStates.push_back(state);
	
}

void StateManage::PushEvent(DWORD event)
{
	for (auto it: vecStates) {
		/*
		if (flags != it->GetFlags()) {
			continue;
		}
		*/
		if (it->NextState(event) == "end") {
			it->Execute();
			it->Reset();
		}
	}
}


