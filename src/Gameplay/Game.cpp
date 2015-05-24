#include <curses.h>

#include "Gameplay/Game.hpp"

#include <algorithm>

namespace MORL {
  namespace Gameplay {
    void Game::Update() {
      mWorld.Update();
    }

    void Game::Draw() {
      mFrameBuffer.Clear('.');
      mWorld.Draw(mFrameBuffer);
    }

    void Game::DrawToTerminal(int maxWidth, int maxHeight) {
      for(int y = 0; y < std::min(mFrameBuffer.Height(), maxHeight); ++y) {
        for(int x = 0; x < std::min(mFrameBuffer.Width(), maxWidth); ++x) {
          //mvprintw(y, x, "%c", mFrameBuffer.CharAt(x, y));
          mvaddch(y, x, mFrameBuffer.CharAt(x, y));
        }
      }
    }
  }
}