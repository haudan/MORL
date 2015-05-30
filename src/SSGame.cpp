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
    auto *localPlayer = mGame.GameplayGame().LocalPlayer();

    if(keyboard.IsKeyDown('q')) {
      mGame.Screen().GoBack();
      return;
    }
    else if(keyboard.IsKeyDown('w')) {
      if(localPlayer) { localPlayer->Move(PlayerMovementPacket::Direction::Up); }
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Up);
    }
    else if(keyboard.IsKeyDown('a')) {
      if(localPlayer) { localPlayer->Move(PlayerMovementPacket::Direction::Left); }
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Left);
    }
    else if(keyboard.IsKeyDown('s')) {
      if(localPlayer) { localPlayer->Move(PlayerMovementPacket::Direction::Down); }
      mGame.IssueClientCommand<PlayerMoveCommand>(PlayerMovementPacket::Direction::Down);
    }
    else if(keyboard.IsKeyDown('d')) {
      if(localPlayer) { localPlayer->Move(PlayerMovementPacket::Direction::Right); }
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