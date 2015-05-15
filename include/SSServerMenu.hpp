#pragma once

#include "ScreenState.hpp"

namespace MORL {
  class Game;

  /**
   * The server's main screen
   */
  class SSServerMenu : public ScreenState {
  public:
    SSServerMenu(Game &game);

    void Update() override;
    void Draw() override;
  };
}