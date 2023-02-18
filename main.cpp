#include <iostream>
#include <fstream>
#include <string>
#include "hook.h"
#include "statemanage.h"
#include "taskmamage.h"
#include "rapidjson\document.h"

using namespace std;
using namespace rapidjson;

HWND hwndDOS = GetForegroundWindow();

string readconfig(string configname) {
	ifstream ifs;
	ifs.open(configname, ios::in);
	if (!ifs.is_open()) {
		cerr << "open " << configname << " failed" << endl;
		exit(0);
	}
	string ret;
	string buf;
	while (getline(ifs, buf)) {
		ret += buf;
	}
	ifs.close();
	return ret;
}

int mouseswitch(int ch) {
	switch (ch){
	case 'z':
		return MOUSEEVENTF_MIDDLEDOWN;
	case 'l':
		return MOUSEEVENTF_LEFTDOWN;
	case 'r':
		return MOUSEEVENTF_RIGHTDOWN;
	}
	return 0;
}

void analysis(const string& json)
{
	
	Document d;
	d.Parse(json.c_str());
	bool cmd = d["cmd"].GetBool();
	if (!cmd) {
		ShowWindow(hwndDOS, SW_HIDE);
	}
	int total = d["mode_total"].GetInt();
	for (int i = 1; i <= total; i++) {
		string moden = "mode" + to_string(i);
		Value::ConstMemberIterator it = d.FindMember(moden.c_str());
		if (it == d.MemberEnd()) {
			continue;
		}

		// state
		int t = it->value["state_total"].GetInt();
		map<string, map<DWORD, string> > states;
		vector<DWORD> ls;
		ls.push_back(0);
		for (int j = 1; j <= t; j++) {
			string staten = "state" + to_string(j);
			Value::ConstMemberIterator iter = it->value.FindMember(staten.c_str());
			if (iter == iter->value.MemberEnd()) {
				continue;
			}
			string type = iter->value["type"].GetString();
			DWORD state;
			if (type == "char") {
				state = iter->value["value"].GetString()[0];
			}
			else if (type == "int") {
				state = iter->value["value"].GetInt();
			}
			else {
				continue;
			}
			ls.push_back(state);
		}
		string start = "";
		string end = "end";
		int j = 0;
		for (auto lit = ls.rbegin(); lit != ls.rend(); ++lit) {
			start = to_string(*lit) + "_" + to_string(++j);
			DWORD evt = *lit;
			if (*(lit+1) == 0) {
				states["start"] = map<DWORD, string>{ { evt, end } };
				break;
			}
			states[start] = map<DWORD, string>{ { evt, end } };
			end = start;
		}

		for (auto ss : states) {
				auto ee = ss.second.begin();
				cout << "state:" << ss.first << " event:" << ee->first << " state2:" << ee->second << endl;
		}
		
		// task
		TaskManager* tm = new TaskManager();
		t = it->value["task_total"].GetInt();
		for (int j = 1; j <= t; j++) {
			std::list<Task> lt;
			string taskn = "task" + to_string(j);
			Value::ConstMemberIterator iter = it->value.FindMember(taskn.c_str());
			if (iter == it->value.MemberEnd()) {
				continue;
			}

			Value::ConstArray tasks = iter->value["tasks"].GetArray();
			for (auto task = tasks.Begin(); task != tasks.End(); task++) {
				TaskType Type;
				int value = 0;
				int x = -1;
				int y = -1;
				string type = (*task)["type"].GetString();
				try {
					if (type == "keyboard") {
						Type = keyboard;
						string ctype = (*task)["Type"].GetString();
						if (ctype == "char") {
							value = (*task)["value"].GetString()[0];
						}
						else if (ctype == "int") {
							value = (*task)["value"].GetInt();
						}
						else {
							continue;
						}
						cout << "char task " << value << endl;
					}
					else if (type == "mouse") {
						Type = mouse;
						x = (*task)["x"].GetInt();
						y = (*task)["y"].GetInt();
						value = (*task)["value"].GetString()[0];

						if (x < 0 || y < 0) {
							continue;
						}
						value = mouseswitch(value);
						if (value == 0) {
							continue;
						}
					}
				}
				catch (...){
					continue;
				}
				
				Task t(Type, x, y, value);
				lt.push_back(t);
			}
			Tasks ts(lt);
			tm->AddTask(ts);
		}
		tm->Run();
		State *st = new State(states, tm);
		StateManage::GetStateManage()->AddState(st);
	}
}

int main(int argc, char*argv[])
{
	string json = readconfig("config.json");
	analysis(json);
	
	
	Hook* hook = Hook::GetHook();
	hook->CreateHook();
	hook->run();
	return 0;
}