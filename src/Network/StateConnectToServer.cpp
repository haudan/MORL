#include "Network/StateConnectToServer.hpp"

#include "Network/Session.hpp"
#include "Network/Packets/ConnectPacket.hpp"
#include "Network/StateGameplay.hpp"

namespace MORL {
  namespace Network {
    StateConnectToServer::StateConnectToServer(
      Session &session, IPEndpoint const &server,
      SuccessCallback const &onSuccess, FailureCallback const &onFailure)
      : SessionState(session),
        mServer(server),
        mOnSuccess(onSuccess),
        mOnFailure(onFailure)
    {}

    void StateConnectToServer::Update() {
      // Send connect packet
      if(!mPacketSent) {
        mSession.Socket().WritePacket(mServer, ConnectPacket{false});
        mTimeoutStartPoint = TimeNow();
        mPacketSent = true;
      }
      else {
        if(TimeoutExceeded()) {
          // TODO: Put the actual TimeoutMs in that string
          mOnFailure("Server didn't respond in 5 seconds");
          mSession.GoBack();
        }

        // Listen for acknowledge packet
        if(mSession.Socket().PacketAvailable<ConnectPacket>()) {
          ConnectPacket connect;
          IPEndpoint from;
          if(mSession.Socket().ReadPacket(connect, from)) {
            if(from == mServer) {
              if(connect.serverAknowledged) {
                mOnSuccess();
                mSession.ReplaceState(StateGameplay{mSession});
              }
              else {
                mOnFailure("Server refused connection");
                mSession.GoBack();
              }
            }
          }
        }
      }
    }
  }
}