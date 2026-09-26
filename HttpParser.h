#pragma once

#include <string>
#include <sstream>

#include "HttpRequest.h"

class HttpParser
{
public:
    HttpRequest parse(const std::string& rawRequest)
    {
        // Find the end of the HTTP request line
        size_t position = rawRequest.find("\r\n");

        if (position == std::string::npos)
        {
            // Invalid request
            return HttpRequest("", "", "");
        }

        // Extract request line
        std::string request_line =
            rawRequest.substr(0, position);

        // Parse request line
        std::stringstream ss(request_line);

        std::string method;
        std::string path;
        std::string version;

        ss >> method >> path >> version;

        // Create and return HttpRequest object
        return HttpRequest(
            method,
            path,
            version
        );
    }
};