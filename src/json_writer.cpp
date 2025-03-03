#include "json_writer.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

bool writeToJson(const string &filename, const map<int32_t, Packet> &packets)
{
  json outputJson = json::array();

  for (const auto &[seq, pkt] : packets)
  {
    outputJson.push_back({{"symbol", string(pkt.symbol, 4)},
                          {"side", string(1, pkt.buySellIndicator)},
                          {"quantity", pkt.quantity},
                          {"price", pkt.price},
                          {"sequence", pkt.sequence}});
  }

  ofstream outFile(filename);
  if (!outFile)
    return false;

  outFile << outputJson.dump(4);
  outFile.close();
  return true;
}
