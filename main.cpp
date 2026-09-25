#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <sstream>
int main()
{
    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("socket");
        std::cout << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Socket created successfully\n";

    // 2. Prepare server address
    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // 3. Bind socket to 127.0.0.1:8080
    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1)
    {
        perror("bind");
        return 1;
    }

    std::cout << "Socket bound successfully\n";

    // 4. Listen for connections
    if (listen(server_fd, 10) == -1)
    {
        perror("listen");
        return 1;
    }

    std::cout << "Server is listening...\n";

    // 5. Accept a client
    int client_fd = accept(server_fd, nullptr, nullptr);

    if (client_fd == -1)
    {
        perror("accept");
        return 1;
    }

    std::cout << "Client connected!\n";


    std::cout << "Waiting to receive data...\n";

    char buffer[4096];

int bytes_received = recv(
    client_fd,
    buffer,
    sizeof(buffer),
    0
);

std::cout << "Bytes received: " << bytes_received << "\n";
std::cout.write(buffer, bytes_received);
std::cout << "\n";

std::string request(buffer, bytes_received);

size_t position = request.find("\r\n");

std::string request_line = request.substr(0, position);

std::stringstream ss(request_line);

std::string method;
std::string path;
std::string version;

ss >> method >> path >> version;

std::cout << "Method: " << method << "\n";
std::cout << "Path: " << path << "\n";
std::cout << "Version: " << version << "\n";

std::string status = "HTTP/1.1 200 OK\r\n";
std::string body;


if (path == "/")
{
    body = "Hello World";
}
else if (path == "/about")
{
    body = "About page";
}
else
{
    status = "HTTP/1.1 404 Not Found\r\n";
    body = "Not Found";
}

std::string response = "HTTP/1.1 200 OK\r\n";
response += "Content-Type: text/plain\r\n";
response += "Content-Length: " + std::to_string(body.length()) + "\r\n";

response += "\r\n";
response += body;

int bytes_sent = send(
    client_fd,
    response.c_str(),
    response.length(),
    0
);

if (bytes_sent == -1)
{
    perror("send");
    return 1;
}

std::cout << "Bytes sent: " << bytes_sent << "\n";
return 0;
}