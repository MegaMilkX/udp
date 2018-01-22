#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 8686

int main()
{
    sockaddr_in si_other;
    int s, slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    WSADATA wsa;
    
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "WSAStartup failed" << std::endl;
    }
    
    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        std::cout << "socket() failed " << std::endl;
    }
    
    memset((char*)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
    
    while(1)
    {
        Sleep(100);
        POINT pt;
        GetCursorPos(&pt);
        std::string message = std::string("CURSOR: ") + std::to_string(pt.x) + "/" + std::to_string(pt.y);
        if(sendto(s, message.c_str(), message.size(), 0, (sockaddr*)&si_other, slen) == SOCKET_ERROR)
        {
            std::cout << "sendto() failed" << std::endl;
        }
    }
}
