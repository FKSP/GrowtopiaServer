#ifndef GROWTOPIASERVER_ENCODING_H
#define GROWTOPIASERVER_ENCODING_H

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

std::string encode_base64(const std::string &val);
std::string decode_base64(const std::string &val);

#endif //GROWTOPIASERVER_ENCODING_H
