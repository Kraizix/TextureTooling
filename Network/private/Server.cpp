#include "Server.h"

#include <iostream>

Server::Server() : listenSocket(INVALID_SOCKET)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		throw std::runtime_error("WSAStartup failed");
	}
}

void Server::Start(const std::string& port)
{
	struct addrinfo* result = nullptr,
		* ptr = nullptr,
		hints;

	ZeroMemory(&hints, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int err = getaddrinfo(nullptr, port.c_str(), &hints, &result);
	if (err)
	{
		WSACleanup();
		throw std::runtime_error("getaddrinfo failed");
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		throw std::runtime_error("socket failed");
	}

	err = bind(listenSocket, result->ai_addr, int(result->ai_addrlen));
	freeaddrinfo(result);
	if (err)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw std::runtime_error("bind failed");
	}

	err = listen(listenSocket, SOMAXCONN);
	if (err)
	{
		closesocket(listenSocket);
		WSACleanup();
		throw std::runtime_error("listen failed");
	}

	serverThread = std::thread(&Server::Listen, this);
	running = true;

}

void Server::SendData(const std::string& data)
{
	std::unique_lock<std::mutex>(clientMutexx);
	for (auto clientSocket : clientSockets)
	{
		send(clientSocket, data.c_str(), data.length(), 0);
	}
}

void Server::SendData(const std::string& data, int socket)
{
	send(socket, data.c_str(), data.length(), 0);
}

void Server::SetDataHandler(std::function<void(const std::string&, int)> handler)
{
	dataHandler = handler;
}

void Server::Stop()
{
	running = false;
	closesocket(listenSocket);
	{
		std::unique_lock<std::mutex>(clientMutexx);
		for (auto clientSocket : clientSockets)
		{
			closesocket(clientSocket);
		}
	}
	serverThread.join();
}

Server::~Server()
{
	Stop();
	WSACleanup();
}

void Server::Listen()
{
	while (running) {
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		if(clientSocket != SOCKET_ERROR)
		{
			std::unique_lock<std::mutex>(clientMutex);
			clientSockets.push_back(clientSocket);
		}
		std::thread(std::bind(&Server::HandleClient, this, clientSocket)).detach();
	}
}

void Server::HandleClient(SOCKET clientSocket)
{
	int bytesReceived;
	do
	{
		char buffer[1024];
		ZeroMemory(buffer, 1024);
		bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		{
			std::unique_lock<std::mutex>(clientMutex);
			std::cout << "Received: " << buffer << std::endl;
		}
		if (dataHandler) {
			dataHandler(std::string(buffer), clientSocket);
		}
	} while (bytesReceived > 0 && running);

	closesocket(clientSocket);

}
