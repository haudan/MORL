#pragma once

#include "ScreenState.hpp"
#include "IPEndpoint.hpp"

#include <vector>

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
    std::vector<IPEndpoint> const *mClients = nullptr;
  };
}