#pragma once

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
            return HttpResponse(
                "HTTP/1.1 200 OK\r\n",
                "text/html",
                "Hello from Router"
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