////////////////////////////////////////////////////////////////////////////
// child.cpp : loads the library, execute tests and sent test log  //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 3                                        //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Wenqi Zheng, EECS Department, Syracuse University        //
//               315/395-9301, wzheng11@syr.edu							  //
//																		  //
////////////////////////////////////////////////////////////////////////////

#include "../logger/logger.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "child_process.h"
#include <iostream>
#include <thread>
#include <functional>
#include <windows.h>

using namespace MsgPassingCommunication;

int main(int argc, char* argv[]) {
	int childIndex = std::stoi(argv[1]);
	int childPort = stoi("919" + std::to_string(childIndex));
	std::cout << "child No." + std::to_string(childIndex) + " in port " + std::to_string(childPort) + " started." << endl;
	SocketSystem ss;
	EndPoint eptest("localhost", 9190);
	EndPoint epchild("localhost", childPort);
	Comm comm(epchild, "comm"); comm.start();

	// send msg from child to testharness, suggesting child is ready
	MsgPassingCommunication::Message msg;
	msg.to(eptest); msg.from(epchild);
	msg.command("ready");
	std::cout << "\nchild No." + std::to_string(childIndex) + " in port " + std::to_string(childPort) + " just posted first ready message";
	comm.postMessage(msg);

	while (1) {
		MsgPassingCommunication::Message requstMsg = comm.getMessage();
		if (requstMsg.command() == "dotest") {
			std::string file_path = "../SaveFile/"+ requstMsg.name();
			std::string log_path = "../log_files/child-" + std::to_string(childIndex) + '-' + requstMsg.name() + ".log";
			Executive exe(file_path, log_path);
			bool res = exe.test();
			MsgPassingCommunication::Message reReady;
			reReady.to(eptest); reReady.from(epchild);
			reReady.command("ready");
			std::cout << "\n\ntest in child No." << childIndex << " finished tests in " + file_path + " \nReready this child.";
			comm.postMessage(reReady);//from child to testharness, ready again
			EndPoint epOriginClient = MsgPassingCommunication::EndPoint::fromString(requstMsg.attributes()["origin"]);
			std::cout << "\norigin client port:" << epOriginClient.port << std::endl;
			MsgPassingCommunication::Message logMsg;
			logMsg.to(epOriginClient); logMsg.from(epchild);
			res ? logMsg.attribute("result",("success in testing " + requstMsg.name())) : logMsg.attribute("result", ("failure in testing " + requstMsg.name()));
			logMsg.command("result");
			/*logMsg.file("child-" + std::to_string(childIndex) + '-' + requstMsg.name() + ".log");*/
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			comm.postMessage(logMsg);// from child to client, log result
		}
	}
	MsgPassingCommunication::Message msg2;
	msg2.command("stop");    // stop receiver
	comm.postMessage(msg2);
	msg2.command("quit");    // stop sender
	comm.postMessage(msg2);
	comm.stop(); std::cout << "\n\n";
}
