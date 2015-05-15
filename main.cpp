#include <curses.h>
#include "Game.hpp"

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "Util.hpp"
#include "UdpSocket.hpp"

int main() {
  #ifdef _WIN32
  WSAData wsaData;
  int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  ScopeGuard wsaGuard{[]{ WSACleanup(); }};

  if(result != 0) {
    throw std::runtime_error("WSAStartup failed!");
  }
  #endif

  MORL::Game game;

  while(game.Running()) {
    game.Update();
    game.Draw();
    SleepMs(33);
  }

  return 0;
}