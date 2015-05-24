#include "Network/Session.hpp"

#include "Util.hpp"
#include "Game.hpp"

#include "Network/StateConnectToServer.hpp"

#include <memory>

namespace MORL {
  namespace Network {
    #ifdef MORL_SERVER_SIDE
    Session::Session(Game &game)
      : mSocket(IPEndpoint{ServerSocketPort}),
        mGame(game) {
      mStates.push(MakeUnique<StateNotRunning>(*this));
    }
    #else
    Session::Session(Game &game)
      : mSocket(IPEndpoint{0}),
        mGame(game) {
      mStates.push(MakeUnique<StateNotRunning>(*this));
    }
    #endif

    Session::Session(Session && other)
      : mSocket(std::move(other.mSocket)),
        mGame(other.mGame)
    { }

    Session::~Session() {
      SendDisconnectToServer();
    }

    void Session::GoBack() {
      if(mStates.size() <= 1) {
        throw std::runtime_error("You've reached the end of the stack, yo!");
      }

      mStates.pop();
    }

    void Session::SendDisconnectToServer() {
      // TODO: implement
    }

    void Session::Update() {
      CurrentState().Update();
    }
  }
}