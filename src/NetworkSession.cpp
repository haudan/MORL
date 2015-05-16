#include "NetworkSession.hpp"

#include <memory>

namespace MORL {
  #ifdef MORL_SERVER_SIDE
  NetworkSession::NetworkSession()
    : mSocket({ServerSocketPort})
  {}
  #else
  NetworkSession::NetworkSession()
    : mSocket(IPEndpoint{0})
  {}
  #endif

  NetworkSession::NetworkSession(NetworkSession && other)
    : mSocket(std::move(other.mSocket))
  {}

  NetworkSession::~NetworkSession() {
    SendDisconnectToServer();
  }

  void NetworkSession::SendDisconnectToServer() {

  }

  void NetworkSession::Update() {
    // Protocol stuff
  }

  void NetworkSession::ConnectToServer(IPEndpoint const &serverAddr,
                                       std::function<void()> onConnect, std::function<void()> onTimeout) {
    onConnect();
  }


}