#include "TTServer.h"
#include <functional>
#include <iostream>
#include "Network/include/Server.h"
#include <string>
#include <vector>
#include <ranges>


int main()
{
    TTServer srv;
    while (srv.isRunning());
}

TTServer::TTServer()
{
    srv = new Server();
    srv->Start("13578");
    srv->SetDataHandler(std::bind(&TTServer::HandleData, this, std::placeholders::_1, std::placeholders::_2));
}

bool TTServer::isRunning()
{
    return srv->running;
}

TTServer::~TTServer()
{
    delete srv;
}

void TTServer::HandleData(const std::string& data, int socket)
{
    std::cout << data << std::endl;
    const std::string& delim = ";";
    std::vector<std::string> line;
    for (const auto& word : std::views::split(data, delim))
    {
        line.push_back(std::string(word.begin(), word.end()));
    }
    if (line.size() != 2)
    {
        return;
    }
    if (line[1] == "show")
    {
        ShowData(line[0], socket);
    }
    else if (line[1] == "start")
    {
        std::unique_lock<std::mutex>(srvMutex);
        temp_stack.insert(make_pair(line[0], std::vector<std::string>()));
    }
    else if (line[1] == "end")
    {
        SaveData(line[0]);
    }
    else
    {
        std::unique_lock<std::mutex>(srvMutex);
        if (temp_stack.contains(line[0]))
        {
            temp_stack[line[0]].emplace_back(line[1]);
        }
    }
}

void TTServer::SaveData(const std::string& data)
{
    std::unique_lock<std::mutex>(srvMutex);
    stack.emplace(data, temp_stack[data]);
    srv->SendData(data);
    auto it = temp_stack.find(data);
    if (it != temp_stack.end())
    {
        temp_stack.erase(it);
    }
    
}

void TTServer::ShowData(const std::string& data, int socket)
{
    std::unique_lock<std::mutex>(srvMutex);
    if (stack.contains(data))
    {
        srv->SendData(data + ";" + "start", socket);
        for (auto& line : stack[data])
        {
            srv->SendData(data + ";" + line, socket);
        }
        srv->SendData(data + ";" + "end", socket);
    }
}

