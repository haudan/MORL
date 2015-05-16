#include "UdpSocket.hpp"

#include <stdexcept>
#include <algorithm>

#include "platform_sockets.hpp"

#ifdef _WIN32
constexpr int InvalidSocket = static_cast<int>(INVALID_SOCKET);
constexpr int SocketError = static_cast<int>(SOCKET_ERROR);
#else
constexpr int InvalidSocket = -1;
constexpr int SocketError = -1;
#endif

UdpSocket::UdpSocket(IPEndpoint const &addr)
  : mSocket(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)),
    mAddr(addr) {
  #ifdef _WIN32
  if(mSocket == InvalidSocket) {
    throw std::runtime_error("Creating socket failed!");
  }

  // Enter non-blocking mode
  u_long mode = 1;
  int res = ioctlsocket(mSocket, FIONBIO, &mode);
  if(res != 0) {
    throw std::runtime_error("Couldn't set socket to non-blocking!");
  }

  // Bind socket
  auto const addrin = static_cast<sockaddr_in>(addr);
  res = bind(mSocket, (const sockaddr*)&addrin, sizeof(addrin));
  if(res == SocketError) {
    throw std::runtime_error("Binding socket failed!");
  }
  #else
  if(mSocket == -1) {
    throw std::runtime_error("Creating socket failed!");
  }

  // Enter non-blocking mode
  int res = fcntl(mSocket, F_SETFL, O_NONBLOCK);
  if(res != 0) {
    throw std::runtime_error("Couldn't set socket to non-blocking!");
  }

  // Bind socket
  auto const addrin = static_cast<sockaddr_in>(addr);
  res = bind(mSocket, (const sockaddr*)&addrin, sizeof(addrin));
  if(res == SocketError) {
    throw std::runtime_error("Binding socket failed!");
  }
  #endif
}

UdpSocket::UdpSocket(UdpSocket && other)
  : mSocket(other.mSocket) {
  other.InvalidateSocket();
}

UdpSocket::~UdpSocket() {
  if(IsSocketValid()) {
    #ifdef _WIN32
    closesocket(mSocket);
    #else
    close(mSocket);
    #endif

    InvalidateSocket();
  }
}

bool UdpSocket::IsSocketValid() const {
  return mSocket != InvalidSocket;
}

void UdpSocket::InvalidateSocket() {
  mSocket = InvalidSocket;
}

int UdpSocket::NumBytesAvailable() const {
  #ifdef _WIN32
  u_long count = 0;
  ioctlsocket(mSocket, FIONREAD, &count);
  return static_cast<int>(count);
  #else
  int count = 0;
  ioctl(mSocket, FIONREAD, &count);
  return count;
  #endif
}

UdpSocket::ReadResult UdpSocket::Read(unsigned numBytes) {
  ReadResult result;
  result.data.resize(numBytes);

  sockaddr_in from;

  #ifdef _WIN32
  int fromLength = sizeof(from);
  int numRead = recvfrom(mSocket, (char*)(result.data.data()),
           static_cast<int>(numBytes), 0, (sockaddr*)(&from), &fromLength);
  #else
  unsigned fromLength = sizeof(from);
  int numRead = recvfrom(mSocket, (void*)(result.data.data()),
           numBytes, 0, (sockaddr*)(&from), &fromLength);
  #endif

  result.data.resize(std::max(numRead, 0));
  #ifdef _WIN32
  result.from.Address(ntohl(from.sin_addr.S_un.S_addr));
  #else
  result.from.Address(ntohl(from.sin_addr.s_addr));
  #endif
  result.from.Port(ntohs(from.sin_port));
  return result;
}

UdpSocket::ReadResult UdpSocket::ReadAll() {
  int avail = NumBytesAvailable();
  if(avail <= 0) {
    return {{}, {}};
  }

  return Read(static_cast<unsigned>(avail));
}

void UdpSocket::Write(IPEndpoint const &dest, void const *dataSrc, size_t dataSize) {
  auto addrin = static_cast<sockaddr_in>(dest);
  sendto(mSocket, (char*)dataSrc, dataSize, 0, (sockaddr*)&addrin, sizeof(addrin));
}
