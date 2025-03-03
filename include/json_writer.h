#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include "packet_handler.h"
#include <string>
#include <map>

bool writeToJson(const std::string &filename, const std::map<int32_t, Packet> &packets);

#endif
