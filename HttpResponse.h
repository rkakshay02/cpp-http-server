#pragma once

#include <string>

class HttpResponse
{
private:
    std::string status;
    std::string contentType;
    std::string body;

public:
    HttpResponse(
        const std::string& status,
        const std::string& contentType,
        const std::string& body
    )
        : status(status),
          contentType(contentType),
          body(body)
    {
    }

    std::string build()
    {
        std::string response = status;

        response +=
            "Content-Type: "
            + contentType
            + "\r\n";

        response +=
            "Content-Length: "
            + std::to_string(body.length())
            + "\r\n";

        response +=
            "Connection: close\r\n";

        response += "\r\n";

        response += body;

        return response;
    }
};