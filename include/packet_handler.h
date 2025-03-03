#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <map>
#include<string>
#include <cstdint>

struct Packet
{
  char symbol[4];
  char buySellIndicator;
  int32_t quantity;
  int32_t price;
  int32_t sequence;
};

int32_t bigEndianToHost(int32_t value);
std::map<int32_t, Packet> requestPackets(int sock);
void requestMissingPackets(const std::string &ip, int port, std::map<int32_t, Packet> &packets, int timeOutSeconds);

#endif
