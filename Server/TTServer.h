#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include "Network/include/Server.h"
class TTServer
{
public:
	TTServer();
	bool isRunning();
	~TTServer();
private:
	void HandleData(const std::string& data, int socket);
	void SaveData(const std::string& data);
	void ShowData(const std::string& data, int socket);
	Server* srv;
	std::unordered_map<std::string, std::vector<std::string>> stack;
	std::unordered_map<std::string, std::vector<std::string>> temp_stack;
	std::mutex srvMutex;
};