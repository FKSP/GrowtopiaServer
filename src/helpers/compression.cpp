#include "compression.h"

std::vector<uint8_t> compress(std::vector<uint8_t> const& data) {
    std::vector<uint8_t> compressed = std::vector<uint8_t>();

    boost::iostreams::filtering_ostream os = boost::iostreams::filtering_ostream();
    os.push(boost::iostreams::zlib_compressor());
    os.push(std::back_inserter(compressed));

    //os << data.data();
    //os.flush();
    boost::iostreams::write(os, (const char*) data.data(), data.size());

    return compressed;
}

std::vector<uint8_t> decompress(std::vector<uint8_t> const& compressed)
{
    std::vector<uint8_t> decompressed = std::vector<uint8_t>();

    boost::iostreams::filtering_ostream os = boost::iostreams::filtering_ostream();

    os.push(boost::iostreams::zlib_decompressor());
    os.push(std::back_inserter(decompressed));

    //boost::iostreams::write(os, reinterpret_cast<const char*>(&compressed[0]), compressed.size());
    boost::iostreams::write(os, (const char*) compressed.data(), compressed.size());

    return decompressed;
}

