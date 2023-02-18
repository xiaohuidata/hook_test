#include "hook.h"
#include "statemanage.h"
#include <iostream>

Hook* Hook::hook = nullptr;

LRESULT CALLBACK LowLevelKeyboardProc(
	_In_ int nCode,                   // 规定钩子如何处理消息，小于 0 则直接 CallNextHookExit
	_In_ WPARAM wParam,					// 消息类型
	_In_ LPARAM lParam					// 指向某个结构体的指针，这里是 KBDLLHOOKSTRUCT（低级键盘输入事件）
) {
	KBDLLHOOKSTRUCT *ks = (KBDLLHOOKSTRUCT*)lParam;

	/*
	typedef struct tagKBDLLHOOKSTRUCT {
	DWORD vkCode;				// 按键代号
	DWORD scaCode;				// 硬件扫描带好，同 vkCode 也可以作为按键的代号。
	DWORD flags;				// 事件类型，一般案件按下为0 抬起为128
	DWORD time;					// 消息时间戳
	ULONG_PTR dwExtraInfo;		// 消息附加信息，一般为0.
	} KBDLLHOOKSTRUCT, *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;
	*/
	//cout << "ks->flags:" << ks->flags << "|" << endl;
	/*
	if (ks->flags == 0)
	{
	cout << "0:" << ks->vkCode << endl;
	}
	
	if (ks->flags == 128)
	{
		std::cout << "128:" << ks->vkCode << std::endl;
	}
	
	if (ks->flags == 129)
	{
	cout << "129:" << ks->vkCode << endl;
	}
	*/
	
	if (ks->flags == 0)
	{
		StateManage::GetStateManage()->PushEvent(ks->vkCode);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


void  Hook::CreateHook()
{
	// 安装钩子
	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,				// 钩子类型，WH_KEYBOARD_LL为键盘钩子
		LowLevelKeyboardProc,		// 指向钩子函数的指针
		GetModuleHandleA(NULL),		// Dll 句柄
		NULL
	);
	if (keyboardHook == 0) {
		std::cerr << "挂钩键盘失败" << std::endl;
		exit(0);
	}
}

Hook* Hook::GetHook()
{
	if (hook != nullptr) {
		return hook;
	}
	hook = new Hook();
	return hook;
}


void Hook::run()
{
	// 不可漏掉的消息处理，不然程序会卡死
	MSG msg;
	while (1)
	{
		// 如果消息队列中有消息
		if (PeekMessageA(
			&msg,			// MSG 接收这个消息
			NULL,			// 检测消息的窗口句柄，NULL：检索当前线程所有窗口信息
			NULL,			// 检查消息范围中第一个消息的值，NULL：检查所有消息（必须和下面的同时为NULL）
			NULL,			// 检查消息范围中最后一个消息的值，NULL：检查所有消息（必须和上面的同时为NULL）
			PM_REMOVE)) {	// 处理消息的方式，PM_REMOVE；处理后将消息从队列中删除
							// 把按键消息传递给字符消息
			TranslateMessage(&msg);

			// 消息分派给窗口程序
			DispatchMessageW(&msg);
		}
		else
			Sleep(0);
	}
	// 删除钩子
	UnhookWindowsHookEx(keyboardHook);
}