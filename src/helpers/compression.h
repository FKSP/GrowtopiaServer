#ifndef GROWTOPIASERVER_COMPRESSION_H
#define GROWTOPIASERVER_COMPRESSION_H

#include <iostream>
#include <vector>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>

std::vector<uint8_t> compress(std::vector<uint8_t> const& data);
std::vector<uint8_t> decompress(std::vector<uint8_t> const& compressed);

#endif //GROWTOPIASERVER_COMPRESSION_H
