# ABX Stock Market Client

This project is a C++ client application that connects to the ABX mock exchange server over TCP, requests stock ticker data, detects missing packets, and generates a structured JSON output.

## Features

- Establishes a TCP connection with the ABX exchange server
- Requests and receives stock market data packets
- Identifies and retrieves missing packets for data integrity
- Stores the received data in a structured `output.json` file



## Installation & Running the Project

1. **Clone the repository**  
   ```sh
   git clone <repo-url> && cd abx_client
2. make
3. ./abx_client

By default, the client connects to:

IP: 127.0.0.1
Port: 3000