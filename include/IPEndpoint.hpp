#pragma once

#include <cstdint>
#include <string>

#include "platform_sockets.hpp"

class IPEndpoint {
public:
  IPEndpoint() = default;
  IPEndpoint(uint32_t addr, uint16_t port);
  IPEndpoint(uint16_t port);
  IPEndpoint(std::string const &addr);
  IPEndpoint(IPEndpoint const &other) = default;
  ~IPEndpoint() = default;

  inline uint32_t Address() const {
    return mAddr;
  }

  inline void Address(uint32_t addr) {
    mAddr = addr;
  }

  inline uint16_t Port() const {
    return mPort;
  }

  inline void Port(uint16_t port) {
    mPort = port;
  }

  operator std::string () const;
  operator sockaddr_in () const;
private:
  uint32_t mAddr = 0;
  uint16_t mPort = 0;
};