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

    if(keyboard.IsKeyDown('e')) {
      mGame.Screen().GotoScreen(MakeUnique<SSOptions>(mGame));
    }

    if(keyboard.IsKeyDown('w')) {
      mRenderIpPrompt = true;
      RequireRedraw();
    }
  }

  void SSMenu::Draw() {
    clear();
    SetColor(TerminalColor::HeroText);
    printw("Hey there, welcome to the main menu!\n");
    SetColor(TerminalColor::Default);
    printw("Press 'q' to quit, 'e' for the options menu\n");
    printw("Press 'w' to connect to a server");

    if(mRenderIpPrompt) {
      mvprintw(mGame.Screen().Height() - 1, 0, "IP: ");

      echo();
      char buf[80];
      getstr(buf);
      noecho();

      mvprintw(mGame.Screen().Height() - 1, 0, "You entered: %s", buf);
    }
  }
}