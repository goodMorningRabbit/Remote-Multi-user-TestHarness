#pragma once
#ifndef TEST_H
#define TEST_H
////////////////////////////////////////////////////////////////////////////
// Testharness.h : child process header file							  //
// ver 1.2                                                                //
//                                                                        //
// Application : OOD F18 Project 3	                                      //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               315/395-9301, wzheng11@syr.edu							  //
// Source      : Jim Fawcett   CSE687-Object Oriented Design              //
//				 Fall 2018, Syracuse University							  //
////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides the testharness process. Test harness spawns up a
*  number of child process when starts. It maintains two blocking queues of
*  child process in ready status and testrequsts sent from client. It
*  distribute the testrequests to the child in ready status in a thread-safty
*  way.
*  The main funcion of teshharness includes:
*  -1. fetches test requests files (*.tr) from client side to server side
*  -2. maintains the blocking queue of child process and testrequest, with 2 queues
*  -3. adds the client port to each message sent to child process in the "origin" attribute, 
*      to make sure child process where the requests come from so that they can send their 
*      results back.
*
*
*  Required Files:
*  ---------------
*  Process.h, Process.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.2   12/4 2018
	- design for project 4
*  ver 1.0 : 19 Feb 2018
*  - first release
*/

#include <windows.h>
#include <string>
#include <iostream>
#include <functional>
#include <fstream>
#include <iomanip>

using CBP = std::function<void(void)>;

class Process
{
public:
	Process();
	virtual ~Process() {}
	bool create(const std::string& path = "");
	void title(const std::string& title);
	void application(const std::string& app);
	void commandLine(const std::string& cmdLine);
	void registerCallback();
	void setCallBackProcessing(CBP cbp);
	void callback();
private:
	STARTUPINFO startInfo_;
	PROCESS_INFORMATION procInfo_;
	std::wstring title_;
	std::wstring appName_;
	std::wstring commandLine_;
	static CBP cbp_;
};

CBP Process::cbp_ = []() { std::cout << "\n  --- child process exited ---"; };

//----< Process constructor >------------------------------------------
/*
* - This eventually will have more functionality
*/
inline Process::Process()
{
	GetStartupInfo(&startInfo_);
	startInfo_.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	startInfo_.dwX = 200;
	startInfo_.dwY = 250;
	startInfo_.dwYSize = 300;
}
//----< helper function to convert strings >---------------------------

std::string wToS(const std::wstring& ws)
{
	std::string conv;
	for (auto ch : ws)
	{
		conv.push_back(static_cast<char>(ch));
	}
	return conv;
}
//----< helper function to convert strings >---------------------------

std::wstring sToW(const std::string& s)
{
	std::wstring conv;
	for (auto ch : s)
	{
		conv.push_back(static_cast<wchar_t>(ch));
	}
	return conv;
}
//----< define application new process runs >--------------------------

inline void Process::application(const std::string& appName)
{
	appName_ = sToW(appName);
}
//----< define commandline passed to new application >-----------------

inline void Process::commandLine(const std::string& cmdLine)
{
	commandLine_ = sToW(cmdLine);
}
//----< define title bar for console window >--------------------------
/*
* - applications like Notepad overwrite this string
*/
inline void Process::title(const std::string& title)
{
	title_ = sToW(title);
	startInfo_.lpTitle = const_cast<LPWSTR>(title_.c_str());
}
//----< start new child process >--------------------------------------

inline bool Process::create(const std::string& appName)
{
	std::wstring app;
	if (appName.size() == 0)
		app = appName_;
	else
	{
		app = sToW(appName);
	}
	LPCTSTR applic = app.c_str();
	LPTSTR cmdLine = const_cast<LPTSTR>(commandLine_.c_str());
	LPSECURITY_ATTRIBUTES prosec = nullptr;
	LPSECURITY_ATTRIBUTES thrdsec = nullptr;
	BOOL inheritHandles = false;
	DWORD createFlags = CREATE_NEW_CONSOLE;  // or CREATE_NO_WINDOW
	LPVOID environment = nullptr;
	LPCTSTR currentPath = nullptr;
	LPSTARTUPINFO pStartInfo = &startInfo_;
	LPPROCESS_INFORMATION pPrInfo = &procInfo_;

	BOOL OK =
		CreateProcess(
			applic, cmdLine, prosec, thrdsec, inheritHandles,
			createFlags, environment, currentPath, pStartInfo, pPrInfo
		);
	return OK;
}

///////////////////////////////////////////////////////////////////////
// child process exit callback processing

//----< provide new callable object for callback to invoke >-----------

void Process::setCallBackProcessing(CBP cbp)
{
	cbp_ = cbp;
}
//----< function called when child process exists >--------------------

void Process::callback()
{
	cbp_();
}
//----< Windows API declared function type for callbacks >-------------

void CALLBACK WaitOrTimerCallback(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired)
{
	Process p;
	p.callback();
	return;
}
//----< register callback with Windows API >---------------------------

void Process::registerCallback()
{
	HANDLE hNewHandle;
	HANDLE hProcHandle = procInfo_.hProcess;
	RegisterWaitForSingleObject(&hNewHandle, hProcHandle, WaitOrTimerCallback, NULL, INFINITE, WT_EXECUTEONLYONCE);
}

#endif