#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

static const std::unordered_set<std::string> methods = {
    "GET",
    "POST",
    "PUT",
    "DELETE",
    "PATCH",
    "HEAD",
    "OPTIONS"};

static const std::unordered_set<std::string> versions = {
    "HTTP/1.0",
    "HTTP/1.1",
    "HTTP/2"};

static const std::unordered_map<std::string, std::string> mimeTypes = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"}};