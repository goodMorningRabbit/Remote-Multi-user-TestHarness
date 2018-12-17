#ifndef CHILD_H
#define CHILD_H
#pragma once
////////////////////////////////////////////////////////////////////////////
// child.h : child process header file									  //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 3	                                      //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               315/395-9301, wzheng11@syr.edu							  //
// Source      : Jim Fawcett   CSE687-Object Oriented Design              //
//				 Fall 2018, Syracuse University							  //
////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* childProcess pkg will provide main functionality of testing. It accepts
* request message from testharness, execute the test and send ready message
* back to testharness as soon as it finishes the test. The execution of the
* test is realized by the testharness core similar to project 1 & 2. The
* child process will also send a log message back to client, and log the
* test result to logfile.
*
*
* Build Process:
* --------------
* Required Files:
*   Executor.h
*   child.h child.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 15 Nov 2018
* - first release
*
* Notes:
* ------
* - Designed to provide all
functionality in header file.
*/
#include <fstream>
#include <iostream>
#include <iomanip>
#include "../CppCommWithFileXfer/Utilities/FileUtilities/FileUtilities.h"
#include "../_ITests.h"
#include "../logger/logger.h"
#include "../DllLoader/DllLoader.h"
#include "../CppCommWithFileXfer/Utilities/FileSystem/FileSystem.h"
#include "../Utilities/Properties/Properties.h"
#include "../TestRequest/TestRequest.h"

using namespace Testing;
using namespace std;
using Requests = std::vector<TestRequest>;
using namespace Utilities;

class Tester
{
public:
	Property<Requests> requests;
	Property<std::string> requestPath;
	Property<std::string> libPath;

	//----< initialize paths and logger >------------------------------

	Tester()
	{
		requestPath("../TestRequests");
		libPath("../testLibraries");
	}
	//----< deallocate logger >----------------------------------------

	~Tester() {};
	//----< load Test Requests >-----------------------------------------
	/*
	* - In Project #4 serialized TestRequests, sent by client, will be
	*   extracted from Comm channel.
	*/
	void loadTestRequests(std::string test_req)
	{
		std::ifstream in;

		if (!Utilities::openReadFile(test_req, in))
		{
			std::cout << "\nFailure to fine test request \"" << test_req << "\"\n";
		}
		std::ostringstream sstr;
		sstr << in.rdbuf();
		TestRequest tr = TestRequest::fromString(sstr.str());
		requests.valueRef().push_back(tr);
	}

	//void loadLibraries(TestRequest& tr);
	//bool doTests();
	void setLibraryPath(std::string& path)
	{
		libPath(path);
	}
	void clear()
	{
		dllLoader_.clear();
	}
private:
	DllLoader dllLoader_;
};

class Executor
{
private:
	_ITest* _test;
	LoggerHelp& _logger;
public:
	Executor(_ITest* test, LoggerHelp& logger, std::string logPath) :
		_test(test), _logger(logger) {};
	~Executor()
	{
		cout << "\n deleting Executor";
		delete _test;
	}
	bool do_test()
	{
		//execute test in each dll file
		cout << "\n      Test name: " << setw(30) << left << _test->name() << " - Author: " << setw(20) << _test->author();
		cout << "\n      Executing test...";
		bool res = _test->test(_logger);
		res ? _logger.write("\ntest passed.") : _logger.write("\ntest failed.");

		return true;
	}
};

class Executive
{
private:
	std::string test_name;
	LoggerHelp* logger;
	std::string _logPath;
public:
	Executive(std::string filePath, std::string logPath) :
		test_name(filePath), _logPath(logPath) {};
	~Executive() {
		std::cout << "\n  deleting Executive";
	};
	bool test()
	{
		TestRequest tr; Tester tester;
		tester.loadTestRequests(test_name);

		for (TestRequest& tr : tester.requests())
		{
			for (auto dll : tr.request.valueRef())
			{
				DllLoader loader;
				bool loaded = loader.loadAndExtract("../SaveFile/"+dll);
				std::string logfilePath = _logPath;
				logger = new LoggerHelp();  	// create a logger for each testdrive
				try {
					if (!loaded) { // load failure
						loader.clear();
						logger->write("\nload failure of dll \"" + dll + "\".\n");
					}
					else {
						std::cout << "\nSuccessfully load dll \"" << dll << "\"\n";
						std::cout << "Extracted tests:";
						logger->addStream(&std::cout);
						std::ofstream outfileStrm(logfilePath, std::ios::out);
						if (outfileStrm.good())
							logger->addStream(&outfileStrm);  // adding pointer to std::ofstream instance
						for (auto test : loader.tests())
						{
							//let Executor package execute all the tests in each dll file
							Executor exe(test, *logger, logfilePath);
							exe.do_test();
						}
						outfileStrm.close();
						loader.clear();
					}
					logger->write("\nTest on dll \"" + dll + "\" seceeced.\n");
					delete logger;
				}
				catch (std::exception& ex) // catch exception and throw
				{
					/*std::cout << "\nTest dll \"" << test_name << "\" failed, exception occured.\n";
					std::cout << ex.what();*/
					logger->write("\nTest on dll \"" + dll + "\" failed, exception occured.\n");
					logger->write(ex.what());
					delete logger;
					return false;
				}
			}
		}
		return true;
	}
};

#endif