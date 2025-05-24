# Client-Server Program in C

## Overview

This project implements a simple client-server application in C using **TCP/IP sockets**. The server listens on a specified port and accepts incoming connections from clients. Once connected, the client can send messages to the server, which responds accordingly.

This is a fundamental example demonstrating inter-process communication over a network.

## Features

- TCP-based reliable communication
- Server handles one client at a time (extendable to multiple clients)
- Simple request-response model
- Basic error handling

## Files

- `server.c`: Source code for the server
- `client.c`: Source code for the client
- `Makefile` *(optional)*: To compile the programs easily
- `README.md`: This documentation file

## Requirements

- GCC or any C compiler
- Unix/Linux environment (Windows may need adaptation or use of WSL)
- Basic knowledge of networking and C programming

## Compilation

To compile the client and server:

```bash
gcc server.c -o server
gcc client.c -o client

./server [PORT]
./client [HOST IP] [PORT]
