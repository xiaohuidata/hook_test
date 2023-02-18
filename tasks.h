#pragma once
#include <list>

enum TaskType {
	keyboard,
	mouse,
};

struct Task {
	TaskType type;
	int x;
	int y;
	int value;

	Task(TaskType type, int value) : type(type), value(value) {}
	Task(TaskType type, int x, int y, int value) : type(type), x(x), y(y), value(value) {}
	Task(const Task & task) {
		type = task.type;
		x = task.x;
		y = task.y;
		value = task.value;
	}
};

class Tasks {
public:
	Tasks(std::list<Task> t) {
		for (auto it : t) {
			tasks.push_back(it);
		}
	}
	void Execute();
private:
	std::list<Task> tasks;
};