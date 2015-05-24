#pragma once

#include "IPEndpoint.hpp"
#include "UdpSocket.hpp"
#include "Network/Packets/PlayerMovementPacket.hpp"

namespace MORL {
  namespace Network {
    class ClientCommand {
    public:
      virtual void Execute(UdpSocket &socket, IPEndpoint const &server) = 0;
    };

    class PlayerMoveCommand : public ClientCommand {
    public:
      PlayerMoveCommand(PlayerMovementPacket::Direction direction)
        : mDirection(direction)
      {}

      void Execute(UdpSocket &socket, IPEndpoint const &server) override {
        socket.WritePacket(server, PlayerMovementPacket{mDirection});
      }
    private:
      PlayerMovementPacket::Direction mDirection;
    };
  }
}