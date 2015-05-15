#pragma once

#include "ScreenState.hpp"

namespace MORL {
  class Game;

  class SSOptions : public ScreenState {
  public:
    SSOptions(Game &game);

    void Update() override;
    void Draw() override;
  };
}