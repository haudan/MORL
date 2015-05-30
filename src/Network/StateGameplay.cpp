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
            // Send list of all current players to the new one
            SendPlayerListing(from);
            // Add new player to the list of players
            GameplayGame().AddConnectedPlayer(from, newPlayer);
            GameplayGame().World().AddEntity<PlayerEntity>(newPlayer);
            socket.WritePacket(from, ConnectPacket{true});
            SendPlayerUpdate(from);
            // Now tell everyone you're here
            SendPlayerDataToAll(from);
            mNumClientsChangeCallback(GameplayGame().ConnectedPlayers());
          }
        }

        // Handle disconnect packet
        else if(socket.PacketAvailable<DisconnectPacket>(result)) {
          DisconnectPacket disconnect;
          socket.ReadPacket(result, disconnect, from);
          SendPlayerDisconnectToAll(from);
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
            SendPlayerDataToAll(from);
          }
        }
      }
    }

    size_t StateGameplay::PlayerIndex(IPEndpoint const &player) {
      auto iter = GameplayGame().ConnectedPlayers().find(player);
      size_t dist = std::distance(GameplayGame().ConnectedPlayers().begin(), iter);
      return dist;
    }

    void StateGameplay::SendPlayerUpdate(IPEndpoint const &forPlayer, IPEndpoint const &to) {
      auto *player = GameplayGame().GetConnectedPlayer(forPlayer);
      if(player) {
        PlayerUpdatePacket update;
        update.isMe = (forPlayer == to ? 1 : 0);
        auto id = PlayerIndex(forPlayer);
        update.playerId = htonl(static_cast<uint32_t>(id));
        auto const &pos = player->Position();
        update.newX = htonl(pos.X());
        update.newY = htonl(pos.Y());
        auto nameLength = std::min(player->Name().size(), PlayerUpdateMaxNameLength);
        auto name = player->Name().substr(0, nameLength);
        memcpy(update.name, name.c_str(), nameLength + 1);
        Socket().WritePacket(to, update);
      }
    }

    void StateGameplay::SendPlayerListing(IPEndpoint const &to) {
      for(auto &player : GameplayGame().ConnectedPlayers()) {
        SendPlayerUpdate(player.first, to);
      }
    }

    void StateGameplay::SendPlayerDataToAll(IPEndpoint const &newPlayer) {
      for(auto &player : GameplayGame().ConnectedPlayers()) {
        if(player.first != newPlayer) {
          SendPlayerUpdate(newPlayer, player.first);
        }
      }
    }

    void StateGameplay::SendPlayerDisconnectToAll(IPEndpoint const &disconnectedPlayer) {
      for(auto &player : GameplayGame().ConnectedPlayers()) {
        if(player.first != disconnectedPlayer) {
          uint32_t discPlayerId = static_cast<uint32_t>(PlayerIndex(disconnectedPlayer));
          Socket().WritePacket<DisconnectPacket>(player.first, {discPlayerId});
        }
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
          GameplayGame().LocalPlayerUpdate(update);
        }

        // Handle entity event packet
        else if(socket.PacketAvailable<EntityEventPacket>(result)) {
          EntityEventPacket event;
          socket.ReadPacket(result, event, from);
          GameplayGame().LocalEntityEvent(event);
        }

        // Handle disconnect packet
        else if(socket.PacketAvailable<DisconnectPacket>(result)) {
          DisconnectPacket disconnect;
          socket.ReadPacket(result, disconnect, from);
          GameplayGame().DisconnectPlayer(disconnect);
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