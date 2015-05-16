#include "Game.hpp"

namespace MORL {
  void Game::Update() {
    mKeyboard.Update();
    mSession.Update();
    mScreen.Update();
  }

  void Game::Draw() {
    mScreen.Refresh();
  }
}