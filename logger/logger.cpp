/////////////////////////////////////////////////////////////////////
// Logger.cpp - demonstrates how to register streams			   //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#include "logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
bool displayFileContents(const std::string& msg, const std::string& fileSpec)
{
	if (msg.size() > 0)
	{
		std::cout << "\n\n  " << msg;
	}
	std::ifstream inFileStrm(fileSpec);
	if (!inFileStrm.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n";
		return false;
	}
	while (inFileStrm.good())
	{
		std::string line;
		std::getline(inFileStrm, line);
		std::cout << "\n" << line;
	}
	return true;
}

//#define TEST_LOGGER 1
#ifdef  TEST_LOGGER
int main()
{
	std::cout << "\n  Demonstrating LoggerHelp";
	std::cout << "\n ==========================";
	_Logger lh;
	lh.addStream(&std::cout);  // adding pointer to an instance of std::ostream

	std::string fileSpec = "../log_files/log_test.txt";

	std::ofstream outfileStrm(fileSpec, std::ios::out);
	if (outfileStrm.good())
		lh.addStream(&outfileStrm);  // adding pointer to std::ofstream instance

	std::cout << "\n  logging to console and " << fileSpec << "\n";

	lh.write("\n  Hi from main");
	lh.write("\n  hi again");

	outfileStrm.close();
	std::string msg = "displaying contents of " + fileSpec;

	displayFileContents(msg, fileSpec);

	std::cout << "\n\n";
	system("pause");
	return 0;
}
#endif // DEBUG


