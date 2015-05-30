#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <functional>

#include "platform_sockets.hpp"

#include "Gameplay/Player.hpp"

class IPEndpoint {
public:
  IPEndpoint() = default;
  IPEndpoint(uint32_t addr, uint16_t port);
  IPEndpoint(uint16_t port);
  IPEndpoint(std::string const &addr);
  IPEndpoint(IPEndpoint const &other) = default;
  ~IPEndpoint() = default;

  static std::unique_ptr<IPEndpoint> TryParseString(std::string const &str);

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

  bool operator == (IPEndpoint const &other) const;
private:
  uint32_t mAddr = 0;
  uint16_t mPort = 0;
};

// WTF STL
inline bool operator == (std::pair<IPEndpoint const&, MORL::Gameplay::Player> const &pair, IPEndpoint const &endpoint) {
  return pair.first == endpoint;
}

// Hasher for IPEndpoint
namespace std {
  template <>
  struct hash<IPEndpoint> {
    size_t operator () (IPEndpoint const &endpoint) const {
      size_t h1 = std::hash<uint32_t>()(endpoint.Address());
      size_t h2 = std::hash<uint16_t>()(endpoint.Port());
      return h1 ^ (h2 << 1);
    }
  };
}