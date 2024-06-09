#include "Log.h"
#include "TcpServer.h"
#include <cstdlib>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("./a.out port path\n");
        return -1;
    }
    unsigned short port = atoi(argv[1]);
    Debug("切换服务器的工作路径: %s", argv[2]);
    chdir(argv[2]);
    auto* server = new TcpServer(port, 4);
    server->run();
    return 0;
}