#pragma once
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class Server
{

public:
	Server();
	void Start(const std::string& port);
	void SendData(const std::string& data);
	void SendData(const std::string& data, int socket);
	void SetDataHandler(std::function<void(const std::string&, int)> handler);
	void Stop();
	~Server();

	bool running = false;

private:
	WSADATA wsaData;
	SOCKET listenSocket;
	std::vector<SOCKET> clientSockets;
	std::mutex clientMutex;
	std::thread serverThread;
	
	std::function<void(const std::string&, int)> dataHandler;

	void Listen();
	void HandleClient(SOCKET clientSocket);
	

};

