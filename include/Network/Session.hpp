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

      Session(MORL::Game &game);
      Session(Session const &) = delete;
      Session(Session && other);
      ~Session();

      inline UdpSocket &Socket() {
        return mSocket;
      }

      inline MORL::Game &Game() {
        return mGame;
      }

      inline bool IsConnectedToServer() const {
        return mConnectedToServer;
      }

      inline void ConnectedToServer(IPEndpoint const &server) {
        mConnectedToServer = true;
        mServer = server;
      }

      inline IPEndpoint const &Server() const {
        return mServer;
      }

      template <typename S>
      inline void PushState(S const &state) {
        mStates.push(MakeUnique<S>(state));
      }

      template <typename S>
      inline void PushState(S && state) {
        mStates.push(MakeUnique<S>(std::move(state)));
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
    private:
      inline SessionState &CurrentState() {
        return *(mStates.top().get());
      }
    private:
      // How this socket is used varies from server to client side
      UdpSocket mSocket;
      MORL::Game &mGame;
      SessionStateStack mStates;
      // Only used by client
      IPEndpoint mServer;
      // Only used by client
      bool mConnectedToServer;
    };
  }
}