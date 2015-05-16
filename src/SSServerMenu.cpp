#include <curses.h>

#include "SSServerMenu.hpp"

#include "Game.hpp"

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