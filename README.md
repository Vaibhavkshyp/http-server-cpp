# HTTP Server in C++

A lightweight multithreaded HTTP server built from scratch in C++ using the Windows Winsock API. The server handles HTTP GET requests, serves static files along with standard HTTP response, and demonstrates the fundamentals of socket programming and the HTTP protocol.

## Features

- Multithreaded client handling
- HTTP request parsing
- Static file serving
- MIME type detection
- Binary file support (images, favicon, etc.)
- Custom Error handling
- Built using Winsock APIs

## Tech Stack

- C++
- Winsock2
- Multithreading
- File I/O
- HTTP/1.1

## Project Structure

```
public/
src/
README.md
```

## How to Run

1. Clone the repository.
2. Build the project using a C++ compiler with Winsock support.
3. Run the executable.
4. Open your browser and visit:

```
http://localhost:5000
```

## Learning Objectives

This project was built to understand:

- TCP socket programming
- HTTP request/response lifecycle
- Multithreaded server design
- Static file serving
- MIME type handling
- Binary file transmission
