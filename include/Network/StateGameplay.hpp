#pragma once

#include "Network/SessionState.hpp"

namespace MORL {
  namespace Network {
    class Session;

    class StateGameplay : public SessionState {
    public:
      StateGameplay(Session &session);
      StateGameplay(StateGameplay const &) = delete;
      StateGameplay(StateGameplay && other);
      ~StateGameplay() = default;

      void Update() override;
    };
  }
}