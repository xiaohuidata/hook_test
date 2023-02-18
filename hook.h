#pragma once
#include <Windows.h>

class Hook
{
public:
	static Hook* GetHook();

	void CreateHook();
	void run();
private:
	Hook() {};
	~Hook() {};

	
	HHOOK keyboardHook = 0;              // ¹³×Ó¾ä±ú
	static Hook* hook;
};