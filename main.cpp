#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

#include "HttpRequest.h"
#include "HttpParser.h"
#include "HttpResponse.h"
#include "Router.h"

int main()
{
    // 1. Create socket
    int server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    std::cout
        << "Socket created successfully\n";


    // 2. Prepare server address
    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr =
        htonl(INADDR_LOOPBACK);


    // 3. Bind socket to 127.0.0.1:8080
    if (::bind(
            server_fd,
            (struct sockaddr*)&address,
            sizeof(address)) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    std::cout
        << "Socket bound successfully\n";


    // 4. Listen for connections
    if (listen(server_fd, 10) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout
        << "Server is listening on "
        << "http://127.0.0.1:8080\n";


    // Create parser and router
    HttpParser parser;
    Router router;


    // 5. Keep accepting clients
    while (true)
    {
        // Accept a client
        int client_fd = accept(
            server_fd,
            nullptr,
            nullptr
        );

        if (client_fd == -1)
        {
            perror("accept");
            continue;
        }

        std::cout
            << "\nClient connected!\n";


        // 6. Receive HTTP request
        char buffer[4096];

        int bytes_received = recv(
            client_fd,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytes_received == -1)
        {
            perror("recv");
            close(client_fd);
            continue;
        }

        if (bytes_received == 0)
        {
            std::cout
                << "Client closed the connection\n";

            close(client_fd);
            continue;
        }

        std::cout
            << "Bytes received: "
            << bytes_received
            << "\n";


        // Display raw request
        std::cout
            << "Request:\n";

        std::cout.write(
            buffer,
            bytes_received
        );

        std::cout << "\n";


        // 7. Convert request to string
        std::string request_data(
            buffer,
            bytes_received
        );


        // 8. Parse HTTP request
        HttpRequest request =
            parser.parse(request_data);


        // 9. Display parsed request
        std::cout
            << "Method: "
            << request.getMethod()
            << "\n";

        std::cout
            << "Path: "
            << request.getPath()
            << "\n";

        std::cout
            << "Version: "
            << request.getVersion()
            << "\n";


        // 10. Route request
        HttpResponse httpResponse =
            router.route(request);


        // 11. Build HTTP response
        std::string response =
            httpResponse.build();


        // 12. Send response
        int bytes_sent = send(
            client_fd,
            response.c_str(),
            response.length(),
            0
        );

        if (bytes_sent == -1)
        {
            perror("send");
            close(client_fd);
            continue;
        }

        std::cout
            << "Bytes sent: "
            << bytes_sent
            << "\n";


        // 13. Close client connection
        close(client_fd);

        std::cout
            << "Client connection closed\n";
    }


    // This is technically unreachable because
    // the server currently runs forever.
    close(server_fd);

    return 0;
}