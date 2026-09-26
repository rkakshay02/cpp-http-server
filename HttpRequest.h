#pragma once

#include <string>

class HttpRequest
{
private:
    std::string method;
    std::string path;
    std::string version;

public:
    // Constructor
    HttpRequest(
        const std::string& method,
        const std::string& path,
        const std::string& version
    )
        : method(method),
          path(path),
          version(version)
    {
    }

    // Get HTTP method
    std::string getMethod() const
    {
        return method;
    }

    // Get request path
    std::string getPath() const
    {
        return path;
    }

    // Get HTTP version
    std::string getVersion() const
    {
        return version;
    }
};