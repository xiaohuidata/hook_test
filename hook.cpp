#include "hook.h"
#include "statemanage.h"
#include <iostream>

Hook* Hook::hook = nullptr;

LRESULT CALLBACK LowLevelKeyboardProc(
	_In_ int nCode,                   // �涨������δ�����Ϣ��С�� 0 ��ֱ�� CallNextHookExit
	_In_ WPARAM wParam,					// ��Ϣ����
	_In_ LPARAM lParam					// ָ��ĳ���ṹ���ָ�룬������ KBDLLHOOKSTRUCT���ͼ����������¼���
) {
	KBDLLHOOKSTRUCT *ks = (KBDLLHOOKSTRUCT*)lParam;

	/*
	typedef struct tagKBDLLHOOKSTRUCT {
	DWORD vkCode;				// ��������
	DWORD scaCode;				// Ӳ��ɨ����ã�ͬ vkCode Ҳ������Ϊ�����Ĵ��š�
	DWORD flags;				// �¼����ͣ�һ�㰸������Ϊ0 ̧��Ϊ128
	DWORD time;					// ��Ϣʱ���
	ULONG_PTR dwExtraInfo;		// ��Ϣ������Ϣ��һ��Ϊ0.
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
	// ��װ����
	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,				// �������ͣ�WH_KEYBOARD_LLΪ���̹���
		LowLevelKeyboardProc,		// ָ���Ӻ�����ָ��
		GetModuleHandleA(NULL),		// Dll ���
		NULL
	);
	if (keyboardHook == 0) {
		std::cerr << "�ҹ�����ʧ��" << std::endl;
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
	// ����©������Ϣ������Ȼ����Ῠ��
	MSG msg;
	while (1)
	{
		// �����Ϣ����������Ϣ
		if (PeekMessageA(
			&msg,			// MSG ���������Ϣ
			NULL,			// �����Ϣ�Ĵ��ھ����NULL��������ǰ�߳����д�����Ϣ
			NULL,			// �����Ϣ��Χ�е�һ����Ϣ��ֵ��NULL�����������Ϣ������������ͬʱΪNULL��
			NULL,			// �����Ϣ��Χ�����һ����Ϣ��ֵ��NULL�����������Ϣ������������ͬʱΪNULL��
			PM_REMOVE)) {	// ������Ϣ�ķ�ʽ��PM_REMOVE���������Ϣ�Ӷ�����ɾ��
							// �Ѱ�����Ϣ���ݸ��ַ���Ϣ
			TranslateMessage(&msg);

			// ��Ϣ���ɸ����ڳ���
			DispatchMessageW(&msg);
		}
		else
			Sleep(0);
	}
	// ɾ������
	UnhookWindowsHookEx(keyboardHook);
}