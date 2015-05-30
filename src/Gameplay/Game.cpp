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
    void Game::LocalPlayerUpdate(Network::PlayerUpdatePacket &updatePacket) {
      Vec2i newPos{int(ntohl(updatePacket.newX)), int(ntohl(updatePacket.newY))};

      if(!mLocalPlayer) {
        mLocalPlayer.reset(new Player{updatePacket.name, newPos});
        mWorld.AddEntity<PlayerEntity>(*mLocalPlayer);
      }
      else {
        mLocalPlayer->Position(newPos);
      }
    }
    #endif
  }
}