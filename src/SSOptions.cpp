#include "SSOptions.hpp"

#include "Game.hpp"
#include "TerminalColors.hpp"

#include <curses.h>

namespace MORL {
  SSOptions::SSOptions(Game &game)
    : ScreenState(game)
  {}

  void SSOptions::Update() {
    auto &keyboard = mGame.Keyboard();
    if(keyboard.IsKeyDown('q')) {
      mGame.Screen().GoBack();
    }
  }

  void SSOptions::Draw() {
    clear();
    printw("Hello, I'm the options menu!\n");
    printw("Press q to go back!\n\n");

    SetColor(TerminalColor::Error);
    printw("This is a error\n");

    SetColor(TerminalColor::Important);
    printw("I'm important!\n");

    SetColor(TerminalColor::Warning);
    printw("Not enough minerals!\n");

    SetColor(TerminalColor::Default);
    printw("Unimportant text\n");
  }
}