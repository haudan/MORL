#include <curses.h>

#include "Gameplay/Game.hpp"
#include "Gameplay/PlayerEntity.hpp"

#include "platform_sockets.hpp"

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

      if(!DoesLocalPlayerExist()) {
        // TODO: The below 2 lines probably won't work consistently
        mConnectedPlayers.push_back({updatePacket.name, newPos});
        mLocalPlayer = &mConnectedPlayers[mConnectedPlayers.size() - 1];
        auto id = mWorld.AddEntity<PlayerEntity>(*mLocalPlayer);
        mLocalPlayerEntity = static_cast<PlayerEntity*>(mWorld.GetEntity(id));
      }
      else {
        mLocalPlayer->Position(newPos);
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
    #endif
  }
}