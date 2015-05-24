#include "Network/StateGameplay.hpp"

#include "Network/Session.hpp"
#include "Network/Packets/ConnectPacket.hpp"

#include <algorithm>

namespace MORL {
  namespace Network {
    StateGameplay::StateGameplay(Session &session)
      : SessionState(session)
    {}

    #ifdef MORL_SERVER_SIDE
    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession),
        mConnectedClients(std::move(other.mConnectedClients)),
        mNumClientsChangeCallback(other.mNumClientsChangeCallback)
    {}
    #else
    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession)
    {}
    #endif

    void StateGameplay::Update() {
      #ifdef MORL_SERVER_SIDE
      ServerUpdate();
      #else
      ClientUpdate();
      #endif
    }

    #ifdef MORL_SERVER_SIDE
    void StateGameplay::ServerUpdate() {
      UdpSocket &socket = mSession.Socket();
      UdpSocket::ReadResult result;
      IPEndpoint from;

      // Might want to get rid of the stack frame
      { // Handle connect packet
        ConnectPacket connect;
        if(socket.ReadPacket(connect, result, from)) {
          auto iter = std::find(mConnectedClients.begin(), mConnectedClients.end(), from);
          if(iter != mConnectedClients.end()) {
            // Client is already connected, so we answer with false!
            socket.WritePacket(from, ConnectPacket{false});
          }
          else {
            // Client is new, so we accept!
            mConnectedClients.push_back(from);
            socket.WritePacket(from, ConnectPacket{true});
            mNumClientsChangeCallback(mConnectedClients);
          }
        }
      }
    }

    #else
    void StateGameplay::ClientUpdate() {

    }
    #endif
  }
}