#include <curses.h>

#include "SSGame.hpp"

#include "Game.hpp"
#include "Network/StateGameplay.hpp"

namespace MORL {
  SSGame::SSGame(Game &game)
    : ScreenState(game)
  {}

  void SSGame::Update() {
    using namespace Network;

    auto const &keyboard = mGame.Keyboard();

    if(keyboard.IsKeyDown('q')) {
      mGame.Screen().GoBack();
      return;
    }
    else if(keyboard.IsKeyDown('w')) {
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Up);
    }
    else if(keyboard.IsKeyDown('a')) {
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Left);
    }
    else if(keyboard.IsKeyDown('s')) {
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Down);
    }
    else if(keyboard.IsKeyDown('d')) {
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Right);
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