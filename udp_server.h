#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define BUFLEN 512

class UDPServer
{
public:
    virtual ~UDPServer() {}
    
    bool Init(int port)
    {
        slen = sizeof(si_other);
    
        //Winsock2 init
        if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            std::cout << "WSAStartup failed: " << std::hex << WSAGetLastError() << std::endl;
            return false;
        }
        
        //Create socket
        if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
        {
            std::cout << "socket() failed: " << std::hex << WSAGetLastError() << std::endl;
            return false;
        }
        
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(port);
        
        //Bind
        if(bind(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        {
            std::cout << "bind() failed: " << std::hex << WSAGetLastError() << std::endl;
            return false;
        }
        
        running = true;
        hThread = CreateThread(NULL, 0, UDPServer::UDPServerThreadFunc, this, 0, &idThread);
        if(hThread == NULL)
        {
            std::cout << "CreateThread failed" << std::endl;
            return false;
        }
        
        return true;
    }
    
    void Cleanup()
    {
        running = false;
        closesocket(sock);
        WSACleanup();
    }
    
    virtual void OnData(char* data, size_t sz, const std::string& from) = 0;
private:
    static DWORD WINAPI UDPServerThreadFunc(void* lpParam)
    {
        UDPServer* serv = (UDPServer*)lpParam;

        while(serv->running)
        {
            memset(serv->buf, '\0', BUFLEN);
            
            if((serv->recv_len = recvfrom(serv->sock, serv->buf, BUFLEN, 0, (sockaddr*)&serv->si_other, &serv->slen)) == SOCKET_ERROR)
            {
                std::cout << "recvfrom() failed: " << std::hex << WSAGetLastError() << std::endl;
                continue;
            }
            
            std::string from = 
                std::string(inet_ntoa(serv->si_other.sin_addr))
                + ":"
                + std::to_string(ntohs(serv->si_other.sin_port));
            serv->OnData(serv->buf, serv->recv_len, from);
        }
        
        return 0;
    }
    
    bool running;

    SOCKET sock;
    sockaddr_in server, si_other;
    int slen, recv_len;
    char buf[BUFLEN];
    WSADATA wsa;
    
    HANDLE hThread;
    DWORD idThread;
};

#endif
