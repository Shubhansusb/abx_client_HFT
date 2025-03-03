#include "packet_handler.h"
#include "client.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <unistd.h>

using namespace std;

int32_t bigEndianToHost(int32_t value)
{
  return ntohl(value);
}

map<int32_t, Packet> requestPackets(int sock)
{
  map<int32_t, Packet> packets;
  uint8_t request[2] = {1, 0};

  if (send(sock, request, sizeof(request), 0) != sizeof(request))
  {
    cerr << "Failed to send request." << endl;
    return packets;
  }

  uint8_t buffer[17];
  while (recv(sock, buffer, sizeof(buffer), 0) > 0)

  
  {
    Packet pkt{};
    memcpy(pkt.symbol, buffer, 4);
    pkt.buySellIndicator = buffer[4];
    pkt.quantity = bigEndianToHost(*reinterpret_cast<int32_t *>(&buffer[5]));
    pkt.price = bigEndianToHost(*reinterpret_cast<int32_t *>(&buffer[9]));
    pkt.sequence = bigEndianToHost(*reinterpret_cast<int32_t *>(&buffer[13]));

    packets[pkt.sequence] = pkt;
  }

  return packets;
}

void requestMissingPackets(const string &ip, int port, map<int32_t, Packet> &packets, int timeOutSeconds)
{
  if (packets.empty())
    return;

  vector<int32_t> missingSequences;
  // int32_t minSeq = packets.begin()->first;
  int32_t maxSeq = packets.rbegin()->first;

  for (int32_t seq = 1; seq <= maxSeq; ++seq)
  {
    if (packets.find(seq) == packets.end())
    {
      cout << "server hasn't sent the sequence " << seq << endl;
      missingSequences.push_back(seq);
    }
  }

  uint8_t buffer[17];
  for (int32_t seq : missingSequences)
  
  {
    cout << "attempting the resend seq for the sequence " << seq << endl;
    int sock_retry = connectToServer(ip, port);
    if (sock_retry == -1)
      continue;

    uint8_t retryRequest[5] = {2};
    memcpy(&retryRequest[1], &seq, 4);
    send(sock_retry, retryRequest, sizeof(retryRequest), 0);

    fd_set readfds;
    struct timeval timeout;
    timeout.tv_sec = timeOutSeconds; 
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(sock_retry, &readfds);

    int activity = select(sock_retry + 1, &readfds, nullptr, nullptr, &timeout);

    if (activity > 0 && FD_ISSET(sock_retry, &readfds))
    {
      if (recv(sock_retry, buffer, sizeof(buffer), 0) > 0)
      {
        Packet pkt{};
        memcpy(pkt.symbol, buffer, 4);
        pkt.buySellIndicator = buffer[4];
        pkt.quantity = bigEndianToHost(*reinterpret_cast<int32_t *>(&buffer[5]));
        pkt.price = bigEndianToHost(*reinterpret_cast<int32_t *>(&buffer[9]));
        pkt.sequence = bigEndianToHost(*reinterpret_cast<int32_t *>(&buffer[13]));

        packets[pkt.sequence] = pkt;
      }
    }
    else if (activity == 0)
    {
      cerr << "Timeout waiting for response for sequence " << seq << endl;
    }
    close(sock_retry);
  }
}
