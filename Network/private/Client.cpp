#include "Client.h"
#include <iostream>

Client::Client() : clientSocket(INVALID_SOCKET), connected(false)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw std::runtime_error("WSAStartup failed");
	}

}

void Client::ConnectToServer(const std::string& host, const std::string& port)
{
	struct addrinfo* result = nullptr,
		* ptr = nullptr,
		hints;

	ZeroMemory(&hints, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int err = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	if (err)
	{
		WSACleanup();
		throw std::runtime_error("getaddrinfo failed");
	} 

	clientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (clientSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		throw std::runtime_error("socket failed");
	}

	err = connect(clientSocket, result->ai_addr, int(result->ai_addrlen));
	freeaddrinfo(result);
	if (err)
	{
		closesocket(clientSocket);
		WSACleanup();
		throw std::runtime_error("connect failed");
	}

	connected = true;

	recvThread = std::thread(&Client::ReceiveData, this);
}

void Client::Disconnect()
{
	if (connected)
	{
		connected = false;
		closesocket(clientSocket);
		recvThread.join();
	}
}

void Client::SendData(const std::string& data)
{
	if (!connected)
		return;
	send(clientSocket, data.c_str(), data.size(), 0);
}

void Client::SetDataHandler(std::function<void(const std::string&)> handler)
{
	dataHandler = handler;
}

Client::~Client()
{
	Disconnect();
	WSACleanup();
}

void Client::ReceiveData()
{
	int bytesReceived;
	do
	{
		char buffer[1024];
		ZeroMemory(buffer, 1024);
		bytesReceived = recv(clientSocket, buffer, 1024, 0);
		if (bytesReceived > 0)
		{
			std::cout << buffer << '\n';
			if (dataHandler) {
				dataHandler(std::string(buffer));
			}
		}
	} while (bytesReceived > 0 && connected);
}
