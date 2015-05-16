#pragma once

namespace MORL {
  namespace Network {
    class Session;

    /**
     * Base class for all network states
     */
    class SessionState {
    public:
      SessionState(Session &session)
        : mSession(session)
      {}

      SessionState(SessionState const &other) = default;

      virtual ~SessionState() {}

      /**
       * Run the state's logic
       */
      virtual void Update() = 0;
    protected:
      Session &mSession;
    };
  }
}