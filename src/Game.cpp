#include <curses.h>

#include "Game.hpp"

#include "FrameBuffer.hpp"

class FooEntity : public MORL::Gameplay::WorldEntity {
public:
  void Update() override {}
  void Draw(MORL::FrameBuffer &frameBuffer) const override {
    constexpr float DeltaX = 2.0f;
    constexpr float DeltaY = 1.0f;
    static float x = 0;
    static float y = 0;
    int const xi = (int)x;
    int const yi = (int)y;

    frameBuffer.CharAt(xi, yi, '#');

    if(xi >= frameBuffer.Width()) {
      x = 0;
      y += DeltaY;
    }

    if(yi >= frameBuffer.Height()) {
      x = 0;
      y = 0;
    }

    x += DeltaX;
  }
};

namespace MORL {
  Game::Game() {
    mGameplayGame.World().AddEntity(FooEntity{});
  }

  void Game::Update() {
    mKeyboard.Update();
    mSession.Update();
    mScreen.Update();
  }

  void Game::Draw() {
    mScreen.Refresh();
  }
}