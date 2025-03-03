#include <iostream>
#include "client.h"
#include "packet_handler.h"
#include "json_writer.h"

using namespace std;
int main()
{
  cout << "let's start the execution" << endl;
  string ip = "127.0.0.0";
  int port = 3000;
  int sock = connectToServer(ip, port);
  if (sock == -1)
  {
    cout << "error while connecting to the server" << endl;
    return 1;
  }

  map<int32_t, Packet> received_packets = requestPackets(sock);

  if (received_packets.empty())
  {
    cerr << "no packets received or failed to send the request" << endl;
  }
  int timeOut = 5; // implemeted this because the requestMissignFunction blocks the program execution after reeiving a packet and which is leading to a deadlock as it waits indefinitely for the server to send the missing packet. so using a timeout for this call such that received packets can be written in the json file.
  requestMissingPackets(ip, port, received_packets, timeOut);

  if (writeToJson("output.json", received_packets))
  {
    cout << "data has been written to the output.json file" << endl;
  }
  else
  {
    cerr << "Failed to write output.json!" << endl;
  }

  return 0;
}
