#pragma once

#include "ScreenState.hpp"

namespace MORL {
  class Game;

  class SSMenu : public ScreenState {
  public:
    SSMenu(Game &game);

    void Update() override;
    void Draw() override;
  private:
    bool mRenderIpPrompt = false;
    bool mIsConnected = false;
  };
}