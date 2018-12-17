////////////////////////////////////////////////////////////////////////////
// DllLoader.cpp : implementation and testing for DllLoader.h             //
// ver 1.2                                                                //
//      -test dll integrity, when you are not sure, especially when the   //
//		sending file procecedure breaks the file.						  //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               wzheng11@syr.edu										  //
// Source:     : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////

#include "DllLoader.h"
#include "../_ITests.h"

//#define TEST_DLLLOADER 1
#ifdef TEST_DLLLOADER

#include <iostream>
#include <iomanip>

int main() {
	cout << "\n  Testing DLL Loader";
	cout << "\n ====================";

	DllLoader loader;

	cout << "\n\n    Loading '../Debug/TestDriver1.dll' which is a single-test TestDriver...";

	
	LoggerHelp* logger;  	// create a logger for each testdrive
	bool loaded = loader.loadAndExtract("../SendFile/DemoSomeReqs.dll");
	std::string logfilePath = "../log_files/test_log.log";
	try {
		if (!loaded) { // load failure
			loader.clear();
		}
		else {
			
			std::cout << "Extracted tests:";
			
			for (auto test : loader.tests())
			{
				logger = new LoggerHelp();
				//let Executor package execute all the tests in each dll file
				Executor exe(test, *logger, logfilePath);
				exe.do_test();
			}
			loader.clear();
		}
	}
	catch (std::exception& ex) // catch exception and throw
	{
		std::cout << "\nTest dll \"" << "\" failed, exception occured.\n";
		std::cout << ex.what(); 
		
	}
}

#endif