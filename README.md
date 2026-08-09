# C++ HTTP Server

A multithreaded HTTP server built from scratch in C++ using the Winsock API. The server handles HTTP requests, serves static files along with standard HTTP response, designed to understand networking, HTTP Protocol, socket programming and concurrent client handling.

## Features

* TCP socket communication using Winsock
* HTTP request parsing
* Request line, headers, query parameters, and body parsing
* Static file serving
* MIME type detection
* Multithreaded client handling
* HTTP response generation

## Tech Stack

* C++17
* Winsock2
* TCP/IP
* HTTP/1.1
* Multithreading

## Project Structure

```text
HTTP-Server/
├── src/
├── include/
├── public/
├── main.cpp
├── Makefile
├── .gitignore
└── README.md
```

## Build & Run

```bash
git clone https://github.com/Vaibhavkshyp/http-server-cpp.git
```

```bash
cd http-server-cpp
```

```bash
make
```

```bash
./server
```

Then open:

```text
http://localhost:5000
```

## Key Concepts

**TCP Sockets • HTTP • Request Parsing • Multithreading • File I/O • C++17**

## Author

**Vaibhav Kashyap**
