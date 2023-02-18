#include "tasks.h"
#include <Windows.h>
#include <iostream>

DWORD Switch(DWORD flag) {
	switch (flag){
	case MOUSEEVENTF_LEFTDOWN:
		return MOUSEEVENTF_LEFTUP;
	case MOUSEEVENTF_RIGHTDOWN:
		return MOUSEEVENTF_RIGHTUP;
	case MOUSEEVENTF_MIDDLEDOWN:
		return MOUSEEVENTF_MIDDLEUP;
	}
	return 0;
}

void Tasks::Execute()
{
	std::cout << "tasks:" << std::endl;
	for (auto it : tasks) {
		INPUT inputs[1] = {};
		ZeroMemory(inputs, sizeof(inputs));
		// °´¼ü VK_CONTROL
		if (it.type == keyboard) {
			std::cout << "taskstype(" << it.type << ") value:" << it.value << std::endl;
			inputs[0].type = INPUT_KEYBOARD;
			inputs[0].ki.wVk = it.value;
		}
		if (it.type == mouse) {
			std::cout << "taskstype(" << it.type << ") (" << it.x << "," << it.y << ") value:" << it.value << std::endl;
			SetCursorPos(it.x, it.y);
			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dx = 0;
			inputs[0].mi.dy = 0;
			inputs[0].mi.mouseData = 0;
			inputs[0].mi.dwFlags = it.value;

		}
		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		if (uSent != ARRAYSIZE(inputs))
		{
			std::cout << "SendInput failed: " << HRESULT_FROM_WIN32(GetLastError()) << std::endl;
		}
	}
	
	for (auto it : tasks) {
		INPUT inputs[1] = {};
		ZeroMemory(inputs, sizeof(inputs));
		if (it.type == keyboard) {
			inputs[0].type = INPUT_KEYBOARD;
			inputs[0].ki.wVk = it.value;
			inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
		}
		if (it.type == mouse) {
			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dx = 0;
			inputs[0].mi.dy = 0;
			inputs[0].mi.mouseData = 0;
			inputs[0].mi.dwFlags = Switch(it.value);

		}
		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		if (uSent != ARRAYSIZE(inputs))
		{
			std::cout << "SendInput failed: " << HRESULT_FROM_WIN32(GetLastError()) << std::endl;
		}
	}
	return;

	for (auto it : tasks) {
		// °´¼ü VK_CONTROL
		if (it.type == keyboard) {
			std::cout << "taskstype(" << it.type << ") value:" << it.value << std::endl;
			keybd_event(it.value, 0, 0, 0);
		}
		if (it.type == mouse) {
			std::cout << "taskstype(" << it.type << ") (" << it.x << ","<< it.y<< ") value:" << it.value << std::endl;
			SetCursorPos(it.x, it.y);
			switch (it.value) {
			case MOUSEEVENTF_LEFTDOWN:
			case MOUSEEVENTF_RIGHTDOWN:
			case MOUSEEVENTF_MIDDLEDOWN:
				mouse_event(it.value, 0, 0, 0, 0);
			}
		}
	}

	for (auto it : tasks) {
		if (it.type == keyboard)
			keybd_event(it.value, 0, 2, 0);
		if (it.type == mouse) {
			SetCursorPos(it.x, it.y);
			switch (it.value) {
			case MOUSEEVENTF_LEFTDOWN:
			case MOUSEEVENTF_RIGHTDOWN:
			case MOUSEEVENTF_MIDDLEDOWN:
				mouse_event(Switch(it.value), 0, 0, 0, 0);
			}
		}
	}
}
