#pragma once

#include "Keyboard.hpp"
#include "ScreenState.hpp"
#include "SSMenu.hpp"
#include "SSServerMenu.hpp"
#include "Screen.hpp"
#include "Util.hpp"
#include "IPEndpoint.hpp"
#include "UdpSocket.hpp"
#include "Network/Session.hpp"
#include "Gameplay/Game.hpp"
#include "Network/ClientCommand.hpp"

#include <queue>

namespace MORL {
  class Game {
  public:
    using OwnedClientCommand = std::unique_ptr<Network::ClientCommand>;
    using ClientCommandQueue = std::queue<OwnedClientCommand>;

    Game() = default;
    Game(Game const &other) = delete;
    ~Game() = default;

    void Update();
    void Draw();

    inline bool Running() const {
      return mRunning;
    }

    inline void Quit() {
      mRunning = false;
    }

    inline MORL::Keyboard const &Keyboard() const {
      return mKeyboard;
    }

    inline MORL::Screen &Screen() {
      return mScreen;
    }

    inline Network::Session &Session() {
      return mSession;
    }

    inline Gameplay::Game &GameplayGame() {
      return mGameplayGame;
    }

    inline ClientCommandQueue &ClientCommands() {
      return mClientCommands;
    }

    template <typename C, typename... Args>
    inline void IssueClientCommand(Args && ... args) {
      mClientCommands.push(MakeUnique<C>(std::forward<Args>(args)...));
    }
  private:
    bool mRunning = true;
    MORL::Keyboard mKeyboard;
    Network::Session mSession{*this};

    #ifdef MORL_SERVER_SIDE
    MORL::Screen mScreen{MakeUnique<MORL::SSServerMenu>(*this)};
    #else
    MORL::Screen mScreen{MakeUnique<MORL::SSMenu>(*this)};
    #endif

    ClientCommandQueue mClientCommands;
    Gameplay::Game mGameplayGame;
  };
}
