#pragma once

#include "Network/SessionState.hpp"
#include "IPEndpoint.hpp"
#include "Gameplay/Game.hpp"
#include "Gameplay/Player.hpp"
#include "Network/Session.hpp"

#include <unordered_map>
#include <functional>

class UdpSocket;

namespace MORL {
  namespace Network {
    class StateGameplay : public SessionState {
    public:
      using NumClientsChangeCallback = std::function<void(std::unordered_map<IPEndpoint, Gameplay::Player> const&)>;

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
      UdpSocket &Socket() const {
        return mSession.Socket();
      }

      #ifdef MORL_SERVER_SIDE
      void ServerUpdate();
      void SendPlayerUpdate(IPEndpoint const &to);
      #else
      void ClientUpdate();
      #endif
    private:
      bool mWasMoved = false;
      #ifdef MORL_SERVER_SIDE
      std::unordered_map<IPEndpoint, Gameplay::Player> mConnectedClients;
      NumClientsChangeCallback mNumClientsChangeCallback = [](auto&){};
      #else

      #endif
    };
  }
}