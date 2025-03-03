#include "client.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int connectToServer(const string &ip, int port)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    cerr << "socket connection failure" << endl;
    return -1;
  }

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

  if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
  {
    cerr << "Connection failed (make sure the backend server is running)" << endl;
    close(sock);
    return -1;
  }

  cout << "Connected to server(socket connected)" << endl;
  return sock;
}
