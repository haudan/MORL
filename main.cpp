#include "Game.hpp"

/*void WorldAddEntity() {
  #ifdef MORL_SERVER_SIDE
  #warning "Server side"
  #else
  #warning "Client side"
  #endif
}*/

int main() {
  MORL::Game game;

  while(game.Running()) {
    game.Update();
    game.Draw();
    SleepMs(33);
  }

  return 0;
}