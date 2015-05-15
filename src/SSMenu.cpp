#include "SSMenu.hpp"

#include "Game.hpp"
#include "TerminalColors.hpp"
#include "SSOptions.hpp"

#include <curses.h>

namespace MORL {
  SSMenu::SSMenu(Game &game)
  : ScreenState(game)
  {}

  void SSMenu::Update() {
    auto &keyboard = mGame.Keyboard();
    if(keyboard.IsKeyDown('q')) {
      mGame.Quit();
    }

    if(keyboard.IsKeyDown(KEY_F(1))) {
      auto &screen = mGame.Screen();
      screen.GotoScreen(MakeUnique<SSOptions>(mGame));
    }
  }

  void SSMenu::Draw() {
    clear();
    SetColor(TerminalColor::HeroText);
    printw("Hey there, welcome to the main menu!\n");
    SetColor(TerminalColor::Default);
    printw("Press 'q' to quit, F1 for the options menu\n");
  }
}