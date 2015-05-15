#include "SSServerMenu.hpp"

#include "Game.hpp"

#include <curses.h>

namespace MORL {
  SSServerMenu::SSServerMenu(Game &game)
    : ScreenState(game)
  {}

  void SSServerMenu::Update() {
    if(mGame.Keyboard().IsKeyDown('q')) {
      mGame.Quit();
    }
  }

  void SSServerMenu::Draw() {
    printw("Hello, I'm the server!");
  }
}