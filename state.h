#pragma once
#include <map>
#include <Windows.h>
#include <list>
#include "taskmamage.h"
#include "rapidjson\document.h"

using namespace std;

// "start"  ��ʼ״̬
// "end"  ����״̬
class State
{
public:
	State(map<string, map<DWORD, string> > states, TaskManager* task) : states(states), tasks(task), state("start"){ };
	~State() {};

	string NextState(DWORD event);
	void Reset();
	void Execute();
	const DWORD GetFlags()const { return flags; }

private:
	DWORD flags;
	string state;
	map<string, map<DWORD, string>> states;
	TaskManager* tasks;
};