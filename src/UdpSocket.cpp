#include "UdpSocket.hpp"

#include "platform_sockets.hpp"

#include <stdexcept>
#include <inaddr.h>

#ifdef _WIN32
constexpr int InvalidSocket = static_cast<int>(INVALID_SOCKET);
constexpr int SocketError = static_cast<int>(SOCKET_ERROR);
#else
constexpr int InvalidSocket = -1;
constexpr int SocketError = -1;
#endif

UdpSocket::UdpSocket()
  : mSocket(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) {
  #ifdef _WIN32
  if(mSocket == INVALID_SOCKET) {
    throw std::runtime_error("Creating socket failed!");
  }

  // Enter non-blocking mode
  u_long mode = 1;
  int res = ioctlsocket(mSocket, FIONBIO, &mode);
  if(res != 0) {
    throw std::runtime_error("Couldn't set socket to non-blocking!");
  }
  #else
  if(mSocket == -1) {
  throw std::runtime_error("Creating socket failed!");
  }

  int res = fcntl(mSocket, F_SETFL, O_NONBLOCK);
  if(res != 0) {
    throw std::runtime_error("Couldn't set socket to non-blocking!");
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

int UdpSocket::NumBytesAvailable() const {
  #ifdef _WIN32
  u_long count = 0;
  ioctlsocket(mSocket, FIONREAD, &count);
  return static_cast<int>(count);
  #else
  int count = 0;
  ioctl(fd, FIONREAD, &count);
  return count;
  #endif
}

UdpSocket::ReadResult UdpSocket::Read(unsigned numBytes) {
  ReadResult result;
  result.data.reserve(numBytes);

  sockaddr_in from = {0};
  int fromLength = sizeof(from);

  int numRead = recvfrom(mSocket, (char*)(result.data.data()),
           static_cast<int>(numBytes), 0, (sockaddr*)(&from), &fromLength);

  // TODO: Do I need to convert these from network to host endianness?
  result.from.Address(from.sin_addr.S_un.S_addr);
  result.from.Port(from.sin_port);
  return result;
}

UdpSocket::ReadResult UdpSocket::ReadAll() {
  int avail = NumBytesAvailable();
  if(avail <= 0) {
    return {};
  }

  return Read(static_cast<unsigned>(avail));
}

bool UdpSocket::IsSocketValid() const {
  return mSocket != InvalidSocket;
}

void UdpSocket::InvalidateSocket() {
  mSocket = InvalidSocket;
}