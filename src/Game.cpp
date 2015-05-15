#include "Game.hpp"

namespace MORL {
  void Game::Update() {
    mKeyboard.Update();
    mScreen.Update();
  }

  void Game::Draw() {
    mScreen.Refresh();
  }
}