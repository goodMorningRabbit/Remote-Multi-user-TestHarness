////////////////////////////////////////////////////////////////////////////
// Process.cpp -  Process Pool with Interprocess Communication            //
// Author : Zheng Zhan                                                    //
// Language : C++, Visual Studio 2017                                     //
// Project3 : ver 1.0  Process Pool with Interprocess Communication       //
// SUID: 825530128                                                        //
// SU Email: zzhan03@syr.edu                                              //
// CSE687-Object Oriented Design, Fall 2018                               //
////////////////////////////////////////////////////////////////////////////

#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../CppCommWithFileXfer/Utilities/CodeUtilities/CodeUtilities.h"
#include "Process.h"
#include <iostream>

using namespace Utilities;
using namespace MsgPassingCommunication;

/*----< Create child process >------------------------------------------*/
/*
*  - Supports create multi process.
*  - Each child process will receive a unique index.
*/
bool create_process(std::string path, int index)
{
	Process p;
	try
	{
		p.application(path);
		std::string num = Utilities::Converter<int>::toString(index);
		std::string cmdLine = "/A " + num;
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		p.commandLine(cmdLine);
		p.create();
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
		return false;
	}
	return true;
}

#ifdef TEST_TESTHARNESS
int main(int argc, char* argv[])
{
	std::string child_num = argv[1];
	int num = Converter<int>::toValue(child_num);
	EndPoint erPeer0("localhost", 8079);
	EndPoint epPeer1("localhost", 8080);
	Comm comm(epPeer1, "Peer1");
	comm.start();

	std::string appPath = "../Debug/child_process.exe";
	std::cout << "\n  starting process: \"" << appPath << "\"";
	for (int i = 1; i <= num; i++) {
		if (create_process(appPath, i)) std::cout << " - succeeded";
		else std::cout << " - failed";
	}

	Message rdyMsg;
	rdyMsg.to(erPeer0);
	rdyMsg.from(epPeer1);
	rdyMsg.command("Process_call");
	comm.postMessage(rdyMsg);
	Message msg;
	BlockingQueue<Message> rdyQ;BlockingQueue<Message> reqQ;
	
	while (true){
		msg = comm.getMessage();
		if (msg.command() == "ready"){rdyQ.enQ(msg);}
		else if (msg.command() == "request"){reqQ.enQ(msg);}
		if (rdyQ.size() != 0 && reqQ.size() != 0){
			Message msg2;
			msg2 = rdyQ.deQ();
			msg2.to(msg2.from());
			msg2.from(epPeer1);
			std::cout << "\n show message from " << msg2.from().toString() << " to " <<msg2.to().toString();
			msg2.command("dotest");
			Message msg3;
			msg3 = reqQ.deQ();
			msg2.name(msg3.name());
			std::cout << "\n show test file name " << msg2.name();
			comm.postMessage(msg2);
		}
	}
	return 0;
}

#endif