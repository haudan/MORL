#include <curses.h>

#include "Network/StateGameplay.hpp"

#include "Network/Session.hpp"
#include "Network/Packets/ConnectPacket.hpp"
#include "Network/Packets/DisconnectPacket.hpp"

#include <algorithm>
#include <sstream>

std::string VecDump(std::vector<uint8_t> const &vec) {
  std::stringstream ss;

  ss << "uint_8 array of size " << vec.size() << ":\n";
  for(size_t i = 0; i < vec.size(); ++i) {
    ss << (unsigned)vec[i] << (i < vec.size() - 1 ? ", " : "");
  }

  return ss.str();
}

namespace MORL {
  namespace Network {
    StateGameplay::StateGameplay(Session &session)
      : SessionState(session)
    {}

    #ifdef MORL_SERVER_SIDE
    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession),
        mConnectedClients(std::move(other.mConnectedClients)),
        mNumClientsChangeCallback(other.mNumClientsChangeCallback) {
      other.mWasMoved = true;
    }
    #else
    StateGameplay::StateGameplay(StateGameplay && other)
      : SessionState(other.mSession) {
      other.mWasMoved = true;
    }
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

      auto numBytes = socket.NumBytesAvailable();
      if(numBytes > 0) {
        auto const result = socket.ReadAll();
        IPEndpoint from;

        // Handle connect packet
        if(socket.PacketAvailable<ConnectPacket>(result)) {
          ConnectPacket connect;
          socket.ReadPacket(result, connect, from);
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

        // Handle disconnect packet
        else if(socket.PacketAvailable<DisconnectPacket>(result)) {
          DisconnectPacket disconnect;
          socket.ReadPacket(result, disconnect, from);

          // Remove the client from the list if he is on it
          auto iter = std::find(mConnectedClients.begin(), mConnectedClients.end(), from);
          if(iter != mConnectedClients.end()) {
            mConnectedClients.erase(iter);
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