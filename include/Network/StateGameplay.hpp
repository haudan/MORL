#pragma once

#include "Network/SessionState.hpp"
#include "IPEndpoint.hpp"
#include "Gameplay/Game.hpp"

#include <vector>
#include <functional>

namespace MORL {
  namespace Network {
    class Session;

    class StateGameplay : public SessionState {
    public:
      using NumClientsChangeCallback = std::function<void(std::vector<IPEndpoint> const&)>;

      StateGameplay(Session &session);
      StateGameplay(StateGameplay const &) = delete;
      StateGameplay(StateGameplay && other);
      ~StateGameplay() = default;

      inline void SetNumClientsChangeCallback(NumClientsChangeCallback const &callback) {
        #ifdef MORL_SERVER_SIDE
        mNumClientsChangeCallback = callback;
        #else
        (void)callback;
        #endif
      }



      void Update() override;
    private:
      #ifdef MORL_SERVER_SIDE
      void ServerUpdate();
      #else
      void ClientUpdate();
      #endif
    private:
      bool mWasMoved = false;
      #ifdef MORL_SERVER_SIDE
      std::vector<IPEndpoint> mConnectedClients;
      NumClientsChangeCallback mNumClientsChangeCallback = [](auto&){};
      #endif
    };
  }
}