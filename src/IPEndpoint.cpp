#include "IPEndpoint.hpp"

#include <sstream>
#include <stdexcept>
#include <iostream>

IPEndpoint::IPEndpoint(uint32_t addr, uint16_t port)
  : mAddr(addr), mPort(port)
{}

IPEndpoint::IPEndpoint(std::string const &addr) {
  uint32_t ip[4] = {0};

  // TODO: This can probably be done with std::stringstream
  /*int res = std::scanf("%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3], &mPort);
  if(res != 5) {
    throw std::runtime_error("Couldn't parse ip address from string!");
  }*/

  // Parse address
  char sep;
  std::stringstream ss{addr};
  for(int i = 0; i < 4; ++i) {
    uint32_t part = 0;
    if(!(ss >> part) || !(ss >> sep)) {
      throw std::runtime_error("Couldn't parse ip address from string!");
    }

    if(i < 3) {
      if(sep != '.') {
        throw std::runtime_error("Couldn't parse ip address from string!");
      }
    }
    else {
      if(sep != ':') {
        throw std::runtime_error("Couldn't parse ip address from string!");
      }
    }

    ip[i] = part;
  }

  // Parse port
  if(!(ss >> mPort)) {
    throw std::runtime_error("Couldn't parse ip address from string!");
  }

  // TODO: Fix obvious endianness problem!
  mAddr = (ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8) | ip[3];
}

IPEndpoint::operator std::string () const {
  std::stringstream ss;
  // TODO: And a endianness problem again!
  ss << ((mAddr >> 24) & 0xFF) << '.' << ((mAddr >> 16) & 0xFF) << '.'
     << ((mAddr >> 8) & 0xFF) << '.' << (mAddr & 0xFF) << ':' << mPort;
  return ss.str();
}