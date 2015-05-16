#pragma once

#include "UdpSocket.hpp"
#include "IPEndpoint.hpp"
#include "Network/SessionState.hpp"

#include <cstdint>
#include <functional>
#include <stack>

namespace MORL {
  class Game;

  namespace Network {

    class Session;

    class StateNotRunning : public SessionState {
    public:
      StateNotRunning(Session &session)
        : SessionState(session)
      {}

      void Update() override {}
    };

    class Session {
    public:
      static constexpr uint16_t ServerSocketPort = 5666;

      using OwnedSessionState = std::unique_ptr<SessionState>;
      using SessionStateStack = std::stack<OwnedSessionState>;

      Session(Game &game);
      Session(Session const &) = delete;
      Session(Session && other);
      ~Session();

      inline UdpSocket &Socket() {
        return mSocket;
      }

      template <typename S>
      inline void PushState(S const &state) {
        mStates.push(MakeUnique<S>(state));
      }

      template <typename S>
      inline void ReplaceState(S const &state) {
        if(mStates.size() >= 1) {
          mStates.pop();
        }
        mStates.push(MakeUnique<S>(state));
      }

      template <typename S>
      inline void ReplaceState(S && state) {
        if(mStates.size() >= 1) {
          mStates.pop();
        }
        mStates.push(MakeUnique<S>(std::move(state)));
      }

      void GoBack();

      /**
       * Update internal stuff, call this once a frame
       */
      void Update();

      /**
       * Connect this client to the server
       */
      void ConnectToServer(IPEndpoint const &serverAddr,
                           std::function<void()> onConnect, std::function<void()> onFailure);
      //                   These are pass-by-val on purpose
    private:
      void SendDisconnectToServer();
      inline SessionState &CurrentState() {
        return *(mStates.top().get());
      }
    private:
      // How this socket is used varies from server to client side
      UdpSocket mSocket;
      Game &mGame;
      SessionStateStack mStates;
    };
  }
}