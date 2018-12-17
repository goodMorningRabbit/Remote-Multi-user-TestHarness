////////////////////////////////////////////////////////////////////////////
// DemoSomeReqs.h : demostrate the requirments							  //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 4	                                      //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               315/395-9301, wzheng11@syr.edu							  //
////////////////////////////////////////////////////////////////////////////

/*
*  Package description:
* ======================
*  This cpp demonstrate the requirements using the protocol defined in
*  _Itest.h, exporting them as dlls. Dllloader will accepts one or
*  multiple tests as a collection.

*  Required files:
* =================
*  _ITests.h  DemoSomeReqs.cpp
*  CodeUtilities.h  logger.h
*  FileUtilities.h
*
*  Maintainence History:
* =======================
* Nov.16
*  ver 1.0 - first release
*/

#define IN_DLL
#define ARG_SIZE 256  // for the manufactured cmd args

#include "../_ITests.h"
#include "../CppCommWithFileXfer/Utilities/CodeUtilities/CodeUtilities.h"
#include "../CppCommWithFileXfer/Utilities/FileUtilities/FileUtilities.h"
#include "../logger/logger.h"
#include <iostream>
#include <string>
#include <string.h>

using namespace Utilities;
using namespace std;
using Message = std::string;
using Path = std::string;

// the concrete Test class that implements ITest
// it performs tests on ProcessCmdArgs and Convereter classes in a single test class

class TestReq3 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};

DLL_DECL bool TestReq3::test(LoggerHelp& logger)
{
	Message msg = "Req #2 - assemble the working parts from Projects #1, #2, and #3 into a Client-Server configuration.";
	msg += " Unlike conventional Client-Server architectures, the Client need not wait for a reply for a TestRequest from the TestHarness before sending additional TestRequests.";
	logger.write(msg);
	Path fileSpec = "../GUI/MainWindow.xaml.cs";
	return Utilities::showFileLines(fileSpec, 396, 413, "Req #2");
}

DLL_DECL std::string TestReq3::name()
{
	return string("Requirements #2");
}

DLL_DECL std::string TestReq3::author()
{
	return string("zwqrabbit");
}

class TestReq4 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};

DLL_DECL bool TestReq4::test(LoggerHelp& logger)
{
	Message msg = "\nReq #4 - Shall provide a Graphical User Interface (GUI) for the client that supports selecting test libraires to send for testing, and supports displaying the test results."; logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../GUI/MainWindow.xaml.cs";
	return Utilities::showFileLines(fileSpec, 276, 293, "Req #4");
}

DLL_DECL std::string TestReq4::name()
{
	return string("Requirements #4");
}

DLL_DECL std::string TestReq4::author()
{
	return string("zwqrabbit");
}

class TestReq5 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};
DLL_DECL bool TestReq5::test(LoggerHelp& logger)
{
	Message msg = "\nReq #5 - Shall provide message designs appropriate for this application. All messages are instances of the same Message class, but have a specified set of attributes and body contents suited for the intended task.";
	logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../CppCommWithFileXfer/Message/Message.h";
	return Utilities::showFileLines(fileSpec, 78, 126, "Req #5");
}

DLL_DECL std::string TestReq5::name()
{
	return string("Requirement #5");
}

DLL_DECL std::string TestReq5::author()
{
	return string("zwqrabbit");
}

class TestReq6 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};


DLL_DECL bool TestReq6::test(LoggerHelp& logger)
{
	Message msg = "\nReq #6 - Shall support sending TestRequest messages from Client to TestHarness Server.";
	logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../GUI/MainWindow.xaml.cs";
	return Utilities::showFileLines(fileSpec, 415, 422, "Req #6");
}

DLL_DECL std::string TestReq6::name()
{
	return string("Requirement #6");
}

DLL_DECL std::string TestReq6::author()
{
	return string("zwqrabbit");
}

class TestReq7 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};


DLL_DECL bool TestReq7::test(LoggerHelp& logger)
{
	Message msg = "\nReq #7 - Shall support correctly executing each TestRequest, as a sequence of tests, with each TestRequest running in its own Process Pool child process.";
	logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../GUI/MainWindow.xaml.cs";
	Utilities::showFileLines(fileSpec, 413, 422, "Req #7");
	fileSpec = "../child_process/child_process.h";
	return Utilities::showFileLines(fileSpec,150,189, "Req #7");
}

DLL_DECL std::string TestReq7::name()
{
	return string("Requirement #7");
}

DLL_DECL std::string TestReq7::author()
{
	return string("zwqrabbit");
}

class TestReq8 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};
DLL_DECL bool TestReq8::test(LoggerHelp& logger)
{
	Message msg = "\nReq #8 - Shall support sending test results, via results messages, from the child process executing the tests to the Client that made the request.";
	logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../child_process/child_process.cpp";
	return Utilities::showFileLines(fileSpec, 52, 57, "Req #8");
}

DLL_DECL std::string TestReq8::name()
{
	return string("Requirements #8");
}

DLL_DECL std::string TestReq8::author()
{
	return string("zwqrabbit");
}


class TestReq9 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};
DLL_DECL bool TestReq9::test(LoggerHelp& logger)
{
	Message msg = "\nReq #9 - Shall demonstrate correct test operations for two or more concurrent clients. You need to ensure that testing takes enough time to ensure that more than one child process is running at the same time.";
	logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../GUI/App.xaml.cs";
	return Utilities::showFileLines(fileSpec, 16, 26, "Req #9");
}

DLL_DECL std::string TestReq9::name()
{
	return string("Requirements #9");
}

DLL_DECL std::string TestReq9::author()
{
	return string("zwqrabbit");
}


class TestReq10 : public _ITest {

public:
	DLL_DECL bool test(LoggerHelp& logger);
	DLL_DECL std::string name();
	DLL_DECL std::string author();
};
DLL_DECL bool TestReq10::test(LoggerHelp& logger)
{
	Message msg = "\nReq #10 - Shall include an automated unit test suite that demonstrates your Clients and TestHarness satisfy all of the functional requirements, above.";
	logger.write(msg);
	msg = "\n =============================="; logger.write(msg);
	Path fileSpec = "../DemoSomeReqs/DemoSomeReqs.cpp";
	return Utilities::showFileLines(fileSpec, 240, 254, "Req #10");
}

DLL_DECL std::string TestReq10::name()
{
	return string("Requirements #10");
}

DLL_DECL std::string TestReq10::author()
{
	return string("zwqrabbit");
}
//////////////////////////////////////////////////////////////////////////////
// test collection 

class TestCollection : public ITests {
	DLL_DECL std::vector<_ITest*> tests();
};

DLL_DECL std::vector<_ITest*> TestCollection::tests()
{
	std::vector<_ITest*> tests_vec;
	tests_vec.push_back(new TestReq3);
	tests_vec.push_back(new TestReq4);
	tests_vec.push_back(new TestReq5);
	tests_vec.push_back(new TestReq6);
	tests_vec.push_back(new TestReq7);
	tests_vec.push_back(new TestReq8);
	tests_vec.push_back(new TestReq9);
	tests_vec.push_back(new TestReq10);
	return tests_vec;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// this section is where each Test Driver DLL completely differs from other
// test drivers. Although the same name can be used in all TestDrivers, the 
// actual instance of TestCollection is different in the way it returns 
// different collection of ITest*.

DLL_DECL ITests* get_ITests()
{
	return new TestCollection;
}

#ifdef TEST_DEMOSOMEREQS

int main()
{
	std::cout << "This is a package for single test dll file, please set this package as dll";
	return 0;
}

#endif