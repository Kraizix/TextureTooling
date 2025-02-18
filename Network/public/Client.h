#pragma once
#include <string>
#include <thread>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mutex>
#include <functional>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
	Client();

	void ConnectToServer(const std::string& host, const std::string& port);
	void Disconnect();
	void SendData(const std::string& data);
	void SetDataHandler(std::function<void(const std::string&)> handler);
	~Client();

private:
	WSADATA wsaData;
	SOCKET clientSocket;
	std::thread recvThread;
	bool connected;
	std::mutex socketMutex;
	std::function<void(const std::string&)> dataHandler;

	void ReceiveData();
};

