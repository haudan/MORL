#include "Gameplay/Game.hpp"

namespace MORL {
  namespace Gameplay {
    void Game::Update() {
      mWorld.Update();
    }

    void Game::Draw() {
      mWorld.Draw();
    }
  }
}