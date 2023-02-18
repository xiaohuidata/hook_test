#include "taskmamage.h"
#include <Windows.h>
#include <iostream>

void TaskManager::AddTask(Tasks& task)
{
	tasks.push_back(task);
}

void TaskManager::Run() {
	t = new std::thread(&TaskManager::Thread, this);
}

void TaskManager::Thread() {
	while (true) {
		if (!run) {
			Sleep(10);
			continue;
		}
		std::cout << "Execute" << std::endl;
		for (auto it : tasks) {
			it.Execute();
			Sleep(100);
		}
		run = false;
	}
}

void TaskManager::Execute()
{
	run = true;
}
