#include <curses.h>

#include "SSGame.hpp"

#include "Game.hpp"

namespace MORL {
  SSGame::SSGame(Game &game)
    : ScreenState(game)
  {}

  void SSGame::Update() {
    if(mGame.Keyboard().IsKeyDown('q')) {
      mGame.Screen().GoBack();
      return;
    }

    mGame.GameplayGame().Update();
    // This really doesn't belong here. Or does it?

    RequireRedraw();
  }

  void SSGame::Draw() {
    clear();
    mGame.GameplayGame().Draw();
    mGame.GameplayGame().DrawToTerminal(mGame.Screen().Width(), mGame.Screen().Height());
  }
}