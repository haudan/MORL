#pragma once

#include "IPEndpoint.hpp"

#include <cstdint>
#include <vector>

class UdpSocket {
public:
  struct ReadResult {
    std::vector<uint8_t> data;
    IPEndpoint from;
  };

  UdpSocket(IPEndpoint const &addr);
  UdpSocket(UdpSocket const &other) = delete;
  UdpSocket(UdpSocket && other);
  ~UdpSocket();

  /**
   * How many bytes are available on the socket to read?
   */
  int NumBytesAvailable() const;

  /**
   * Try to read the specified number of bytes of data on the socket
   */
  ReadResult Read(unsigned numBytes);

  /**
   * Read all data on the socket
   */
  ReadResult ReadAll();

  /**
   * Write data to the socket!
   */
  void Write(IPEndpoint const &dest, void const *dataSrc, size_t dataSize);

  template <typename P>
  void WritePacket(IPEndpoint const &dest, P const &packet) {
    static_assert(std::is_trivial<P>::value, "The packet type must be trivially copyable!");

    // Who knows if this works...
    Write(dest, (void*)(&packet), sizeof(P));
  }
private:
  bool IsSocketValid() const;
  void InvalidateSocket();
private:
  int mSocket = -1;
  IPEndpoint mAddr;
};