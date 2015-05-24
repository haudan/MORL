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

    }

    void Session::Update() {
      CurrentState().Update();
    }

    /*void Session::ConnectToServer(IPEndpoint const &serverAddr,
                                  std::function<void()> const &onConnect,
                                  std::function<void()> const &onFailure) {
      #ifndef MORL_SERVER_SIDE
      mStates.push(MakeUnique<StateConnectToServer>(*this, serverAddr, onConnect, onFailure));
      #else
      (void)serverAddr;
      (void)onConnect;
      (void)onFailure;
      throw std::runtime_error("Tried to call ConnectToServer on a server build!");
      #endif
    }*/
  }
}