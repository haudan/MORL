#pragma once

#include "Network/SessionState.hpp"
#include "IPEndpoint.hpp"

#include <functional>
#include <chrono>

namespace MORL {
  namespace Network {
    class Session;

    class StateConnectToServer : public SessionState {
    public:
      using SuccessCallback = std::function<void()>;
      using FailureCallback = std::function<void()>;

      using TimePoint = std::chrono::system_clock::time_point;

      static constexpr auto TimeoutMs = 5000;

      StateConnectToServer(Session &session, IPEndpoint const &server,
                           SuccessCallback const &onSuccess, FailureCallback const &onFailure);

      StateConnectToServer(StateConnectToServer const &other) = default;
      ~StateConnectToServer() = default;

      void Update() override;
    private:
      inline TimePoint TimeNow() const {
        return std::chrono::system_clock::now();
      }

      inline bool TimeoutExceeded() const {
        auto dur = TimeNow() - mTimeoutStartPoint;
        return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() > TimeoutMs;
      }
    private:
      IPEndpoint mServer;
      SuccessCallback mOnSuccess;
      FailureCallback mOnFailure;
      bool mPacketSent = false;

      TimePoint mTimeoutStartPoint;
    };
  }
}