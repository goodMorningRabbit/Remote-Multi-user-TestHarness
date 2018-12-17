////////////////////////////////////////////////////////////////////////////
// TestHarness.cpp :  maintain the process pool with two blocking queues  //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 3	                                      //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               315/395-9301, wzheng11@syr.edu							  //
////////////////////////////////////////////////////////////////////////////

#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../CppCommWithFileXfer/Utilities/CodeUtilities/CodeUtilities.h"
#include "../TestUtilities/FileSystem.h"
#include "TestHarness.h"
#include <iostream>
#include <vector>

using namespace Utilities;
using namespace MsgPassingCommunication;

int main(int argc, char* argv[]) {
	EndPoint eptest("localhost", 9190);
	EndPoint epclient("localhost", 9180);
	Comm comm(eptest, "comm"); comm.start();

	std::string childProcessPath = "../Debug/child_process.exe";
	std::string childNum = argv[1];
	for (int i = 1; i <= std::stoi(argv[1]); i++) {
		Process p;
		std::cout << "\nsetting up child No." << i << " from testharness";
		p.application(childProcessPath);
		std::cout << "\nchild process path: " << childProcessPath;
		p.commandLine("/A " + std::to_string(i));
		p.create();
	}
	BlockingQueue<Message> ready_queue;
	BlockingQueue<Message> request_queue;
	while (1) {
		Message msg = comm.getMessage();
		std::string temp = msg.command();
		if (msg.command() == "ready") {
			ready_queue.enQ(msg);
			std::cout << "\n testharness got ready message from " << msg.from().toString();
		}
		else if (msg.command() == "request") {
			request_queue.enQ(msg);
			std::cout << "\n testharness got request message from " << msg.from().toString();
		}
		if (ready_queue.size() > 0 && request_queue.size() > 0) {
			//who send a ready message should receive a dotest message
			Message rdyMsg;
			rdyMsg = ready_queue.deQ();
			rdyMsg.to(rdyMsg.from()); rdyMsg.from(eptest);
			rdyMsg.command("dotest");
			//one received request should be dequed
			Message reqMsg;
			reqMsg = request_queue.deQ();
			std::string requestTestFile = reqMsg.name();
			rdyMsg.name(requestTestFile);
			rdyMsg.file(requestTestFile);
			rdyMsg.attribute("origin", reqMsg.from().toString());
			comm.postMessage(rdyMsg);
		}
	}
	return 0;
}