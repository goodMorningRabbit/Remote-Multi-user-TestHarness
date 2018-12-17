#ifndef DLLLOADER_H
#define DLLLOADER_H
#pragma once
////////////////////////////////////////////////////////////////////////////
// DllLoader.h : Test-Driver-DLL loader that uses ITests.h as protocole   //
// ver 1.1                                                                //
//                                                                        //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This package loads Test-Driver DLLs that follow the ITests protocole
*  defined in ITests.h
*
*  The protocol forces all TestDrivers to have an implementation of ITests
*  interface which holds a collection of ITest pointers. This package will
*  attempt to load the DLL, get an instance of ITests* and retrieve the
*  std::vector<ITest*> contained within the DLL.
*
*  Required files:
* =================
*  ITests.h DllLoader.h DllLoader.cpp
*  Note: nothing else is needed since this will load the DLLs at RUNTIME.
*
*  Maintainence History:
* =======================
*  ver 1.1 : 27 Sep 2018 : Jim Fawcett
*  - demonstrate some options for converting strings
*  ver 1.0 : 24 Sep 2018 : Ammar Salman
*  - first release
*/
#include <iostream>
#include "../_ITests.h"
#include <vector>
#include <windows.h>

using namespace std;

// a function to convert std::string to std::wstring (Windows.h uses a
// different string format)
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
//----< alternate string converter using std C++ >-------------------

std::wstring toWstring(const std::string& s)
{
	std::wstring wstr;
	for (auto ch : s)
	{
		wstr += (wchar_t)ch;
	}
	return wstr;  // will move
}

class DllLoader
{
public:
	DllLoader() {}

	~DllLoader()
	{
		clear(); // to make sure the DLL is unloaded
	}

	// accepts a DLL path and attempts to load it and extract its tests

	bool loadAndExtract(const std::string & dll)
	{
		///////////////////////////////////////////////////////
		// all of these work - Fawcett
		//----------------------------------------------------
		// hmod = LoadLibrary(s2ws(dll).c_str()); // load DLL
		// hmod = LoadLibraryA(dll.c_str()); // load DLL

		hmod = LoadLibraryW(toWstring(dll).c_str()); // load DLL
		if (!hmod)
		{
			std::cout << "\nfailed to load dll \"" << dll << "\"    Terminating...\n";
			return false;         // check if DLL was properly loaded
		}
		// check if the DLL contains get_ITests() function ...
		// in other words, check if the DLL complies with the protocol

		//typedef ITests* (*gITs)();
		using gITs = ITests * (*)();  // using instead of typedef

		gITs get_ITests = (gITs)GetProcAddress(hmod, "get_ITests");
		if (!get_ITests) {
			cout << " failed to get_ITests.\n    Terminating...\n";
			return false;
		}

		// execute get_ITests() to get the collection of tests
		ITests* tests = get_ITests();
		if (tests) { // check if tests is nullptr before attempting to get its tests collection
			std::vector<_ITest*> extTsts = tests->tests();
			extractedTests.insert(extractedTests.begin(), extTsts.begin(), extTsts.end());
			return true;
		}

		// this will only be reached if get_ITests() returned nullptr in which case the 
		// TestDriver DLL contains no tests to execute.
		return false;
	}

	const std::vector<_ITest*>& tests() const
	{
		return extractedTests;
	}

	void clear()
	{
		if (hmod) FreeLibrary(hmod);
		extractedTests.clear();
	}

private:
	std::vector<_ITest*> extractedTests;  // contains the set of tests extracted from the TestDriver DLL
	HMODULE hmod;  // handle to the loaded DLL
};

#endif