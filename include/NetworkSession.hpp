#pragma once

#include "UdpSocket.hpp"
#include "IPEndpoint.hpp"

#include <cstdint>
#include <functional>

namespace MORL {
  class NetworkSession {
  public:
    static constexpr uint16_t ServerSocketPort = 5666;

    NetworkSession();
    NetworkSession(NetworkSession const &) = delete;
    NetworkSession(NetworkSession && other);
    ~NetworkSession();

    /**
     * Update internal stuff, call this once a frame
     */
    void Update();

    /**
     * Connect this client to the server
     */
    void ConnectToServer(IPEndpoint const &serverAddr,
                         std::function<void()> onConnect, std::function<void()> onTimeout);
    //                   These are pass-by-val on purpose
  private:
    void SendDisconnectToServer();
  private:
    // How this socket is used varies from server to client side
    UdpSocket mSocket;
  };
}