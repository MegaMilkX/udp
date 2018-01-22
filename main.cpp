#include "udp_server.h"

class Server : public UDPServer
{
public:
    void OnData(char* data, size_t sz, const std::string& from)
    {
        std::cout << data << std::endl;
    }
};

int main()
{
    Server server;
    server.Init(8686);
    
    
    
    
    
    
    
    std::getchar();
    
    server.Cleanup();
    
    return 0;
}