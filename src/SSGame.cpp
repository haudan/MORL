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
    }

    mGame.GameplayGame().Update();
  }

  void SSGame::Draw() {
    clear();
    mGame.GameplayGame().Draw();
  }
}