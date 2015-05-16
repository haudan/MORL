#include "Network/StateGameplay.hpp"

namespace MORL {
  namespace Network {
    StateGameplay::StateGameplay(Session &session)
      : SessionState(session)
    {}

    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession)
    {}

    void StateGameplay::Update() {

    }
  }
}