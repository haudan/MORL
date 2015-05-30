#pragma once

#include "ScreenState.hpp"
#include "IPEndpoint.hpp"
#include "Gameplay/Player.hpp"

#include <unordered_map>

namespace MORL {
  class Game;

  /**
   * The server's main screen
   */
  class SSServerMenu : public ScreenState {
  public:
    SSServerMenu(Game &game);
    SSServerMenu(SSServerMenu const &other) = default;
    ~SSServerMenu() = default;

    void Update() override;
    void Draw() override;
  private:
    std::unordered_map<IPEndpoint, Gameplay::Player> const *mClients = nullptr;
  };
}