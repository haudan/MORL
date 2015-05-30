#include <curses.h>

#include "Gameplay/Game.hpp"

#include <sstream>

namespace MORL {
  namespace Gameplay {
    void Game::Update() {
      mWorld.Update();
    }

    void Game::Draw() {
      mFrameBuffer.Clear(' ');
      mWorld.Draw(mFrameBuffer);
    }

    void Game::DrawToTerminal(int maxWidth, int maxHeight) {
      for(int y = 0; y < std::min(mFrameBuffer.Height(), maxHeight); ++y) {
        for(int x = 0; x < std::min(mFrameBuffer.Width(), maxWidth); ++x) {
          mvaddch(y, x, mFrameBuffer.CharAt(x, y));
        }
      }
    }

    #ifndef MORL_SERVER_SIDE
    void Game::LocalPlayerUpdate(Network::PlayerUpdatePacket const &updatePacket) {
      Vec2i newPos{int(ntohl(updatePacket.newX)), int(ntohl(updatePacket.newY))};
      uint32_t playerId = ntohl(updatePacket.playerId);
      bool isMe = updatePacket.isMe;

      if(isMe) {
        if(!mLocalPlayer) {
          mLocalPlayer.reset(new Player{updatePacket.name, newPos});
          auto entityId = mWorld.AddEntity<PlayerEntity>(*mLocalPlayer);
          mLocalPlayerEntity = static_cast<PlayerEntity*>(mWorld.GetEntity(entityId));
        }
        else {
          mLocalPlayer->Name(updatePacket.name);
          mLocalPlayer->Position(newPos);
        }
      }
      else {
        // Is this player new to me?
        auto iter = mConnectedPlayers.find(playerId);
        if(iter == mConnectedPlayers.end()) {
          // Create a player entity for them, then!
          mConnectedPlayers[playerId] = {updatePacket.name, newPos};
          mWorld.AddEntity<PlayerEntity>(mConnectedPlayers[playerId]);
          std::stringstream ss;
          ss << "Spieler mit ID " << playerId << " hinzugefügt.";
          MessageBox(nullptr, ss.str().c_str(), "", 0);
        }
        else {
          mConnectedPlayers[playerId] = {updatePacket.name, newPos};
        }
      }
    }

    void Game::LocalEntityEvent(Network::EntityEventPacket const &eventPacket) {
      using EventType = Network::EntityEventPacket::Type;
      using EntityType = Gameplay::WorldEntityType;

      Vec2i entityPos{int(ntohl(eventPacket.data.entityX)), int(ntohl(eventPacket.data.entityY))};

      switch(eventPacket.type) {
        case EventType::NewEntity:
        {
          switch(eventPacket.data.entityType) {
            case EntityType::Player:
              //mWorld.AddEntity
              break;
          }
        }
      }
    }

    void Game::DisconnectPlayer(Network::DisconnectPacket const &discPacket) {
      uint32_t playerId = discPacket.playerId;

      std::stringstream ss;
      ss << "Spieler mit ID " << playerId << " hat das Spiel verlassen.";
      MessageBox(nullptr, ss.str().c_str(), "", 0);

      auto iter = mConnectedPlayers.find(playerId);
      if(iter != mConnectedPlayers.end()) {
        // Remove player entity
        Player &player = (*iter).second;
        mWorld.RemoveEntity(mWorld.FindEntity(player.PlayerEntity()));

        // Remove player from connected players list
        if(iter != mConnectedPlayers.end()) {
          mConnectedPlayers.erase(iter);
        }
      }
    }
    #endif
  }
}