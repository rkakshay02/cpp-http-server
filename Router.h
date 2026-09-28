#pragma once

#include <fstream>
#include <string>
#include <sstream>

#include "HttpRequest.h"
#include "HttpResponse.h"

class Router
{
public:

    HttpResponse route(const HttpRequest& request)
    {
        // Reject methods other than GET
        if (request.getMethod() != "GET")
        {
            return HttpResponse(
                "HTTP/1.1 405 Method Not Allowed\r\n",
                "text/html",
                "Method Not Allowed"
            );
        }

        // Home page
        if (request.getPath() == "/")
        {
            std::ifstream file("index.html");

            if (!file.is_open())
            {
                return HttpResponse(
                    "HTTP/1.1 404 Not Found\r\n",
                    "text/html",
                    "File Not Found"
                );
            }

            std::stringstream buffer;
            buffer << file.rdbuf();

            std::string body = buffer.str();

            return HttpResponse(
                "HTTP/1.1 200 OK\r\n",
                "text/html",
                body
            );
        }

        // About page
        if (request.getPath() == "/about")
        {
            return HttpResponse(
                "HTTP/1.1 200 OK\r\n",
                "text/html",
                "About page"
            );
        }

        // Unknown route
        return HttpResponse(
            "HTTP/1.1 404 Not Found\r\n",
            "text/html",
            "Not Found"
        );
    }
};