#include <curses.h>

#include "SSMenu.hpp"

#include "Game.hpp"
#include "Terminal.hpp"
#include "SSOptions.hpp"
#include "SSGame.hpp"
#include "Network/StateConnectToServer.hpp"

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
      SetColor(TerminalColor::Warning);
      auto input = ReadString();
      SetColor(TerminalColor::Default);

      auto &session = mGame.Session();
      auto server = IPEndpoint::TryParseString(input);

      if(!server) {
        SetColor(TerminalColor::Error);
        mvprintw(mGame.Screen().Height() - 1, 0, "Ew, what kind of IP address is that!?");
        SetColor(TerminalColor::Default);
      }
      else {
        mvprintw(mGame.Screen().Height() - 1, 0, "Connecting...");
        session.PushState(Network::StateConnectToServer{session, *server, [&] {
          //mvprintw(mGame.Screen().Height() - 1, 0, "Successfully connected!");
          mGame.Screen().GotoScreen(MakeUnique<SSGame>(mGame));
        }, [&] {
          SetColor(TerminalColor::Error);
          mvprintw(mGame.Screen().Height() - 1, 0, "Something went wrong! The server probably timed out.");
          SetColor(TerminalColor::Default);
        }});
      }

      mRenderIpPrompt = false;
    }
  }
}
