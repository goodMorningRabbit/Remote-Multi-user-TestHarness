#pragma once
////////////////////////////////////////////////////////////////////////////
// Logger.h : Logger header file										  //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 2	                                      //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               315/395-9301, wzheng11@syr.edu							  //
// Source      : Jim Fawcett   CSE687-Object Oriented Design              //
//				 Fall 2018, Syracuse University							  //
////////////////////////////////////////////////////////////////////////////

/*
*  What's being demonstrated:
*  - std::ostream is a base class for output streams that include
*    std::ofstream, used to write to files, and std::ostringstream,
*    used to write to in-memory strings using the std::ostream
*    interface.
*  - The most fundamental principle in Object Oriented Design is the
*    Liskov Substitution Principle (LSP), which states:
*
*      Functions that accept base class pointers or references also
*      accept pointers or references to any class that derives from
*      the base, and can be used by the function without any special
*      knowledge beyond the base class interface.
*
*  That means that we can register standard streams that derive from
*  std::ostream with a logger if its add function accepts pointers or
*  references to the base std::ostream class.
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 20 Sep 2018
*  - first release
*/
#include <iostream>
#include <memory>
#include <vector>

class LoggerHelp
{
private:
	std::vector<std::ostream*> streams_;
public:
	void addStream(std::ostream* pStream)
	{
		streams_.push_back(pStream);
	}
	void write(const std::string& text)
	{
		for (auto pStrm : streams_)
			*pStrm << text.c_str();
	}
};