#include "Network/StateGameplay.hpp"

#include "Network/Packets/EntityEventPacket.hpp"
#include "Network/Packets/ConnectPacket.hpp"
#include "Network/Packets/DisconnectPacket.hpp"

#include "Game.hpp"

namespace MORL {
  namespace Network {
    StateGameplay::StateGameplay(Session &session)
      : SessionState(session)
    {}

    #ifdef MORL_SERVER_SIDE
    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession),
        //mConnectedClients(std::move(other.mConnectedClients)),
        mNumClientsChangeCallback(other.mNumClientsChangeCallback) {
      other.mWasMoved = true;
    }
    #else
    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession) {
      other.mWasMoved = true;
    }
    #endif

    void StateGameplay::Update() {
      #ifdef MORL_SERVER_SIDE
      ServerUpdate();
      #else
      ClientUpdate();
      #endif
    }

    #ifdef MORL_SERVER_SIDE
    void StateGameplay::ServerUpdate() {
      using namespace Gameplay;

      UdpSocket &socket = Socket();

      auto numBytes = socket.NumBytesAvailable();
      if(numBytes > 0) {
        auto const result = socket.ReadAll();
        IPEndpoint from;

        // Handle connect packet
        if(socket.PacketAvailable<ConnectPacket>(result)) {
          ConnectPacket connect;
          socket.ReadPacket(result, connect, from);
          auto player = GameplayGame().GetConnectedPlayer(from);
          if(player) {
            // Client is already connected, so we answer with false!
            socket.WritePacket(from, ConnectPacket{false});
          }
          else {
            // Client is new, so we accept!
            Player newPlayer{"SomeName"};
            //mConnectedClients.emplace(from, newPlayer);
            GameplayGame().AddConnectedPlayer(from, newPlayer);
            GameplayGame().World().AddEntity<PlayerEntity>(newPlayer);
            socket.WritePacket(from, ConnectPacket{true});
            SendPlayerUpdate(from);
            mNumClientsChangeCallback(GameplayGame().ConnectedPlayers());
          }
        }

        // Handle disconnect packet
        else if(socket.PacketAvailable<DisconnectPacket>(result)) {
          DisconnectPacket disconnect;
          socket.ReadPacket(result, disconnect, from);

          // Remove the client from the list if he is on it
          GameplayGame().RemoveConnectedPlayer(from);
          mNumClientsChangeCallback(GameplayGame().ConnectedPlayers());
        }

        // Handle player movement packet
        else if(socket.PacketAvailable<PlayerMovementPacket>(result)) {
          PlayerMovementPacket move;
          socket.ReadPacket<PlayerMovementPacket>(result, move, from);
          // Check if the packets source is a connected client
          auto player = GameplayGame().GetConnectedPlayer(from);
          if(player) {
            // Handle the movement and send back the new position
            player->Move(move.direction);
            SendPlayerUpdate(from);
          }
        }
      }
    }

    void StateGameplay::SendPlayerUpdate(IPEndpoint const &to) {
      auto *player = GameplayGame().GetConnectedPlayer(to);
      if(player) {
        // Handle the movement and send back the new position
        PlayerUpdatePacket update;
        auto const &pos = player->Position();
        update.newX = htonl(pos.X());
        update.newY = htonl(pos.Y());
        auto nameLength = std::min(player->Name().size(), PlayerUpdateMaxNameLength);
        auto name = player->Name().substr(0, nameLength);
        memcpy(update.name, name.c_str(), nameLength + 1);
        Socket().WritePacket(to, update);
      }
    }

    #else
    void StateGameplay::ClientUpdate() {
      using namespace Gameplay;

      // Handle received packets
      UdpSocket &socket = mSession.Socket();
      auto numBytes = socket.NumBytesAvailable();
      if(numBytes > 0) {
        auto const result = socket.ReadAll();
        IPEndpoint from;

        // Handle player update packet
        if(socket.PacketAvailable<PlayerUpdatePacket>(result)) {
          PlayerUpdatePacket update;
          socket.ReadPacket(result, update, from);
          mSession.Game().GameplayGame().LocalPlayerUpdate(update);
        }

        // Handle entity event packet
        if(socket.PacketAvailable<EntityEventPacket>()) {
          EntityEventPacket event;
          socket.ReadPacket(result, event, from);
          mSession.Game().GameplayGame().LocalEntityEvent(event);
        }
      }

      // Handle local commands
      auto &clientCommands = mSession.Game().ClientCommands();
      while(!clientCommands.empty()) {
        auto &command = clientCommands.front();
        command->Execute(mSession.Socket(), mSession.Server());
        clientCommands.pop();
      }
    }
    #endif
  }
}