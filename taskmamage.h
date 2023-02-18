#pragma once

#include "tasks.h"
#include <thread>

class TaskManager
{
public:
	void AddTask(Tasks& task);
	void Run();
	void Thread();
	void Execute();
private:
	std::list<Tasks> tasks;
	bool run = false;
	std::thread *t;
};
