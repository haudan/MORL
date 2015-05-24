#pragma once

#include "ScreenState.hpp"

namespace MORL {
  class Game;

  class SSGame : public ScreenState {
  public:
    SSGame(Game &game);
    SSGame(SSGame const &other) = default;
    ~SSGame() = default;

    void Update() override;
    void Draw() override;
  };
}