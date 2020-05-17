#include <iostream>
#include "rpc/AsyncServerImpl.h"

int main()
{
    std::cout << "GRPC Initialization..." << std::endl;
    kv_storage::AsyncServerImpl server("0.0.0.0:50051");
    server.Run();
    return 0;
}
