/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../TestUtilities/FileSystem.h"
#include "../TestRequest/TestRequest.h"
#include "../CppCommWithFileXfer/Utilities/FileUtilities/FileUtilities.h"
#include <chrono>
namespace MsgPassComm = MsgPassingCommunication;

using namespace Utilities;
using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

Files Server::getFiles(std::string pattern, const Repository::SearchPath& path)
{
  return Directory::getFiles(path, pattern);
}

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

//Deserialization
std::vector<std::string> decode(std::string s) {
	std::vector<std::string> result;
	int len = s.size(), pos = 0;
	while (pos < len)
	{
		int k = s.find('$', pos), num = stoi(s.substr(pos, k - pos));
		result.push_back(s.substr(k + 1, num));
		pos = k + num + 1;
	}
	return result;
}

std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};

std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
   /* if (path != ".")
      searchPath = searchPath + "\\" + path;*/
    Files files = Server::getFiles("*.dll");
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};

std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
   /* if (path != ".")
      searchPath = searchPath + "\\" + path;*/
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};

std::function<Msg(Msg)> create_reqsts= [](Msg msg) {
	
	Testing::TestRequest tr;
	tr.name("testRequest");
	tr.author("rabbit");
	tr.date(DateTime().now());

	SYSTEMTIME time;
	GetLocalTime(&time);
	std::string time_tag = std::to_string(time.wYear)+'-'+std::to_string(time.wMonth)+'-'+std::to_string(time.wDay) 
		+ '-' + std::to_string(time.wHour)+ '-' + std::to_string(time.wMinute) + '-' + std::to_string(time.wSecond);
	std::string reqFilePath = "../SendFile/tr_"+time_tag+".tr";
	std::vector<std::string>  reqs = decode(msg.attributes()["reqs"]);
	for (auto& req : reqs) {
		tr.request.valueRef().push_back(req);
	}

	std::ofstream out;
	if (Utilities::openWriteFile(reqFilePath, out))
	{
		out << tr.toString();
	}
	std::cout << "\n";

	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getReqFiles");
	size_t count = 0;
	Files files = Server::getFiles("*.tr");
	for (auto& file : files) {
		reply.attribute("file" + std::to_string(++count), file);
	}
	return reply;
};

int main()
{
  std::cout << "\n  Testing Server Prototype";
  std::cout << "\n ==========================";
  std::cout << "\n";


  Server server(serverEndPoint, "ServerPrototype");
  server.start();

  //std::cout << "\n  testing getFiles and getDirs methods";
  //std::cout << "\n --------------------------------------";
  //Files files = server.getFiles("*.dll");
  //show(files, "Files:");
  //Dirs dirs = server.getDirs();
  //show(dirs, "Dirs:");
  //std::cout << "\n";

  std::cout << "\n  testing message processing";
  std::cout << "\n ----------------------------";
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("create_reqsts", create_reqsts);
  
  server.processMessages();
  
  Msg msg(serverEndPoint, serverEndPoint);  // send to self
  msg.name("msgToSelf");
  //msg.command("echo");
  //msg.attribute("verbose", "show me");
  //server.postMessage(msg);
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  //msg.command("getFiles");
  //msg.remove("verbose");
  //msg.attributes()["path"] = storageRoot;
  //server.postMessage(msg);
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  //msg.command("getDirs");
  //msg.attributes()["path"] = storageRoot;
  //server.postMessage(msg);
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << "\n  press enter to exit";
  std::cin.get();
  std::cout << "\n";

  msg.command("serverQuit");
  server.postMessage(msg);
  server.stop();
  return 0;
}

