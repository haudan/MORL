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

  UdpSocket();
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
private:
  bool IsSocketValid() const;
  void InvalidateSocket();
private:
  int mSocket = -1;
};