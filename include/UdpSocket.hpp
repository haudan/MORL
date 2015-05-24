#pragma once

#include "IPEndpoint.hpp"
#include "Util.hpp"

#include <cstdint>
#include <vector>
#include <memory>

#include <sstream>

class UdpSocket {
public:
  using IdTagType = uint32_t;

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
   * This only checks if the sizes of the packet and the available data match!
   */
  template <typename P>
  bool PacketAvailable() const {
    auto numAvailable = NumBytesAvailable();
    return numAvailable >= static_cast<int>(sizeof(IdTagType) + sizeof(P));
  }

  template <typename P>
  bool PacketAvailable(ReadResult const &result) const {
    P p;
    IPEndpoint ep;
    return ReadPacket(result, p, ep);
  }

  template <typename P>
  bool ReadPacket(ReadResult const &result, P &out, IPEndpoint &from) const {
    if(result.data.size() < sizeof(IdTagType) + sizeof(P)) {
      return false;
    }

    IdTagType packetId = ntohl(*((IdTagType*)result.data.data()));
    IdTagType expectedPacketId = TypeId<P>();
    if(packetId != expectedPacketId) {
      return false;
    }

    memcpy(&out, result.data.data() + sizeof(IdTagType), sizeof(P));
    from = result.from;
    return true;
  }

  template <typename P>
  bool ReadPacket(P &out, ReadResult &outResult, IPEndpoint &from) {
    auto result = ReadAll();
    outResult = result;
    if(result.data.size() < sizeof(IdTagType) + sizeof(P)) {
      return false;
    }

    IdTagType packetId = ntohl(*((IdTagType*)result.data.data()));
    IdTagType expectedPacketId = TypeId<P>();
    if(packetId != expectedPacketId) {
      return false;
    }

    memcpy(&out, result.data.data() + sizeof(IdTagType), sizeof(P));
    from = result.from;
    return true;
  }

  template <typename P>
  bool ReadPacket(P &out, ReadResult &outResult) {
    IPEndpoint unused2;
    return ReadPacket(out, outResult, unused2);
  }

  template <typename P>
  bool ReadPacket(P &out, IPEndpoint &from) {
    ReadResult unused;
    return ReadPacket(out, unused, from);
  }

  template <typename P>
  bool ReadPacket(P &out) {
    ReadResult unused;
    IPEndpoint unused2;
    return ReadPacket(out, unused, unused2);
  }

  /**
   * Write data to the socket!
   */
  void Write(IPEndpoint const &dest, void const *dataSrc, size_t dataSize);

  template <typename P>
  void WritePacket(IPEndpoint const &dest, P const &packet) {
    static_assert(std::is_trivial<P>::value, "The packet type must be trivially copyable!");

    constexpr auto packetSize = sizeof(P);
    constexpr auto idTagSize = sizeof(IdTagType);
    constexpr auto totalDataSize = packetSize + idTagSize;

    IdTagType packetId = htonl(TypeId<P>());

    std::unique_ptr<uint8_t[]> dataBuf{new uint8_t[totalDataSize]};
    memcpy(dataBuf.get(), &packetId, idTagSize);
    memcpy(dataBuf.get() + idTagSize, (void*)(&packet), packetSize);

    /*std::stringstream ss;
    ss << "Attemtping to send " << totalDataSize << " bytes.\n";
    ss << idTagSize << " bytes for id tag, " << packetSize << " bytes for the packet.\n";
    MessageBox(nullptr, ss.str().c_str(), "UdpSocket.WritePacket",
               MB_ICONINFORMATION | MB_OK);*/

    Write(dest, (void*)(dataBuf.get()), totalDataSize);
  }

  uint16_t LocalPort() const;
private:
  bool IsSocketValid() const;
  void InvalidateSocket();
private:
  int mSocket = -1;
  IPEndpoint mAddr;
};